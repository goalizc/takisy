#ifndef zlib_h_20140325
#define zlib_h_20140325

#include <takisy/core/stretchy_buffer.h>

class zlib
{
public:
    typedef stretchy_buffer<unsigned char> buffer_type;

    enum CompressLevel
    {
        clStore          = 0,
        clFixedHuffman   = 1,
        clDynamicHuffman = 2, // not implement, use fixed huffman
    };

public:
    static buffer_type compress(const buffer_type& buffer);
    static buffer_type compress(const void* buffer, unsigned int length);
    static buffer_type compress(const buffer_type& buffer, CompressLevel level);
    static buffer_type compress(const void* buffer, unsigned int length,
                                CompressLevel level);

    static buffer_type decompress(const buffer_type& buffer);
    static buffer_type decompress(const void* buffer, unsigned int length);
};

#endif // zlib_h_20140325
