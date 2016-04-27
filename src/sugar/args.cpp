#include <cstdio>
#include <takisy/sugar/args.h>

const char* operator%(const char* format, const args& args)
{
    static constexpr long long kBufferCount        = 99;
    static constexpr long long kOriginalBufferSize = 16;
    static struct buffer_t {
        long long size = kOriginalBufferSize;
        char*     data = new char [kOriginalBufferSize];
    } buffers[kBufferCount];
    static long long index = 0;

    struct buffer_t& buffer = buffers[++index %= kBufferCount];
    char* va_list = const_cast<char*>(args.stack_.data());
    int length = vsnprintf(buffer.data, buffer.size, format, va_list);

    if (buffer.size <= length)
    {
        delete [] buffer.data;
        buffer.data = new char [(buffer.size = length + 1)];
        vsnprintf(buffer.data, buffer.size, format, va_list);
    }

    return buffer.data;
}
