#include <wchar.h>
#include <stdarg.h>
#include <string.h>
#include <takisy/core/codec.h>
#include <takisy/algorithm/stralgo.h>

std::string stralgo::format(const std::string& format, ...)
{
    static constexpr long long kOriginalBufferSize = 16;
    static struct {
        long long size = kOriginalBufferSize;
        char*     data = new char [kOriginalBufferSize];
    } buffer;

    va_list ap;
    va_start(ap, format);

    int length = vsnprintf(buffer.data, buffer.size, format.c_str(), ap);
    if (buffer.size <= length)
    {
        delete [] buffer.data;
        buffer.data = new char [(buffer.size = length + 1)];
        vsnprintf(buffer.data, buffer.size, format.c_str(), ap);
    }

    va_end(ap);

    return buffer.data;
}

int stralgo::unformat(const std::string& buffer, const std::string& format, ...)
{
    va_list ap;
    int count;

    va_start(ap, format);
    count = vsscanf(buffer.c_str(), format.c_str(), ap);
    va_end(ap);

    return count;
}

stralgo::strings stralgo::codecs(void)
{
    return { "gb2312", "gbk", "utf-8" };
}

std::wstring stralgo::decode(const std::string& text,
                             const std::string& codec)
{
    if (codec == "utf-8")
        return codec::utf82unicode(text);
    else
    if (codec == "gb2312" || codec == "gbk")
        return codec::gbk2unicode(text);
    else
        return L"";
}

std::string stralgo::encode(const std::wstring& text,
                            const std::string& codec)
{
    if (codec == "utf-8")
        return codec::unicode2utf8(text);
    else
    if (codec == "gb2312" || codec == "gbk")
        return codec::unicode2gbk(text);
    else
        return "";
}

std::string stralgo::convert(const std::string& text,
                             const std::string& text_codec,
                             const std::string& convert_codec)
{
    return encode(decode(text, text_codec), convert_codec);
}
