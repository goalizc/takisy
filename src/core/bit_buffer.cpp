#include <takisy/core/bit_buffer.h>

class bit_buffer::input::implement
{
    friend class bit_buffer::input;

public:
    implement(const unsigned char* buffer, unsigned int size)
        : buffer_(buffer), size_(size), Bi_(0), bi_(0)
    {}

private:
    const
    unsigned char* buffer_;
    unsigned int size_;
    unsigned int Bi_;
    unsigned int bi_;
};

bit_buffer::input::input(const unsigned char* buffer,
                                     unsigned int size)
    : impl_(new implement(buffer, size))
{}

bit_buffer::input::input(const bit_buffer::input& bbi)
    : bit_buffer::input(bbi.impl_->buffer_, bbi.impl_->size_)
{}

bit_buffer::input::~input(void)
{
    delete impl_;
}

bit_buffer::input& bit_buffer::input::operator=(const bit_buffer::input& bbi)
{
    if (this != &bbi)
    {
        impl_->buffer_ = bbi.impl_->buffer_;
        impl_->size_   = bbi.impl_->size_;
        impl_->Bi_     = bbi.impl_->Bi_;
        impl_->bi_     = bbi.impl_->bi_;
    }

    return *this;
}

void bit_buffer::input::align(void)
{
    if (impl_->bi_)
        ++impl_->Bi_, impl_->bi_ = 0;
}

unsigned int bit_buffer::input::read(register unsigned int n)
{
    static const unsigned int kMask[] =
        { 0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff, };
    register unsigned int result       = 0;
    register unsigned int fetched_bits = 0;
    const unsigned char*& buffer = impl_->buffer_;
    unsigned int& Bi = impl_->Bi_;
    unsigned int& bi = impl_->bi_;

    while (n)
    {
        if (Bi >= impl_->size_)
            throw ecOutOfBuffer;

        register unsigned int r_bi = 8 - bi;

        if (r_bi >= n)
        {
            result |= ((buffer[Bi] >> bi) & kMask[n]) << fetched_bits;
            r_bi == n ? ++Bi, bi = 0 : bi += n;
            n = 0;
        }
        else
        {
            result |= (buffer[Bi] >> bi) << fetched_bits;
            ++Bi, bi = 0, n -= r_bi, fetched_bits += r_bi;
        }
    }

    return result;
}

unsigned int bit_buffer::input::peek(register unsigned int n)
{
    unsigned int Bi = impl_->Bi_, bi = impl_->bi_;
    unsigned int ret = read(n);

    impl_->Bi_ = Bi;
    impl_->bi_ = bi;

    return ret;
}

class bit_buffer::output::implement
{
    friend class bit_buffer::output;

public:
    implement(void) : bit_buffer_(0), bit_count_(0) {}

private:
    stretchy_buffer<unsigned char> buffer_;
    unsigned int bit_buffer_;
    unsigned int bit_count_;
};

bit_buffer::output::output(void)
    : impl_(new implement)
{}

bit_buffer::output::output(const bit_buffer::output& bbo)
    : bit_buffer::output()
{
    operator=(bbo);
}

bit_buffer::output::~output(void)
{
    delete impl_;
}

bit_buffer::output& bit_buffer::output::operator=(const bit_buffer::output& bbo)
{
    if (this != & bbo)
    {
        impl_->buffer_     = bbo.impl_->buffer_.clone();
        impl_->bit_buffer_ = bbo.impl_->bit_buffer_;
        impl_->bit_count_  = bbo.impl_->bit_count_;
    }

    return *this;
}

void bit_buffer::output::align(void)
{
    if (impl_->bit_count_)
        write(0, 8 - impl_->bit_count_);
}

void bit_buffer::output::write(unsigned int bits, unsigned int count)
{
    impl_->bit_buffer_ |= bits << impl_->bit_count_;
    impl_->bit_count_  += count;

    while (impl_->bit_count_ >= 8)
    {
        impl_->buffer_.append(static_cast<unsigned char>(impl_->bit_buffer_));
        impl_->bit_buffer_ >>= 8;
        impl_->bit_count_   -= 8;
    }
}

stretchy_buffer<unsigned char> bit_buffer::output::buffer(void) const
{
    return impl_->buffer_;
}
