#ifndef stream_h_20140410
#define stream_h_20140410

#include <map>
#include <memory>
#include <string>
#include <takisy/core/file.h>
#include <takisy/core/stretchy_buffer.h>

class stream
{
public:
    typedef std::shared_ptr<stream> sptr;
    enum seekdir {
        beg, cur, end
    };

public:
    virtual ~stream(void) {}

public:
    virtual long tell(void) const = 0;
    virtual bool seekable(void) const = 0;
    virtual bool readable(void) const = 0;
    virtual bool writable(void) const = 0;
    virtual bool seek(long offset, seekdir seekdir) const = 0;
    virtual unsigned long read(void* buffer, unsigned long size) const = 0;
    virtual unsigned long write(const void* buffer, unsigned long size) = 0;

public:
    static sptr from_uri(const std::string& uri);

public:
    template <typename T>
    bool read(T& value) const
    {
        return read(&value, sizeof(value)) == sizeof(value);
    }

    template <typename T>
    bool write(const T& value)
    {
        return write(&value, sizeof(value)) == sizeof(value);
    }

    std::string read_all(void) const;
    std::string read_chars(unsigned long nchars) const;
    std::string read_line(void) const;
    std::string read_until(char terminator) const;
    std::string read_until(const std::string& terminator) const;

    unsigned long obtain(const std::string& uri);
    unsigned long obtain(const stream& src);
};

class seek_stream : public stream
{
    class implement;

public:
    seek_stream(const stream& stream);
   ~seek_stream(void);

private:
    seek_stream(const seek_stream&);
    seek_stream& operator=(const seek_stream&);

public:
    long tell(void) const override;
    bool seekable(void) const override;
    bool readable(void) const override;
    bool writable(void) const override;
    bool seek(long, seekdir) const override;
    unsigned long read(void* buffer, unsigned long size) const override;
    unsigned long write(const void*, unsigned long) override;

private:
    class implement* impl_;
};

class buffer_stream : public stream
{
    class implement;

public:
    buffer_stream(void);
    buffer_stream(const void* buffer, unsigned long size);
    buffer_stream(const stretchy_buffer<unsigned char>& buffer);
    buffer_stream(const buffer_stream& bs);
    buffer_stream(buffer_stream&& bs);
   ~buffer_stream(void);

    buffer_stream& operator=(const buffer_stream& bs);
    buffer_stream& operator=(buffer_stream&& bs);

public:
    long tell(void) const override;
    bool seekable(void) const override;
    bool readable(void) const override;
    bool writable(void) const override;
    bool seek(long offset, seekdir seekdir) const override;
    unsigned long read(void* buffer, unsigned long size) const override;
    unsigned long write(const void* buffer, unsigned long size) override;

private:
    class implement* impl_;
};

class file_stream : public stream
{
    class implement;

public:
    file_stream(const char* filepath);
    file_stream(const char* filepath, const char* mode);
    file_stream(const file& file);
    file_stream(const file_stream& fs);
   ~file_stream(void);

    file_stream& operator=(const file_stream& fs);

public:
    void close(void);

public:
    long tell(void) const override;
    bool seekable(void) const override;
    bool readable(void) const override;
    bool writable(void) const override;
    bool seek(long offset, seekdir seekdir) const override;
    unsigned long read(void* buffer, unsigned long size) const override;
    unsigned long write(const void* buffer, unsigned long size) override;

private:
    class implement* impl_;
};

class pipe_stream : public stream
{
    class implement;

public:
    pipe_stream(const char* command);
    pipe_stream(const char* command, const char* mode);
    pipe_stream(const pipe_stream& ps);
   ~pipe_stream(void);

    pipe_stream& operator=(const pipe_stream& ps);

public:
    void close(void);

public:
    long tell(void) const override final;
    bool seekable(void) const override;
    bool readable(void) const override;
    bool writable(void) const override;
    bool seek(long, seekdir) const override final;
    unsigned long read(void* buffer, unsigned long size) const override;
    unsigned long write(const void* buffer, unsigned long size) override;

private:
    class implement* impl_;
};

class tcp_stream : public stream
{
    class implement;

public:
    tcp_stream(int fd);
    tcp_stream(const char* host, unsigned short port);
    tcp_stream(const tcp_stream& ts);
   ~tcp_stream(void);

    tcp_stream& operator=(const tcp_stream& ts);

public:
    void close(void);

public:
    long tell(void) const override final;
    bool seekable(void) const override;
    bool readable(void) const override;
    bool writable(void) const override;
    bool seek(long, seekdir) const override final;
    unsigned long read(void* buffer, unsigned long size) const override;
    unsigned long write(const void* buffer, unsigned long size) override;

private:
    class implement* impl_;
};

class udp_stream : public stream
{
    class implement;

public:
    struct endpoint
    {
        std::string ip;
        unsigned short port;
    };

public:
    udp_stream(void);
    udp_stream(const udp_stream& us);
   ~udp_stream(void);

    udp_stream& operator=(const udp_stream& us);

public:
    bool            bind(const char* ip, unsigned short port);
    void            endpoint(struct endpoint& endpoint);
    struct endpoint endpoint(void) const;
    struct endpoint read_endpoint(void) const;

public:
    long tell(void) const override final;
    bool seekable(void) const override;
    bool readable(void) const override;
    bool writable(void) const override;
    bool seek(long, seekdir) const override final;
    unsigned long read(void* buffer, unsigned long size) const override;
    unsigned long write(const void* buffer, unsigned long size) override;

private:
    class implement* impl_;
};

class http_stream : public stream
{
    class implement;

public:
    typedef std::map<std::string, std::string> dict;

public:
    http_stream(const char* url);
    http_stream(const char* url, const char* method);
    http_stream(const char* url, const dict& headers);
    http_stream(const char* url, const char* method, const char* data);
    http_stream(const char* url, const char* method, const dict& headers);
    http_stream(const char* url, const char* method, const dict& headers,
                const char* data);
    http_stream(const http_stream& hs);
   ~http_stream(void);

    http_stream& operator=(const http_stream& hs);

public:
    int         status_code(void) const;
    std::string status_description(void) const;
    std::string header(const char* key) const;

public:
    long tell(void) const override;
    bool seekable(void) const override;
    bool readable(void) const override;
    bool writable(void) const override;
    bool seek(long, seekdir) const override final;
    unsigned long read(void* buffer, unsigned long size) const override;
    unsigned long write(const void*, unsigned long) override final;

public:
    static std::string encode_query(const dict& query);
    static dict        decode_query(const std::string& query);

private:
    class implement* impl_;
};

stream::sptr  operator""_us(const char* uri, size_t);
buffer_stream operator""_bs(const char* buffer, size_t size);
file_stream   operator""_fs(const char* filepath, size_t);
http_stream   operator""_hs(const char* url, size_t);

#endif // stream_h_20140410
