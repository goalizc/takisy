#include <cstring>
#include <takisy/core/bit_buffer.h>
#include <takisy/core/endian_type.h>
#include <takisy/algorithm/crc.h>
#include <takisy/algorithm/gzip.h>
#include "rfc1951.h"

gzip::buffer_type gzip::compress(const buffer_type& buffer)
{
    return compress(buffer.data(), buffer.size());
}

gzip::buffer_type gzip::compress(const void* buffer, unsigned int length)
{
    return compress(buffer, length, clFixedHuffman);
}

gzip::buffer_type gzip::compress(const buffer_type& buffer, CompressLevel level)
{
    return compress(buffer.data(), buffer.size(), level);
}

gzip::buffer_type gzip::compress(const void* buffer, unsigned int length,
                                 CompressLevel level)
{
    bit_buffer::output output;

    output.write(0x088b1f, 24);
    output.write(0x000000, 56);

    rfc1951::deflate((const unsigned char*)buffer, length, output, level);

    unsigned int crc = crc32(buffer, length).digest();
    for (int i = 0; i < 32; i += 8)
        output.write((crc >> i) & 0xff, 8);
    for (int i = 0; i < 32; i += 8)
        output.write((length >> i) & 0xff, 8);

    return output.buffer();
}

gzip::buffer_type gzip::decompress(const buffer_type& buffer)
{
    return decompress(buffer.data(), buffer.size());
}

gzip::buffer_type gzip::decompress(const void* buffer, unsigned int length)
{
    bit_buffer::input input((const unsigned char*)buffer, length);

    if (input.read(8) != 0x1f || input.read(8) != 0x8b)
        return buffer_type();
    if (input.read(8) != 8)
        return buffer_type();

    unsigned char flag = input.read(8);
    input.read(32);
    input.read(16);

    if (flag & 0x04)
        input.read(16), input.read(input.read(16) * 8);
    if (flag & 0x08)
        while (input.read(8));
    if (flag & 0x10)
        while (input.read(8));
    if (flag & 0x02)
        input.read(16);

    buffer_type output = rfc1951::inflate(input);
    if (output.null())
        return output;

    etle_uint32 c, s;
    c.endian_value(input.read(32));
    s.endian_value(input.read(32));
    if (s != output.size() || c != crc32(output.data(), output.size()).digest())
        return buffer_type();

    return output;
 }
