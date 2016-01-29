#include <takisy/algorithm/crc.h>

template <typename DigestType>
struct crc_struct
{
    DigestType digest_ = ~0;
};

class crc32::implement : public crc_struct<unsigned int>
{};

crc32::crc32(void)
    : impl_(new implement)
{}

crc32::crc32(const void* data, unsigned int length)
    : crc32()
{
    update(data, length);
}

crc32::crc32(const crc32& crc)
    : crc32()
{
    impl_->digest_ = crc.impl_->digest_;
}

crc32::~crc32(void)
{
    delete impl_;
}

crc32& crc32::operator=(const crc32& crc)
{
    if (this != &crc)
        impl_->digest_ = crc.impl_->digest_;

    return *this;
}

void crc32::update(const void* data, unsigned int length)
{
    const unsigned char* current = reinterpret_cast<const unsigned char*>(data);
          unsigned int&  crc     = impl_->digest_;

    static const unsigned int crc32_4bits_lut[16] =
    {
        0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
        0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
        0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
        0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c,
    };

    while (length > 0)
    {
        --length;

        crc = crc32_4bits_lut[(crc ^  *current      ) & 0x0f] ^ (crc >> 4);
        crc = crc32_4bits_lut[(crc ^ (*current >> 4)) & 0x0f] ^ (crc >> 4);

        current++;
    }
}

unsigned int crc32::digest(void) const
{
    return ~impl_->digest_;
}
