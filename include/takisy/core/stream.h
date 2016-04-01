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
    enum SeekType { stBegin, stCurrent, stEnd, };

public:
    virtual ~stream(void) {}

public:
    virtual bool seek(long offset, SeekType seek_type) const = 0;
    virtual long tell(void) const = 0; // -1: invalid position
    virtual bool readable(void) const = 0;
    virtual bool writable(void) const = 0;
    virtual unsigned int read(void* buffer, unsigned int size) const = 0;
    virtual unsigned int write(const void* buffer, unsigned int size) = 0;

public:
    // read all data from src/from_uri(uri) into *this
    unsigned long plunder(const char* uri);
    unsigned long plunder(const stream& src);
    static std::shared_ptr<stream> from_uri(const char* uri);

public:
    template <typename ValueType>
    inline unsigned int read(ValueType& value) const
    {
        return read(&value, sizeof(value));
    }

    template <typename ValueType>
    inline unsigned int write(const ValueType& value)
    {
        return write(&value, sizeof(value));
    }
};

class buffer_stream : public stream
{
    class implement;

public:
    buffer_stream(void);
    buffer_stream(const void* buffer, unsigned int size);
    buffer_stream(const stretchy_buffer<unsigned char>& buffer);
    buffer_stream(const buffer_stream& bs);
   ~buffer_stream(void);

    buffer_stream& operator=(const buffer_stream& bs);

public:
    virtual bool seek(long offset, SeekType seek_type) const override;
    virtual long tell(void) const override;
    virtual bool readable(void) const override;
    virtual bool writable(void) const override;
    virtual unsigned int read(void* buffer, unsigned int size) const override;
    virtual unsigned int write(const void* buffer, unsigned int size) override;

private:
    class implement* impl_;
};

class file_stream : public stream
{
    class implement;

public:
    file_stream(const char* file_path);
    file_stream(const char* file_path, const char* mode);
    file_stream(const file& file);
    file_stream(const file_stream& fs);
   ~file_stream(void);

    file_stream& operator=(const file_stream& fs);

public:
    void close(void);

public:
    virtual bool seek(long offset, SeekType seek_type) const override;
    virtual long tell(void) const override;
    virtual bool readable(void) const override;
    virtual bool writable(void) const override;
    virtual unsigned int read(void* buffer, unsigned int size) const override;
    virtual unsigned int write(const void* buffer, unsigned int size) override;

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
    virtual bool seek(long, SeekType) const override final;
    virtual long tell(void) const override final;
    virtual bool readable(void) const override;
    virtual bool writable(void) const override;
    virtual unsigned int read(void* buffer, unsigned int size) const override;
    virtual unsigned int write(const void* buffer, unsigned int size) override;

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
    virtual bool seek(long, SeekType) const override final;
    virtual long tell(void) const override final;
    virtual bool readable(void) const override;
    virtual bool writable(void) const override;
    virtual unsigned int read(void* buffer, unsigned int size) const override;
    virtual unsigned int write(const void* buffer, unsigned int size) override;

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
    bool bind(const char* ip, unsigned short port);
    void            endpoint(struct endpoint& endpoint);
    struct endpoint endpoint(void) const;
    struct endpoint read_endpoint(void) const;

public:
    virtual bool seek(long, SeekType) const override final;
    virtual long tell(void) const override final;
    virtual bool readable(void) const override;
    virtual bool writable(void) const override;
    virtual unsigned int read(void* buffer, unsigned int size) const override;
    virtual unsigned int write(const void* buffer, unsigned int size) override;

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
    const char* status_description(void) const;
    const char* header(const char* key) const;

public:
    virtual bool seek(long, SeekType) const override final;
    virtual long tell(void) const override;
    virtual bool readable(void) const override;
    virtual bool writable(void) const override;
    virtual unsigned int read(void* buffer, unsigned int size) const override;
    virtual unsigned int write(const void*, unsigned int) override final;

public:
    static std::string encode_query(const dict& query);
    static dict        decode_query(const std::string& query);

private:
    class implement* impl_;
};

#endif // stream_h_20140410
