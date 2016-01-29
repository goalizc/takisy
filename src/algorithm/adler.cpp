#include <takisy/algorithm/adler.h>

class adler32::implement
{
    friend class adler32;

private:
    unsigned int s1_ = 1;
    unsigned int s2_ = 0;
};

adler32::adler32(void)
    : impl_(new implement)
{}

adler32::adler32(const void* data, unsigned int length)
    : adler32()
{
    update(data, length);
}

adler32::adler32(const adler32& adler)
    : adler32()
{
    impl_->s1_ = adler.impl_->s1_;
    impl_->s2_ = adler.impl_->s2_;
}

adler32::~adler32(void)
{
    delete impl_;
}

adler32& adler32::operator=(const adler32& adler)
{
    if (this != &adler)
    {
        impl_->s1_ = adler.impl_->s1_;
        impl_->s2_ = adler.impl_->s2_;
    }

    return *this;
}

void adler32::update(const void* data, unsigned int length)
{
    typedef unsigned char uchar_t;
    const uchar_t* uchar_data = reinterpret_cast<const uchar_t*>(data);
    unsigned int i = 0, j = 0, block_length = length % 5552;

    while (j < length)
    {
        for (i = 0; i < block_length; ++i)
            impl_->s1_ += uchar_data[j + i], impl_->s2_ += impl_->s1_;

        impl_->s1_ %= 65521;
        impl_->s2_ %= 65521;

        j += block_length;

        block_length = 5552;
    }
}

unsigned int adler32::digest(void) const
{
    return (impl_->s2_ << 16) | impl_->s1_;
}
