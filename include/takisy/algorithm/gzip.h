#ifndef gzip_h_20170601
#define gzip_h_20170601

#include <takisy/core/stretchy_buffer.h>

class gzip
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

#endif // gzip_h_20170601
