#include <stdexcept>
#include <takisy/core/macro.h>
#include <takisy/core/ftp_client.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/core/stream.h>

std::string stream::read_all(void)
{
    stretchy_buffer<unsigned char> buffer(0);
    unsigned long length = buffer_stream(buffer).plunder(*this);

    return std::string((char*)buffer.data(), length);
}

std::string stream::read_line(void)
{
    std::string line;
    char ch = 0;

    while (read(ch) == sizeof(ch) && ch != '\n')
        line += ch;

    if (line.back() == '\r' && ch == '\n')
        line.pop_back();

    return std::move(line);
}

std::string stream::read_chars(unsigned long nchars)
{
    stretchy_buffer<char> buffer(nchars);
    unsigned long length = read(buffer.data(), buffer.size());

    return std::string(buffer.data(), length);
}

unsigned long stream::plunder(const char* uri)
{
    std::shared_ptr<class stream> stream = from_uri(uri);

    if (!stream)
        return 0;

    return plunder(*stream);
}

unsigned long stream::plunder(stream& src)
{
    stretchy_buffer<unsigned char> buffer(512 * 1024);
    unsigned long length = 0, read_length = 0;

    while ((read_length = src.read(buffer.data(), buffer.size())))
    {
        length += read_length;
        write(buffer.data(), read_length);
    }

    return length;
}

std::shared_ptr<stream> stream::from_uri(const char* uri)
{
    stralgo::strings pair = stralgo::split(uri, "://", 1);
    if (pair.size() == 1)
    {
        pair.resize(2);
        pair[0] = "file"; pair[1] = uri;
    }

    std::string protocol = stralgo::lowerc(pair[0]);
    if (protocol == "buffer")
    {
        class stream* stream =
                new buffer_stream(pair[1].c_str(), pair[1].size());

        return std::shared_ptr<class stream>(stream);
    }
    else
    if (protocol == "file" || protocol == "pipe")
    {
        class stream* stream = nullptr;
        pair = stralgo::split(pair[1], "@", 1);

        if (protocol == "file")
        {
            if (pair.size() == 1)
                stream = new file_stream(pair[0].c_str());
            else
            if (pair.size() == 2)
                stream = new file_stream(pair[1].c_str(), pair[0].c_str());
        }
        else
        if (protocol == "pipe")
        {
            if (pair.size() == 1)
                stream = new pipe_stream(pair[0].c_str());
            else
            if (pair.size() == 2)
                stream = new pipe_stream(pair[1].c_str(), pair[0].c_str());
        }

        return std::shared_ptr<class stream>(stream);
    }
    else
    if (protocol == "tcp" || protocol == "udp")
    {
        stralgo::strings ipport = stralgo::split(pair[1], ':', 1);
        if (ipport.size() != 2)
            return nullptr;

        std::string ip = ipport[0];
        unsigned short port = strtoul(ipport[1].c_str(), nullptr, 10);

        if (protocol == "tcp")
            return std::shared_ptr<stream>(new tcp_stream(ip.c_str(), port));
        else
        {
            udp_stream* udp_stream = new class udp_stream;
            struct udp_stream::endpoint endpoint = {
                .ip   = ip,
                .port = port,
            };

            udp_stream->endpoint(endpoint);

            return std::shared_ptr<stream>(udp_stream);
        }
    }
    else
    if (protocol == "ftp")
    {
        stralgo::strings ap = stralgo::split(pair[1], '/', 1);
        if (ap.size() != 2)
            return nullptr;

        stralgo::strings up_hp = stralgo::split(ap[0], '@');
        std::string up, hp;
        if (up_hp.size() == 1)
            hp = up_hp[0];
        else
        if (up_hp.size() == 2)
            up = up_hp[0], hp = up_hp[1];
        else
            return nullptr;

        stralgo::strings host_port = stralgo::split(hp, ':');
        std::string host;
        unsigned short port;
        if (host_port.size() == 1)
            host = host_port[0], port = 21;
        else
        if (host_port.size() == 2)
            host = host_port[0], port = atol(host_port[1].c_str());
        else
            return nullptr;

        ftp_client ftp(host.c_str(), port);
        if (!up.empty())
        {
            stralgo::strings user_password = stralgo::split(up, ':');

            if (user_password.size() != 2
                || ftp.login(user_password[0].c_str(),
                             user_password[1].c_str()).status[0] != '2')
                return nullptr;
        }

        std::shared_ptr<stream> shared_ptr(new buffer_stream);
        if (ftp.get(ap[1].c_str(), *shared_ptr).status[0] != '2')
            return nullptr;

        return shared_ptr;
    }
    else
    if (protocol == "http")
        return std::shared_ptr<stream>(new http_stream(pair[1].c_str()));
    else
        return nullptr;
}

/////////////////////////////////////////
//
//  ### Seek Wrapper
//
/////////////////////////////////////////

class seek_stream::implement
{
    friend class seek_stream;

public:
    implement(stream& stream)
        : stream_(stream)
    {}

private:
    stream& stream_;
    buffer_stream history_;
};

seek_stream::seek_stream(stream& stream)
    : impl_(new implement(stream))
{}

seek_stream::~seek_stream(void)
{
    delete impl_;
}

seek_stream::seek_stream(const seek_stream& ss)
{
    operator=(ss);
}

seek_stream& seek_stream::operator=(const seek_stream&)
{
    throw std::logic_error("class seek_stream is noncopyable class.");
}

bool seek_stream::seek(long offset, SeekType seek_type)
{
    switch (seek_type)
    {
    case stBegin:
    case stCurrent:
        {
            long oldtell = impl_->history_.tell();
            impl_->history_.seek(offset, seek_type);
            long newtell = impl_->history_.tell();
            long dsttell = offset + (seek_type == stCurrent ? oldtell : 0);

            if (newtell < dsttell)
            {
                stretchy_buffer<char> skip_data(dsttell - newtell);
                read(skip_data.data(), skip_data.size());
            }

            return true;
        }
    case stEnd:
        {
            impl_->history_.plunder(impl_->stream_);
            impl_->history_.seek(offset, seek_type);

            return true;
        }
    default:
        return false;
    }
}

long seek_stream::tell(void) const
{
    return impl_->history_.tell();
}

bool seek_stream::readable(void) const
{
    return impl_->history_.readable() || impl_->stream_.readable();
}

bool seek_stream::writable(void) const
{
    return impl_->stream_.writable();
}

unsigned long seek_stream::read(void* buffer, unsigned long size)
{
    unsigned char* buffer_uc = reinterpret_cast<unsigned char*>(buffer);
    unsigned long  hl = impl_->history_.read(buffer_uc, size);
    unsigned long  rl = 0;

    if (hl < size)
    {
        rl = impl_->stream_.read(buffer_uc + hl, size - hl);

        if (rl > 0)
        {
            impl_->history_.write(buffer_uc + hl, rl);
            impl_->history_.seek(0, stEnd);
        }
    }

    return hl + rl;
}

unsigned long seek_stream::write(const void* buffer, unsigned long size)
{
    return impl_->stream_.write(buffer, size);
}

/////////////////////////////////////////
//
//  ### Buffer Stream
//
/////////////////////////////////////////

class buffer_stream::implement
{
    friend class buffer_stream;

private:
    implement(void)
        : tell_(0)
    {}

private:
    unsigned long tell_;
    stretchy_buffer<unsigned char> buffer_;
};

buffer_stream::buffer_stream(void)
    : impl_(new implement)
{}

buffer_stream::buffer_stream(const void* buffer, unsigned long size)
    : buffer_stream()
{
    write(buffer, size);
}

buffer_stream::buffer_stream(const stretchy_buffer<unsigned char>& buffer)
    : buffer_stream()
{
    impl_->buffer_ = buffer;
}

buffer_stream::buffer_stream(const buffer_stream& bs)
    : buffer_stream()
{
    operator=(bs);
}

buffer_stream::~buffer_stream(void)
{
    delete impl_;
}

buffer_stream& buffer_stream::operator=(const buffer_stream& bs)
{
    if (this != &bs)
    {
        impl_->tell_   = bs.impl_->tell_;
        impl_->buffer_ = bs.impl_->buffer_.clone();
    }

    return *this;
}

bool buffer_stream::seek(long offset, SeekType seek_type)
{
    long tell = -1;

    switch (seek_type)
    {
    case stBegin:   tell = offset; break;
    case stCurrent: tell = impl_->tell_ + offset; break;
    case stEnd:     tell = impl_->buffer_.size() + offset; break;
    }

    if (tell < 0)
        tell = 0;
    else
    if (tell > static_cast<long>(impl_->buffer_.size()))
        tell = static_cast<long>(impl_->buffer_.size());

    impl_->tell_ = tell;

    return true;
}

long buffer_stream::tell(void) const
{
    return impl_->tell_;
}

bool buffer_stream::readable(void) const
{
    return impl_->tell_ < impl_->buffer_.size();
}

bool buffer_stream::writable(void) const
{
    return true;
}

unsigned long buffer_stream::read(void* buffer, unsigned long size)
{
    if (size > impl_->buffer_.size() - impl_->tell_)
        size = impl_->buffer_.size() - impl_->tell_;

    unsigned char* buffer_uc = reinterpret_cast<unsigned char*>(buffer);

    for (unsigned long i = 0; i < size; ++i)
        buffer_uc[i] = impl_->buffer_[impl_->tell_ + i];

    impl_->tell_ += size;

    return size;
}

unsigned long buffer_stream::write(const void* buffer, unsigned long size)
{
    impl_->buffer_.append(reinterpret_cast<const unsigned char*>(buffer), size);

    return size;
}

/////////////////////////////////////////
//
//  ### File Stream
//
/////////////////////////////////////////

class file_stream::implement
{
    friend class file_stream;

public:
    implement(const file& file)
        : file_(file)
    {}

private:
    file file_;
};

file_stream::file_stream(const char* filepath)
    : file_stream(filepath, "rb")
{}

file_stream::file_stream(const char* filepath, const char* mode)
    : file_stream(file(filepath, mode))
{}

file_stream::file_stream(const file& file)
    : impl_(new implement(file))
{}

file_stream::file_stream(const file_stream& fs)
    : file_stream(fs.impl_->file_)
{}

file_stream::~file_stream(void)
{
    delete impl_;
}

file_stream& file_stream::operator=(const file_stream& fs)
{
    if (this != &fs)
        impl_->file_ = fs.impl_->file_;

    return *this;
}

void file_stream::close(void)
{
    impl_->file_ = nullptr;
}

bool file_stream::seek(long offset, SeekType seek_type)
{
    switch (seek_type)
    {
    case stBegin:   return fseek(impl_->file_, offset, SEEK_SET) == 0;
    case stCurrent: return fseek(impl_->file_, offset, SEEK_CUR) == 0;
    case stEnd:     return fseek(impl_->file_, offset, SEEK_END) == 0;
    default:        return false;
    }
}

long file_stream::tell(void) const
{
    return ftell(impl_->file_);
}

bool file_stream::readable(void) const
{
    return impl_->file_ && !feof(impl_->file_);
}

bool file_stream::writable(void) const
{
    return impl_->file_;
}

unsigned long file_stream::read(void* buffer, unsigned long size)
{
    return fread(buffer, 1, size, impl_->file_);
}

unsigned long file_stream::write(const void* buffer, unsigned long size)
{
    return fwrite(buffer, 1, size, impl_->file_);
}

/////////////////////////////////////////
//
//  ### Pipe Stream
//
/////////////////////////////////////////

class pipe_stream::implement
{
    friend class pipe_stream;

public:
    implement(FILE* pipe)
        : pipe_(pipe)
    {}

    ~implement(void)
    {
        if (pipe_)
            pclose(pipe_);
    }

private:
    FILE* pipe_;
};

pipe_stream::pipe_stream(const char* command)
    : pipe_stream(command, "rb")
{}

pipe_stream::pipe_stream(const char* command, const char* mode)
    : impl_(new implement(popen(command, mode)))
{}

pipe_stream::pipe_stream(const pipe_stream& ps)
    : impl_(new implement(nullptr))
{
    operator=(ps);
}

pipe_stream::~pipe_stream(void)
{
    delete impl_;
}

pipe_stream& pipe_stream::operator=(const pipe_stream& ps)
{
    if (this != &ps)
    {
        impl_->pipe_    = ps.impl_->pipe_;
        ps.impl_->pipe_ = nullptr;
    }

    return *this;
}

void pipe_stream::close(void)
{
    pclose(impl_->pipe_);
    impl_->pipe_ = nullptr;
}

bool pipe_stream::seek(long, SeekType)
{
    return false;
}

long pipe_stream::tell(void) const
{
    return -1;
}

bool pipe_stream::readable(void) const
{
    return impl_->pipe_ && !feof(impl_->pipe_);
}

bool pipe_stream::writable(void) const
{
    return impl_->pipe_;
}

unsigned long pipe_stream::read(void* buffer, unsigned long size)
{
    return fread(buffer, 1, size, impl_->pipe_);
}

unsigned long pipe_stream::write(const void* buffer, unsigned long size)
{
    return fwrite(buffer, 1, size, impl_->pipe_);
}

#ifndef __os_unix__

#include <Winsock2.h>

static struct WSAInitializer
{
    WSAInitializer(void) { WSAStartup(MAKEWORD(2, 2), new WSADATA); }
   ~WSAInitializer(void) { WSACleanup(); }
} __wsa_initializer__;

#else

#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define closesocket ::close

#endif

/////////////////////////////////////////
//
//  ### TCP Stream
//
/////////////////////////////////////////

class tcp_stream::implement
{
    friend class tcp_stream;

public:
    implement(void) : fd_(-1) {}
   ~implement(void) { if (fd_ != -1) closesocket(fd_); }

private:
    int fd_;
};

tcp_stream::tcp_stream(int fd)
    : impl_(new implement)
{
    impl_->fd_ = fd;
}

tcp_stream::tcp_stream(const char* host, unsigned short port)
    : impl_(new implement)
{
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1)
        return;

    struct hostent* hostent = gethostbyname(host);
    if (!hostent)
        return;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = *(unsigned long*)(hostent->h_addr);
    addr.sin_port = htons(port);

    if (connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0)
    {
        closesocket(fd);
        return;
    }

    impl_->fd_ = fd;
}

tcp_stream::tcp_stream(const tcp_stream& ts)
    : impl_(new implement)
{
    operator=(ts);
}

tcp_stream::~tcp_stream(void)
{
    delete impl_;
}

tcp_stream& tcp_stream::operator=(const tcp_stream& ts)
{
    if (this != &ts)
    {
        if (impl_->fd_ != -1)
            closesocket(impl_->fd_);

        impl_->fd_    = ts.impl_->fd_;
        ts.impl_->fd_ = -1;
    }

    return *this;
}

void tcp_stream::close(void)
{
    closesocket(impl_->fd_);
    impl_->fd_ = -1;
}

bool tcp_stream::seek(long offset, SeekType seek_type)
{
    return false;
}

long tcp_stream::tell(void) const
{
    return -1;
}

bool tcp_stream::readable(void) const
{
    return impl_->fd_ != -1;
}

bool tcp_stream::writable(void) const
{
    return impl_->fd_ != -1;
}

unsigned long tcp_stream::read(void* buffer, unsigned long size)
{
    unsigned long read_length = 0;

    while (impl_->fd_ != -1 && read_length < size)
    {
        int l = recv(impl_->fd_,
                     reinterpret_cast<char*>(buffer) + read_length,
                     size - read_length, 0);

        if (l == -1)
            impl_->fd_ = -1;
        else
        if (l == 0)
            closesocket(impl_->fd_), impl_->fd_ = -1;
        else
            read_length += l;
    }

    return read_length;
}

unsigned long tcp_stream::write(const void* buffer, unsigned long size)
{
    unsigned long written_length = 0;

    while (impl_->fd_ != -1 && written_length < size)
    {
        int l = send(impl_->fd_,
                     reinterpret_cast<const char*>(buffer) + written_length,
                     size - written_length, 0);

        if (l == -1)
            impl_->fd_ = -1;
        else
            written_length += l;
    }

    return written_length;
}

/////////////////////////////////////////
//
//  ### UDP Stream
//
/////////////////////////////////////////

class udp_stream::implement
{
    friend class udp_stream;

public:
    implement(void)
        : fd_(socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
    {}

    ~implement(void)
    {
        if (fd_ != -1)
            closesocket(fd_);
    }

private:
    int fd_;
    struct sockaddr_in addr_, raddr_;
};

udp_stream::udp_stream(void)
    : impl_(new implement)
{}

udp_stream::udp_stream(const udp_stream& us)
    : udp_stream()
{
    operator=(us);
}

udp_stream::~udp_stream(void)
{
    delete impl_;
}

udp_stream& udp_stream::operator=(const udp_stream& us)
{
    if (this != &us)
        impl_->addr_ = us.impl_->addr_;

    return *this;
}

bool udp_stream::bind(const char* ip, unsigned short port)
{
    if (impl_->fd_ == -1)
        return false;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    return 0 == ::bind(impl_->fd_,
                       reinterpret_cast<struct sockaddr*>(&addr),
                       sizeof(struct sockaddr));
}

void udp_stream::endpoint(struct endpoint& endpoint)
{
    struct hostent* hostent = gethostbyname(endpoint.ip.c_str());

    if (hostent)
    {
        impl_->addr_.sin_family = AF_INET;
        impl_->addr_.sin_addr.s_addr = *(unsigned long*)(hostent->h_addr);
        impl_->addr_.sin_port = htons(endpoint.port);
    }
}

struct udp_stream::endpoint udp_stream::endpoint(void) const
{
    typedef struct endpoint endpoint;

    return endpoint {
        .ip   = inet_ntoa(impl_->addr_.sin_addr),
        .port = ntohs(impl_->addr_.sin_port)
    };
}

struct udp_stream::endpoint udp_stream::read_endpoint(void) const
{
    typedef struct endpoint endpoint;

    return endpoint {
        .ip   = inet_ntoa(impl_->raddr_.sin_addr),
        .port = ntohs(impl_->raddr_.sin_port)
    };
}

bool udp_stream::seek(long, SeekType)
{
    return false;
}

long udp_stream::tell(void) const
{
    return -1;
}

bool udp_stream::readable(void) const
{
    return impl_->fd_ != -1;
}

bool udp_stream::writable(void) const
{
    return impl_->fd_ != -1;
}

unsigned long udp_stream::read(void* buffer, unsigned long size)
{
    int addrlen = sizeof(impl_->raddr_);

    return recvfrom(impl_->fd_,
                    reinterpret_cast<char*>(buffer), size, 0,
                    reinterpret_cast<struct sockaddr*>(&impl_->raddr_),
                    &addrlen);
}

unsigned long udp_stream::write(const void* buffer, unsigned long size)
{
    unsigned long written_length = 0;

    while (impl_->fd_ != -1 && written_length < size)
    {
        int l = sendto(impl_->fd_,
                       reinterpret_cast<const char*>(buffer) + written_length,
                       size - written_length, 0,
                       reinterpret_cast<struct sockaddr*>(&impl_->addr_),
                       sizeof(struct sockaddr));

        if (l == -1)
            impl_->fd_ = -1;
        else
            written_length += l;
    }

    return written_length;
}

/////////////////////////////////////////
//
//  ### HTTP Stream
//
/////////////////////////////////////////

class http_stream::implement
{
    friend class http_stream;

public:
    implement(std::string url, std::string method, http_stream::dict headers,
              const std::string& data)
        : tcp_stream_(nullptr), content_length_(-1), tell_(0)
    {
        // 去除 http:// 前缀
        std::string prefix = "http://";
        if (url.size() >= prefix.size()
            && prefix == stralgo::lowerc(url.substr(0, prefix.size())))
            url = url.substr(prefix.size());
        if (url.empty())
            return;

        // 分隔服务器地址和url路径
        std::string ap, path = "/";
        std::string::size_type sep1_pos = url.find('/');
        if (sep1_pos != std::string::npos)
            ap = url.substr(0, sep1_pos), path = url.substr(sep1_pos);
        else
            ap = url;
        if (ap.empty())
            return;

        // 分隔服务器域名和端口
        std::string host;
        unsigned short port = 80;
        std::string::size_type sep2_pos = ap.find(':');
        if (sep2_pos != std::string::npos)
        {
            host = ap.substr(0, sep2_pos);
            port = atoi(ap.substr(sep2_pos + 1).c_str());
        }
        else
            host = ap;
        if (host.empty())
            return;

        // 生成请求消息
        if (method.empty())
            method = "GET";
        set_if_not_exist(headers, "Host",       host);
        set_if_not_exist(headers, "Accept",     "*/*");
        set_if_not_exist(headers, "User-Agent", "takisy.stream.http");
        set_if_not_exist(headers, "Connection", "Keep-Alive");
        std::string reqdata =
            stralgo::format("%s\n%s\n%s",
                            stralgo::format("%s %s HTTP/1.1",
                                            method.c_str(),
                                            path.c_str()).c_str(),
                            headers2string(headers).c_str(),
                            data.c_str());

        // 进行请求并解析响应消息
        request(host, port, reqdata);
    }

    implement(const std::string& host, unsigned short port,
              const std::string& reqdata)
    {
        request(host, port, reqdata);
    }

    ~implement(void)
    {
        if (tcp_stream_)
            delete tcp_stream_;
    }

private:
    static std::string headers2string(const dict& headers)
    {
        std::string str;

        for (const dict::value_type& pair : headers)
            str += stralgo::format("%s: %s\n",
                                   pair.first.c_str(),
                                   pair.second.c_str());

        return str;
    }

    static inline void set_if_not_exist(dict& headers,
                                        const std::string& key,
                                        const std::string& value)
    {
        if (headers.find(key) == headers.end())
            headers[key] = value;
    }

    bool request(const std::string& host ,unsigned short port,
                 const std::string& request)
    {
        // 清除
        if (tcp_stream_)
            delete tcp_stream_;
        headers_.clear();
        content_length_ = -1;
        tell_ = 0;

        // 建立到服务器的连接
        tcp_stream_ = new tcp_stream(host.c_str(), port);
        if (   !tcp_stream_
            || !tcp_stream_->readable()
            || !tcp_stream_->writable())
            return false;
        else
            host_ = host, port_ = port;

        // 发送请求消息到服务器
        register unsigned long length = request.size();
        if (tcp_stream_->write(request.c_str(), length) != length)
        {
            delete tcp_stream_;
            tcp_stream_ = nullptr;
            return false;
        }
        else
            request_ = request;

        // 读取HTTP响应消息并解析响应状态码
        status_ = stralgo::split(tcp_stream_->read_line(), ' ', 2);
        if (status_.size() == 0) status_.push_back("HTTP/1.0");
        if (status_.size() == 1) status_.push_back("0");
        if (status_.size() == 2) status_.push_back("");

        // 解析响应消息头
        while (true)
        {
            std::string line = tcp_stream_->read_line();
            if (line.empty())
                break;

            std::string::size_type seq_pos = line.find(":");
            if (seq_pos != std::string::npos)
            {
                std::string key, value;

                key   = stralgo::trimc(line.substr(0, seq_pos));
                value = stralgo::trimc(line.substr(++seq_pos));

                headers_[stralgo::lower(key)] = value;
            }
        }

        // 获取http正文长度
        if (headers_.find("content-length") != headers_.end())
            content_length_ = atoll(headers_["content-length"].c_str());

        return true;
    }

private:
    std::string       host_;
    unsigned short    port_;
    std::string       request_;
    tcp_stream*       tcp_stream_;
    stralgo::strings  status_;
    http_stream::dict headers_;
    long              content_length_, tell_;
};

http_stream::http_stream(const char* url)
    : http_stream(url, "")
{}

http_stream::http_stream(const char* url, const char* method)
    : http_stream(url, method, "")
{}

http_stream::http_stream(const char* url, const dict& headers)
    : http_stream(url, "", headers)
{}

http_stream::http_stream(const char* url, const char* method, const char* data)
    : http_stream(url, method, dict(), data)
{}

http_stream::http_stream(const char* url, const char* method,
                         const dict& headers)
    : http_stream(url, method, headers, "")
{}

http_stream::http_stream(const char* url, const char* method,
                         const dict& headers, const char* data)
    : impl_(new implement(url, method, headers, data))
{}

http_stream::http_stream(const http_stream& hs)
    : impl_(new implement(hs.impl_->host_, hs.impl_->port_, hs.impl_->request_))
{}

http_stream::~http_stream(void)
{
    delete impl_;
}

http_stream& http_stream::operator=(const http_stream& hs)
{
    if (this != &hs)
        impl_->request(hs.impl_->host_, hs.impl_->port_, hs.impl_->request_);

    return *this;
}

int http_stream::status_code(void) const
{
    return atoi(impl_->status_[1].c_str());
}

const char* http_stream::status_description(void) const
{
    return impl_->status_[2].c_str();
}

const char* http_stream::header(const char* key) const
{
    std::string lower_key = stralgo::lowerc(key);

    if (impl_->headers_.find(lower_key) == impl_->headers_.end())
        return nullptr;

    return impl_->headers_[lower_key].c_str();
}

bool http_stream::readable(void) const
{
    if (impl_->content_length_ != -1)
        return impl_->tcp_stream_
            && impl_->tcp_stream_->readable()
            && impl_->tell_ < impl_->content_length_;
    else
        return impl_->tcp_stream_ && impl_->tcp_stream_->readable();
}

bool http_stream::writable(void) const
{
    return false;
}

bool http_stream::seek(long offset, SeekType seek_type)
{
    return false;
}

long http_stream::tell(void) const
{
    return impl_->tell_;
}

unsigned long http_stream::read(void* buffer, unsigned long size)
{
    if (!readable())
        return 0;

    if (impl_->content_length_ != -1)
    {
        unsigned long long left_length = impl_->content_length_ - impl_->tell_;

        if (left_length < size)
            size = left_length;
    }

    unsigned long length = 0;

    if (size > 0)
    {
        length = impl_->tcp_stream_->read(buffer, size);
        impl_->tell_ += length;
    }

    return length;
}

unsigned long http_stream::write(const void* buffer, unsigned long size)
{
    return 0;
}

std::string http_stream::encode_query(const dict& query)
{
    if (query.empty())
        return "";

    stralgo::strings strings;
    for (const dict::value_type& pair : query)
        strings.push_back(stralgo::format("%s=%s",
                                          pair.first.c_str(),
                                          pair.second.c_str()));

    std::string string = strings[0];
    for (unsigned long i = 1; i < strings.size(); ++i)
        string += "&" + strings[i];

    return string;
}

http_stream::dict http_stream::decode_query(const std::string& query)
{
    stralgo::strings strings = stralgo::split(query, '&');
    dict result;

    for (const stralgo::strings::value_type& value : strings)
    {
        stralgo::strings pair = stralgo::split(value, '=');
        if (pair.size() == 2)
            result[pair[0]] = pair[1];
    }

    return result;
}
