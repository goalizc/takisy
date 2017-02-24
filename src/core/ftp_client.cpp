#include <iostream>
#include <takisy/core/os.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/core/ftp_client.h>

class ftp_client::implement
{
    friend class ftp_client;

public:
    implement(const char* host, unsigned short port)
        : debug_(false), tcp_(host, port)
    {
        response rsp = get_response();
        if (rsp.status[0] != '2')
            tcp_.close();
    }

public:
    inline void print(const std::string& message) const
    {
        std::cout << message << std::endl;
    }

    response get_response(void)
    {
        response rsp;

        std::string line = tcp_.read_line();
        if (line.size() < 4)
            return rsp;

        std::string status = line.substr(0, 3);
        if (line[3] != ' ')
        {
            if (line[3] != '-')
                return rsp;

            while (true)
            {
                line = tcp_.read_line();
                if (!tcp_.readable())
                    return rsp;

                if (status == line.substr(0, 3) && line[3] == ' ')
                    break;
            }
        }

        rsp.status[0] = status[0];
        rsp.status[1] = status[1];
        rsp.status[2] = status[2];
        rsp.detail    = line.substr(4);

        return rsp;
    }

    inline response sendcmd(std::string cmd)
    {
        if (debug_)
            print(cmd);

        cmd += "\r\n";
        if (tcp_.write(cmd.data(), cmd.size()) != cmd.size())
            return response();

        return get_response();
    }

    tcp_stream create_tcp_stream_from_pasv(void)
    {
        response rsp = sendcmd("PASV");
        if (rsp.status[0] != '2')
            return tcp_stream(-1);

        unsigned int u8[6] = {0};
        if (sscanf(rsp.detail.c_str(), "%*[^(](%d,%d,%d,%d,%d,%d)",
                   &u8[0], &u8[1], &u8[2], &u8[3], &u8[4], &u8[5]) != 6)
            return tcp_stream(-1);

        return tcp_stream(
            stralgo::format("$0.$1.$2.$3", u8[0], u8[1], u8[2], u8[3]).c_str(),
            u8[4] << 8 | u8[5]);
    }

private:
    bool debug_;
    tcp_stream tcp_;
};

ftp_client::response::operator bool(void) const
{
    return status[0] == '2';
}

ftp_client::ftp_client(const char* host)
    : ftp_client(host, 21)
{}

ftp_client::ftp_client(const char* host, unsigned short port)
    : impl_(new implement(host, port))
{}

ftp_client::ftp_client(const ftp_client& fc)
    : ftp_client("localhost")
{}

ftp_client::~ftp_client(void)
{
    delete impl_;
}

ftp_client& ftp_client::operator=(const ftp_client& fc)
{
    if (this != &fc)
        impl_->tcp_ = fc.impl_->tcp_;

    return *this;
}

void ftp_client::debug(bool debug)
{
    impl_->debug_ = debug;
}

ftp_client::response ftp_client::sendcmd(const char* cmd) const
{
    return impl_->sendcmd(cmd);
}

ftp_client::response ftp_client::login(void) const
{
    return login("anonymous", "pswd@ftp_client");
}

ftp_client::response
    ftp_client::login(const char* user, const char* password) const
{
    response rsp = sendcmd("USER $0", user);

    if (rsp.status[0] != '3')
        return rsp;

    return sendcmd("PASS $0", password);
}

ftp_client::response ftp_client::quit(void) const
{
    response rsp = sendcmd("QUIT");

    impl_->tcp_.close();

    return rsp;
}

ftp_client::response ftp_client::noop(void) const
{
    return sendcmd("NOOP");
}

ftp_client::response ftp_client::mkdir(const char* path) const
{
    return sendcmd("MKD $0", path);
}

ftp_client::response ftp_client::rmdir(const char* path) const
{
    return sendcmd("RMD $0", path);
}

ftp_client::response ftp_client::pwd(void) const
{
    return sendcmd("PWD");
}

ftp_client::response
    ftp_client::list(const char* path, std::string& content) const
{
    buffer_stream bufstream;

    response rsp = list(path, bufstream);
    if (rsp.status[0] == '2')
    {
        bufstream.seek(0, stream::stEnd);
        content.resize(bufstream.tell());
        bufstream.seek(0, stream::stBegin);
        bufstream.read(const_cast<char*>(content.data()), content.size());
    }

    return rsp;
}

ftp_client::response ftp_client::list(const char* path, stream& ostream) const
{
    response rsp;

    tcp_stream tcp = impl_->create_tcp_stream_from_pasv();
    if (!tcp.readable())
    {
        rsp.status[0] = 0;
        rsp.detail    = "Create list data connection failed.";
        return rsp;
    }

    if (path)
        rsp = sendcmd("LIST $0", path);
    else
        rsp = sendcmd("LIST");

    if (rsp.status[0] != '1')
        return rsp;

    ostream.obtain(tcp);

    return impl_->get_response();
}

ftp_client::response ftp_client::cd(const char* path) const
{
    return sendcmd("CWD $0", path);
}

ftp_client::response ftp_client::cdup(void) const
{
    return sendcmd("CDUP");
}

ftp_client::response ftp_client::type(char type) const
{
    return sendcmd("TYPE $0", type);
}

ftp_client::response ftp_client::ascii(void) const
{
    return type('A');
}

ftp_client::response ftp_client::binary(void) const
{
    return type('I');
}

ftp_client::response ftp_client::size(const char* remotefile) const
{
    return sendcmd("SIZE $0", remotefile);
}

ftp_client::response ftp_client::put(const char* localfile) const
{
    return put(localfile, os::path::split(localfile).second.c_str());
}

ftp_client::response
    ftp_client::put(const char* localfile, const char* remotefile) const
{
    file_stream fs(localfile, "rb");

    return put(fs, remotefile);
}

ftp_client::response
    ftp_client::put(const stream& istream, const char* remotefile) const
{
    response rsp;

    if (!istream.readable())
    {
        rsp.status[0] = 0;
        rsp.detail    = "Invalid input stream.";
        return rsp;
    }

    tcp_stream tcp = impl_->create_tcp_stream_from_pasv();
    if (!tcp.writable())
    {
        rsp.status[0] = 0;
        rsp.detail    = "Create stor data connection failed.";
        return rsp;
    }

    rsp = sendcmd("STOR $0", remotefile);
    if (rsp.status[0] != '1')
        return rsp;

    tcp.obtain(istream);
    tcp.close();

    return impl_->get_response();
}

ftp_client::response ftp_client::del(const char* remotefile) const
{
    return sendcmd("DELE $0", remotefile);
}

ftp_client::response ftp_client::get(const char* remotefile) const
{
    return get(remotefile, os::path::split(remotefile).second.c_str());
}

ftp_client::response
    ftp_client::get(const char* remotefile, const char* localfile) const
{
    file_stream fs(localfile, "wb");

    return get(remotefile, fs);
}

ftp_client::response
    ftp_client::get(const char* remotefile, stream& ostream) const
{
    response rsp;

    if (!ostream.writable())
    {
        rsp.status[0] = 0;
        rsp.detail    = "Invalid output stream.";
        return rsp;
    }

    rsp = size(remotefile);
    if (rsp.status[0] != '2')
        return rsp;

    unsigned long total = stralgo::atol(rsp.detail);

    tcp_stream tcp = impl_->create_tcp_stream_from_pasv();
    if (!tcp.readable())
    {
        rsp.status[0] = 0;
        rsp.detail    = "Create retr data connection failed.";
        return rsp;
    }

    rsp = sendcmd("RETR $0", remotefile);
    if (rsp.status[0] != '1')
        return rsp;

    unsigned long getted = ostream.obtain(tcp);
    if (getted != total)
    {
        rsp.status[0] = 0;
        rsp.detail    = stralgo::format("getted/total: $0/$1", getted, total);
        return rsp;
    }

    return impl_->get_response();
}

ftp_client::response
    ftp_client::rename(const char* srcname, const char* dstname) const
{
    response rsp = sendcmd("RNFR $0", srcname);

    if (rsp.status[0] != '3')
        return rsp;

    return sendcmd("RNTO $0", dstname);
}

ftp_client::response ftp_client::abort(void) const
{
    return sendcmd("ABOR");
}
