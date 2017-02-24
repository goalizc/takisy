#include <cwchar>
#include <cstdarg>
#include <cstring>
#include <third_party/milo.h>
#include <takisy/core/codec.h>
#include <takisy/algorithm/stralgo.h>

stralgo::string stralgo::strf(float value)
{
    return strf_milo(static_cast<double>(value));
}

stralgo::string stralgo::strf(double value)
{
    return strf_milo(value);
}

stralgo::string stralgo::strf(long double value)
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
    return { "gb2312", "gbk", "utf-8" };
}

stralgo::wstring stralgo::decode(const std::string& text, const string& codec)
{
    if (codec == "utf-8")
        return codec::utf82unicode(text);
    else
    if (codec == "gb2312" || codec == "gbk")
        return codec::gbk2unicode(text);
    else
        return L"";
}

stralgo::string stralgo::encode(const std::wstring& text, const string& codec)
{
    if (codec == "utf-8")
        return codec::unicode2utf8(text);
    else
    if (codec == "gb2312" || codec == "gbk")
        return codec::unicode2gbk(text);
    else
        return "";
}

stralgo::string
    stralgo::convert(const std::string& text,
                     const string& text_codec, const string& convert_codec)
{
    return encode(decode(text, text_codec), convert_codec);
}

stralgo::string stralgo::format_ss(const char* ptr, const strings& strings)
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
                if (i < strings.size()) {
                    string += strings[i];
                    if (*(ptr = endptr) == ';')
                        ++ptr;
                } else
                    do { string += *ptr++; } while (ptr != endptr);
            } else if (ptr[1] != ';')
                string += *ptr++;
            else
                string += *ptr++, ++ptr;
        }
    }

    return string;
}

template <>
bool stralgo::string::as<bool>(void) const
{
    return lower() == "true" || as<int>();
}

template <>
float stralgo::string::as<float>(void) const
{
    return atof(base_type::c_str());
}

template <>
double stralgo::string::as<double>(void) const
{
    return atof(base_type::c_str());
}

template <>
long double stralgo::string::as<long double>(void) const
{
    long double value;

    if (sscanf(base_type::c_str(), "%Lg", &value) == 1)
        return value;

    return 0;
}
