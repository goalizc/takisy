#include <cstring>
#include <third_party/md5.h>
#include <takisy/algorithm/hashalgo.h>

#define strlen(data) (((data) == nullptr) ? 0 : strlen(data))

hashalgo::base::~base(void)
{}

class hashalgo::md5::implement
{
    friend class hashalgo::md5;

public:
    implement(void)
    {
        md5_init(&md5_state_);
    }

private:
    md5_state_t md5_state_;
};

hashalgo::md5::md5(void)
    : impl_(new implement)
{}

hashalgo::md5::md5(const char* data)
    : md5(data, strlen(data))
{}

hashalgo::md5::md5(const void* data, unsigned int size)
    : md5()
{
    update(data, size);
}

hashalgo::md5::md5(const md5& _md5)
    : md5()
{
    impl_->md5_state_ = _md5.impl_->md5_state_;
}

hashalgo::md5::~md5(void)
{
    delete impl_;
}

hashalgo::md5& hashalgo::md5::operator=(const md5& md5)
{
    if (this != &md5)
        impl_->md5_state_ = md5.impl_->md5_state_;

    return *this;
}

void hashalgo::md5::update(const char* data)
{
    return update(data, strlen(data));
}

void hashalgo::md5::update(const void* data, unsigned int size)
{
    md5_append(&impl_->md5_state_,
               reinterpret_cast<const md5_byte_t*>(data),
               size);
}

std::string hashalgo::md5::digest(void) const
{
    md5_byte_t digest[16] = {0};

    md5_finish(&impl_->md5_state_, digest);

    return std::string(reinterpret_cast<char*>(digest), sizeof(digest));
}

std::string hashalgo::md5::hexdigest(void) const
{
    constexpr char kHexLiteral[] = "0123456789abcdef";
    std::string hexdigest;

    for (md5_byte_t byte : digest())
    {
        hexdigest += kHexLiteral[byte >> 4];
        hexdigest += kHexLiteral[byte & 15];
    }

    return hexdigest;
}

hashalgo::shared_ptr hashalgo::get(const char* name)
{
    return get(name, nullptr);
}

hashalgo::shared_ptr hashalgo::get(const char* name, const char* data)
{
    return get(name, data, strlen(data));
}

hashalgo::shared_ptr
    hashalgo::get(const char* name, const void* data, unsigned int size)
{
    if (strcmp(name, "md5") == 0)
        return shared_ptr(new md5(data, size));

    return nullptr;
}

#undef strlen
