#ifndef hashalgo_h_20141230
#define hashalgo_h_20141230

#include <string>
#include <memory>

class hashalgo
{
public:
    class base;

public:
    class md5;
    class sha1;
    class sha224;
    class sha256;
    class sha384;
    class sha512;

public:
    typedef std::shared_ptr<base> smart_ptr;

    static smart_ptr get(const char* name);
    static smart_ptr get(const char* name, const char* data);
    static smart_ptr get(const char* name, const void* data, unsigned int size);
};

class hashalgo::base
{
public:
    ~base(void);

public:
    virtual void update(const char* data) = 0;
    virtual void update(const void* data, unsigned int size) = 0;
    virtual std::string digest(void) const = 0;
    virtual std::string hexdigest(void) const = 0;
};

class hashalgo::md5 : public hashalgo::base
{
    class implement;

public:
    md5(void);
    md5(const char* data);
    md5(const void* data, unsigned int size);
    md5(const md5& md5);
   ~md5(void);

    md5& operator=(const md5& md5);

public:
    void update(const char* data) override;
    void update(const void* data, unsigned int size) override;
    std::string digest(void) const override;
    std::string hexdigest(void) const override;

private:
    class implement* impl_;
};

#endif // hashalgo_h_20141230
