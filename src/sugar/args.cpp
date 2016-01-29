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

    struct buffer_t& buf = buffers[++index %= kBufferCount];
    char* va_list = const_cast<char*>(args.stack_.data());
    int length = vsnprintf(buf.data, buf.size, format, va_list);

    if (buf.size <= length)
    {
        delete [] buf.data;
        buf.data = new char [(buf.size = length + 1)];
        vsnprintf(buf.data, buf.size, format, va_list);
    }

    return buf.data;
}
