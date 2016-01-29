#include <stdexcept>
#include <takisy/algorithm/stralgo.h>
#include <takisy/core/stream.h>

unsigned long stream::plunder(const char* uri)
{
    return plunder(*from_uri(uri));
}

unsigned long stream::plunder(const stream& src)
{
    stretchy_buffer<unsigned char> buffer(512 * 1024);
    unsigned long length = 0, readed_length = 0;

    while ((readed_length = src.read(buffer.data(), buffer.size())))
    {
        length += readed_length;
        write(buffer.data(), readed_length);
    }

    return length;
}

std::shared_ptr<stream> stream::from_uri(const char* uri)
{
    stralgo::strings parts = stralgo::split(uri, "://", 1);
    if (parts.size() != 2)
        return nullptr;

    std::string protocol = stralgo::lower(parts[0]);

    if (protocol == "buffer")
        return std::shared_ptr<stream>
                  (new buffer_stream(parts[1].c_str(), parts[1].size()));
    else
    if (protocol == "file")
        return std::shared_ptr<stream>(new file_stream(parts[1].c_str()));
    else
    if (protocol == "pipe")
        return std::shared_ptr<stream>(new pipe_stream(parts[1].c_str()));
    else
    if (protocol == "tcp" || protocol == "udp")
    {
        stralgo::strings ipport = stralgo::split(parts[1], ':', 1);
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
    if (protocol == "http")
        return std::shared_ptr<stream>(new http_stream(parts[1].c_str()));
    else
        return nullptr;
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
    mutable unsigned int tell_;
    stretchy_buffer<unsigned char> buffer_;
};

buffer_stream::buffer_stream(void)
    : impl_(new implement)
{}

buffer_stream::buffer_stream(const void* buffer, unsigned int size)
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

bool buffer_stream::working(void) const
{
    return impl_->tell_ < impl_->buffer_.size();
}

bool buffer_stream::seek(long offset, SeekType seek_type) const
{
    int tell = -1;

    switch (seek_type)
    {
    case stBegin:   tell = offset; break;
    case stCurrent: tell = impl_->tell_ + offset; break;
    case stEnd:     tell = impl_->buffer_.size() + offset; break;
    }

    if (0 <= tell && tell <= static_cast<int>(impl_->buffer_.size()))
        return impl_->tell_ = tell, true;
    else
        return false;
}

long buffer_stream::tell(void) const
{
    return impl_->tell_;
}

unsigned int buffer_stream::read(void* buffer, unsigned int size) const
{
    if (size > impl_->buffer_.size() - impl_->tell_)
        size = impl_->buffer_.size() - impl_->tell_;

    unsigned char* buffer_uc = reinterpret_cast<unsigned char*>(buffer);

    for (unsigned int i = 0; i < size; ++i)
        buffer_uc[i] = impl_->buffer_[impl_->tell_++];

    return size;
}

unsigned int buffer_stream::write(const void* buffer, unsigned int size)
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

file_stream::file_stream(const char* file_path)
    : file_stream(file_path, "rb")
{}

file_stream::file_stream(const char* file_path, const char* mode)
    : file_stream(file(file_path, mode))
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

bool file_stream::working(void) const
{
    return impl_->file_ && !feof(impl_->file_);
}

bool file_stream::seek(long offset, SeekType seek_type) const
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

unsigned int file_stream::read(void* buffer, unsigned int size) const
{
    return fread(buffer, 1, size, impl_->file_);
}

unsigned int file_stream::write(const void* buffer, unsigned int size)
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

bool pipe_stream::working(void) const
{
    return impl_->pipe_ && !feof(impl_->pipe_);
}

unsigned int pipe_stream::read(void* buffer, unsigned int size) const
{
    return fread(buffer, 1, size, impl_->pipe_);
}

unsigned int pipe_stream::write(const void* buffer, unsigned int size)
{
    return fwrite(buffer, 1, size, impl_->pipe_);
}

bool pipe_stream::seek(long, SeekType) const
{
    throw std::runtime_error("pipe_stream::seek is unused member function.");
}

long pipe_stream::tell(void) const
{
    throw std::runtime_error("pipe_stream::tell is unused member function.");
}

/// include socket headers

#ifdef WINNT

#include <winsock2.h>

static struct WSAFactory
{
    WSAFactory(void) { WSAStartup(MAKEWORD(2, 2), new WSADATA); }
   ~WSAFactory(void) { WSACleanup(); }
} __wsaFactory;

#else

#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define closesocket close

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

bool tcp_stream::working(void) const
{
    return impl_->fd_ != -1;
}

unsigned int tcp_stream::read(void* buffer, unsigned int size) const
{
    unsigned int readed_length = 0;

    while (impl_->fd_ != -1 && readed_length < size)
    {
        int l = recv(impl_->fd_,
                     reinterpret_cast<char*>(buffer) + readed_length,
                     size - readed_length, 0);

        if (l == -1)
            impl_->fd_ = -1;
        else
        if (l == 0)
            closesocket(impl_->fd_), impl_->fd_ = -1;
        else
            readed_length += l;
    }

    return readed_length;
}

unsigned int tcp_stream::write(const void* buffer, unsigned int size)
{
    unsigned int written_length = 0;

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

bool tcp_stream::seek(long offset, SeekType seek_type) const
{
    throw std::runtime_error("tcp_stream::seek is unused member function.");
}

long tcp_stream::tell(void) const
{
    throw std::runtime_error("tcp_stream::tell is unused member function.");
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

bool udp_stream::working(void) const
{
    return impl_->fd_ != -1;
}

unsigned int udp_stream::read(void* buffer, unsigned int size) const
{
    int addrlen = sizeof(impl_->raddr_);

    return recvfrom(impl_->fd_,
                    reinterpret_cast<char*>(buffer), size, 0,
                    reinterpret_cast<struct sockaddr*>(&impl_->raddr_),
                    &addrlen);
}

unsigned int udp_stream::write(const void* buffer, unsigned int size)
{
    unsigned int written_length = 0;

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

bool udp_stream::seek(long, SeekType) const
{
    throw std::runtime_error("udp_stream::seek is unused member function.");
}

long udp_stream::tell(void) const
{
    throw std::runtime_error("udp_stream::tell is unused member function.");
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
        : tcp_stream_(nullptr), tell_(0)
    {
        // 去除 http:// 前缀
        std::string prefix = "http://";
        if (url.size() >= prefix.size()
            && prefix == stralgo::lowerc(url.substr(0, prefix.size())))
            url = url.substr(prefix.size());
        if (url.empty())
            return;

        // 分隔服务器地址和url路径
        std::string address, path = "/";
        std::string::size_type sep1_pos = url.find('/');
        if (sep1_pos != std::string::npos)
            address = url.substr(0, sep1_pos), path = url.substr(sep1_pos);
        else
            address = url;
        if (address.empty())
            return;

        // 分隔服务器域名和端口
        std::string host;
        unsigned short port = 80;
        std::string::size_type sep2_pos = address.find(':');
        if (sep2_pos != std::string::npos)
        {
            host = address.substr(0, sep2_pos);
            port = atoi(address.substr(sep2_pos + 1).c_str());
        }
        else
            host = address;
        if (host.empty())
            return;

        // 生成请求消息
        if (method.empty())
            method = "GET";
        set_if_not_exist(headers, "Host",       host);
        set_if_not_exist(headers, "Accept",     "*/*");
        set_if_not_exist(headers, "User-Agent", "stream.http_stream");
        set_if_not_exist(headers, "Connection", "Keep-Alive");
        std::string request_message =
            stralgo::format("%s\n%s\n%s",
               stralgo::format("%s %s HTTP/1.0", method.c_str(), path.c_str()),
               headers2string(headers).c_str(),
               data.c_str());

        // 进行请求并解析响应消息
        request(host, port, request_message);
    }

    implement(const std::string& host, unsigned short port,
              const std::string& request_message)
    {
        request(host, port, request_message);
    }

    ~implement(void)
    {
        if (tcp_stream_)
            delete tcp_stream_;
    }

private:
    static std::string readline(tcp_stream* ts)
    {
        std::string line;

        while (true)
        {
            char ch;
            if (ts->read(&ch, 1) != 1) return "";
            if (ch == '\r') continue;
            if (ch == '\n') break;
            line += ch;
        }

        return line;
    }

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
        tell_ = 0;

        // 建立到服务器的连接
        tcp_stream_ = new tcp_stream(host.c_str(), port);
        if (!tcp_stream_ || !tcp_stream_->working())
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
        status_ = stralgo::split(readline(tcp_stream_), ' ', 2);
        if (status_.size() == 0) status_.push_back("HTTP/1.0");
        if (status_.size() == 1) status_.push_back("0");
        if (status_.size() == 2) status_.push_back("");

        // 解析响应消息头
        while (true)
        {
            std::string line = readline(tcp_stream_);
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

        return true;
    }

private:
    std::string        host_;
    unsigned short     port_;
    std::string        request_;
    tcp_stream*        tcp_stream_;
    stralgo::strings   status_;
    http_stream::dict  headers_;
    unsigned long long tell_;
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

bool http_stream::working(void) const
{
    return impl_->tcp_stream_ && impl_->tcp_stream_->working();
}

long http_stream::tell(void) const
{
    return impl_->tell_;
}

unsigned int http_stream::read(void* buffer, unsigned int size) const
{
    if (!working())
        return 0;

    const char* content_length_s = header("content-length");
    if (content_length_s)
    {
        unsigned long long content_length = atoi(content_length_s);
        unsigned long long left_length = content_length - impl_->tell_;
        if (left_length < size)
            size = left_length;
    }

    unsigned int length = 0;

    if (size > 0)
    {
        length = impl_->tcp_stream_->read(buffer, size);
        impl_->tell_ += length;
    }

    return length;
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
    for (unsigned int i = 1; i < strings.size(); ++i)
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

bool http_stream::seek(long offset, SeekType seek_type) const
{
    throw std::runtime_error("http_stream::seek is unused member function.");
}

unsigned int http_stream::write(const void* buffer, unsigned int size)
{
    throw std::runtime_error("http_stream::write is unused member function.");
}
