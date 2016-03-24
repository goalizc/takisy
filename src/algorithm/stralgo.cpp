#include <wchar.h>
#include <stdarg.h>
#include <string.h>
#include <takisy/core/codec.h>
#include <takisy/algorithm/stralgo.h>

const char* stralgo::format(const char* format, ...)
{
    static constexpr long long kBufferCount        = 64;
    static constexpr long long kOriginalBufferSize = 16;
    static struct buffer_t {
        long long size = kOriginalBufferSize;
        char*     data = new char [kOriginalBufferSize];
    } buffers[kBufferCount];
    static long long index = 0;

    va_list ap;
    va_start(ap, format);

    struct buffer_t& buffer = buffers[++index %= kBufferCount];
    int length = vsnprintf(buffer.data, buffer.size, format, ap);
    if (buffer.size <= length)
    {
        delete [] buffer.data;
        buffer.data = new char [(buffer.size = length + 1)];
        vsnprintf(buffer.data, buffer.size, format, ap);
    }

    va_end(ap);

    return buffer.data;
}

int stralgo::unformat(const char* buffer, const char* format, ...)
{
    va_list ap;
    int count;

    va_start(ap, format);
    count = vsscanf(buffer, format, ap);
    va_end(ap);

    return count;
}

std::wstring __gbk2unicode(const std::string& gbk)
{
    std::wstring result;

    for (unsigned int i = 0; i < gbk.size(); ++i)
    {
        unsigned int chr = static_cast<unsigned char>(gbk[i]);

        if (chr > 0x7f)
            chr = (chr << 8) | static_cast<unsigned char>(gbk[++i]);

        result += codec::gbk2unicode(chr);
    }

    return result;
}

std::string __unicode2gbk(const std::wstring& unicode)
{
    std::string result;

    for (wchar_t uchr : unicode)
    {
        unsigned int chr = codec::unicode2gbk(uchr);

        if (chr < 0x80)
            result += chr;
        else
        {
            result += chr >> 8;
            result += chr  & 0xff;
        }
    }

    return result;
}

std::wstring __utf82unicode(const std::string& utf8)
{
    std::wstring result;

    for (unsigned int i = 0; i < utf8.size(); )
    {
        wchar_t uchr = 0;

        if ((utf8[i] & 0x80) == 0x00)
            uchr = utf8[i++];
        else
        if ((utf8[i] & 0xe0) == 0xc0)
        {
            uchr  = (utf8[i++] & 0x1f) << 6;
            uchr |=  utf8[i++] & 0x3f;
        }
        else
        if ((utf8[i] & 0xf0) == 0xe0)
        {
            uchr  = (utf8[i++] & 0x0f) << 12;
            uchr |= (utf8[i++] & 0x3f) <<  6;
            uchr |=  utf8[i++] & 0x3f;
        }

        result += uchr;
    }

    return result;
}

std::string __unicode2utf8(const std::wstring& unicode)
{
    std::string result;

    for (wchar_t uchr : unicode)
    {
        if (uchr < 0x80)
            result += uchr;
        else
        if (uchr < 0x800)
        {
            result += 0xc0 | (uchr >> 6);
            result += 0x80 | (uchr &  0x3f);
        }
        else
        if (uchr < 0x10000)
        {
            result += 0xe0 |  (uchr >> 12);
            result += 0x80 | ((uchr >>  6) & 0x3f);
            result += 0x80 | ((uchr >>  0) & 0x3f);
        }
        else
            result += static_cast<std::string::value_type>(0);
    }

    return result;
}

stralgo::strings stralgo::codecs(void)
{
    return { "gbk", "utf-8" };
}

std::wstring stralgo::decode(const std::string& text,  const char* codec)
{
    if (strcmp(codec, "utf-8") == 0)
        return __utf82unicode(text);
    else
    // if (strcmp(codec, "gbk") == 0)
        return __gbk2unicode(text);
}

std::string stralgo::encode(const std::wstring& text, const char* codec)
{
    if (strcmp(codec, "utf-8") == 0)
        return __unicode2utf8(text);
    else
    // if (strcmp(codec, "gbk") == 0)
        return __unicode2gbk(text);
}

std::string stralgo::convert(const std::string& text,
                             const char* text_codec, const char* ret_codec)
{
    return encode(decode(text, text_codec), ret_codec);
}
