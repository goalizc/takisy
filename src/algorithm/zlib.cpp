#include <cstring>
#include <takisy/core/bit_buffer.h>
#include <takisy/algorithm/adler.h>
#include <takisy/algorithm/zlib.h>
#include "rfc1951.h"

zlib::buffer_t zlib::compress(const buffer_t& buffer)
{
    return compress(buffer.data(), buffer.size(), clFixedHuffman);
}

zlib::buffer_t zlib::compress(const void* buffer, unsigned int length)
{
    return compress(buffer, length, clFixedHuffman);
}

zlib::buffer_t zlib::compress(const buffer_t& buffer, CompressLevel level)
{
    return compress(buffer.data(), buffer.size(), level);
}

zlib::buffer_t zlib::compress(const void* buffer, unsigned int length,
                              CompressLevel level)
{
    bit_buffer_o output;

    output.write(0x78, 8);
    output.write(0x5e, 8);

    rfc1951::deflate(reinterpret_cast<const unsigned char*>(buffer),
                     length, output, level);

    unsigned int adler = adler32(buffer, length).digest();

    for (int i = 24; i >= 0; i -= 8)
        output.write((adler >> i) & 0xff, 8);

    return output.buffer();
}

zlib::buffer_t zlib::decompress(const buffer_t& buffer)
{
    return decompress(buffer.data(), buffer.size());
}

zlib::buffer_t zlib::decompress(const void* buffer, unsigned int length)
{
    bit_buffer_i input(reinterpret_cast<const unsigned char*>(buffer), length);

    unsigned char flag = input.read(8);
    if (flag != 0x78 || ((flag << 8) | input.read(8)) % 31 != 0)
        return buffer_t();

    buffer_t output = rfc1951::inflate(input);
    if (output.null())
        return output;

    unsigned int adler = 0;
    for (int i = 0; i < 4; ++i)
        adler = (adler << 8) | input.read(8);

    if (adler != adler32(output.data(), output.size()).digest())
        return buffer_t();

    return output;
}
