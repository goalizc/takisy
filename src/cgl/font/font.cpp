#include <takisy/cgl/font/font.h>

font::~font(void)
{}

bitmap::Gray bitmap::get_gray(unsigned char pixel_mode)
{
    switch (pixel_mode)
    {
    case pmMono:
        return [](const unsigned char* buffer,
                  unsigned int x) -> unsigned char {
            constexpr unsigned char kMask[] =
                { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
            return buffer[x >> 3] & kMask[x % 8] ? 0xff : 0;
        };
    case pmGray:
        return [](const unsigned char* buffer,
                  unsigned int x) -> unsigned char {
            return buffer[x];
        };
    case pmGray2:
    case pmGray4:
    default:
        return [](const unsigned char*, unsigned int) -> unsigned char {
            return 0x80;
        };
    }
}
