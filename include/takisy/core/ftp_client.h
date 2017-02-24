#ifndef ftp_client_h_20150826
#define ftp_client_h_20150826

#include <string>
#include <takisy/algorithm/stralgo.h>
#include <takisy/core/stream.h>

class ftp_client
{
    class implement;

public:
    struct response
    {
        char status[4] = {0};
        std::string detail;

    public:
        operator bool(void) const;
    };

public:
    ftp_client(const char* host);
    ftp_client(const char* host, unsigned short port);
    ftp_client(const ftp_client& fc);
   ~ftp_client(void);

    ftp_client& operator=(const ftp_client& fc);

public:
    void debug(bool debug);

public:
    response sendcmd(const char* cmd) const;
    template <typename... Args> inline
    response sendcmd(const char* cmdformat, const Args&... args) const;
    response login(void) const;
    response login(const char* user, const char* password) const;
    response quit(void) const;
    response noop(void) const;
    response mkdir(const char* path) const;
    response rmdir(const char* path) const;
    response pwd(void) const;
    response list(const char* path, std::string& content) const;
    response list(const char* path, stream& ostream) const;
    response cd(const char* path) const;
    response cdup(void) const;
    response type(char type) const;
    response ascii(void) const;
    response binary(void) const;
    response size(const char* remotefile) const;
    response put(const char* localfile) const;
    response put(const char* localfile, const char* remotefile) const;
    response put(const stream& istream, const char* remotefile) const;
    response del(const char* remotefile) const;
    response get(const char* remotefile) const;
    response get(const char* remotefile, const char* localfile) const;
    response get(const char* remotefile, stream& ostream) const;
    response rename(const char* srcname, const char* dstname) const;
    response abort(void) const;

private:
    class implement* impl_;
};

template <typename... Args>
ftp_client::response
    ftp_client::sendcmd(const char* cmdformat, const Args&... args) const
{
    return sendcmd(stralgo::format(cmdformat, args...).c_str());
}

#endif // ftp_client_h_20150826
