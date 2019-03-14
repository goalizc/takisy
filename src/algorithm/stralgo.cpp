#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <takisy/core/codec.h>
#include <takisy/algorithm/stralgo.h>
#include "grisu.h"

std::string stralgo::strf(float value)
{
    return grisu::strf(value);
}

std::string stralgo::strf(double value)
{
    return grisu::strf(value);
}

std::string stralgo::strf(long double value)
{
    return sprintf("%.30Lg", value);
}

stralgo::string stralgo::sprintf(const std::string& pattern, ...)
{
    static struct {
        long size  = 64;
        char *data = new char [64];
    } buffer;

    va_list valist;
    va_start(valist, pattern);

    int length = vsnprintf(buffer.data, buffer.size, pattern.c_str(), valist);
    if (buffer.size <= length)
    {
        delete [] buffer.data;
        buffer.data = new char [(buffer.size = length + 1)];
        vsnprintf(buffer.data, buffer.size, pattern.c_str(), valist);
    }

    va_end(valist);

    return buffer.data;
}

stralgo::strings stralgo::codecs(void)
{
    return { "gbk", "utf8" };
}

stralgo::wstring
stralgo::decode(const std::string& text, const std::string& codec)
{
    if (codec == "gbk")
        return codec::gbk2uni(text);
    else
    if (codec == "utf8")
        return codec::utf82uni(text);
    else
        return L"";
}

stralgo::string
stralgo::encode(const std::wstring& text, const std::string& codec)
{
    if (codec == "gbk")
        return codec::uni2gbk(text);
    else
    if (codec == "utf8")
        return codec::uni2utf8(text);
    else
        return "";
}

stralgo::string
stralgo::convert(const std::string& text,
                 const std::string& text_codec, const std::string& conv_codec)
{
    return encode(decode(text, text_codec), conv_codec);
}

stralgo::string stralgo::format_ss(const char* ptr, const strings& strs)
{
    stralgo::string string;

    while (*ptr) {
        if (ptr[0] != '$')
            string += *ptr++;
        else {
            if (isdigit(ptr[1])) {
                const char* endptr = ptr + 1;
                unsigned int i = 0;
                do { i = i * 10 + *endptr - '0'; } while (isdigit(*++endptr));
                if (i < strs.size()) {
                    string += strs[i];
                    if (ptr = endptr, *ptr == ';')
                        ++ptr;
                } else
                    do { string += *ptr++; } while (ptr != endptr);
            } else if (string += *ptr++, *ptr == ';')
                ++ptr;
        }
    }

    return std::move(string);
}

stralgo::string operator""_ts(const char* cstr, size_t len)
{
    return stralgo::string(cstr, len);
}
