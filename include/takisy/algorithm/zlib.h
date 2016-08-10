#ifndef zlib_h_20140325
#define zlib_h_20140325

#include <takisy/core/stretchy_buffer.h>

class zlib
{
public:
    typedef stretchy_buffer<unsigned char> buffer_t;

    enum CompressLevel
    {
        clStore          = 0,
        clFixedHuffman   = 1,
        clDynamicHuffman = 2, // not implement, use fixed huffman
    };

public:
    static buffer_t compress(const buffer_t& buffer);
    static buffer_t compress(const void* buffer, unsigned int length);
    static buffer_t compress(const buffer_t& buffer, CompressLevel level);
    static buffer_t compress(const void* buffer, unsigned int length,
                             CompressLevel level);

    static buffer_t decompress(const buffer_t& buffer);
    static buffer_t decompress(const void* buffer, unsigned int length);
};

#endif // zlib_h_20140325
