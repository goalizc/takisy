#ifndef enhancive_basic_string_h_20160316
#define enhancive_basic_string_h_20160316

#include <string>

template<
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>
> class enhancive_basic_string
    : public std::basic_string<CharT, Traits, Allocator>
{
public:
    typedef std::basic_string<CharT, Traits, Allocator> basic_string;

public:
    using basic_string::basic_string;

    enhancive_basic_string(void)
        : basic_string()
    {}

    enhancive_basic_string(const basic_string& other)
        : basic_string(other)
    {}

    enhancive_basic_string(basic_string&& other)
        : basic_string(other)
    {}

    enhancive_basic_string& operator=(const basic_string& other)
    {
        basic_string::operator=(other);

        return *this;
    }

    enhancive_basic_string& operator=(basic_string&& other)
    {
        basic_string::operator=(other);

        return *this;
    }

public:
    enhancive_basic_string& replace(CharT ch1, CharT ch2)
    {
        for (CharT& ch : *this)
            if (ch == ch1)
                ch = ch2;

        return *this;
    }

    enhancive_basic_string replacec(CharT ch1, CharT ch2) const
    {
        return enhancive_basic_string(*this).replace(ch1, ch2);
    }
};

typedef enhancive_basic_string<char>     enhancive_string;
typedef enhancive_basic_string<wchar_t>  enhancive_wstring;
typedef enhancive_basic_string<char16_t> enhancive_u16string;
typedef enhancive_basic_string<char32_t> enhancive_u32string;

#endif // enhancive_basic_string_h_20160316
