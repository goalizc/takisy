#ifndef stralgo_hpp_20140508
#define stralgo_hpp_20140508

#include <cstdio>
#include <cctype>
#include <cwctype>
#include <string>
#include <vector>
#include <algorithm>

struct stralgo
{
    typedef std::vector<std::string>  strings;
    typedef std::vector<std::wstring> wstrings;

    #define __wrap__(function, ret_t, arg_t) \
        static inline ret_t function(arg_t arg) { return ::function(arg); }

        __wrap__(isalnum,  bool, char); __wrap__(iswalnum,  bool,    wchar_t);
        __wrap__(isalpha,  bool, char); __wrap__(iswalpha,  bool,    wchar_t);
        __wrap__(iscntrl,  bool, char); __wrap__(iswcntrl,  bool,    wchar_t);
        __wrap__(isdigit,  bool, char); __wrap__(iswdigit,  bool,    wchar_t);
        __wrap__(isgraph,  bool, char); __wrap__(iswgraph,  bool,    wchar_t);
        __wrap__(islower,  bool, char); __wrap__(iswlower,  bool,    wchar_t);
        __wrap__(isprint,  bool, char); __wrap__(iswprint,  bool,    wchar_t);
        __wrap__(ispunct,  bool, char); __wrap__(iswpunct,  bool,    wchar_t);
        __wrap__(isspace,  bool, char); __wrap__(iswspace,  bool,    wchar_t);
        __wrap__(isupper,  bool, char); __wrap__(iswupper,  bool,    wchar_t);
        __wrap__(isxdigit, bool, char); __wrap__(iswxdigit, bool,    wchar_t);
        __wrap__(tolower,  char, char); __wrap__(towlower,  wchar_t, wchar_t);
        __wrap__(toupper,  char, char); __wrap__(towupper,  wchar_t, wchar_t);
    #undef __wrap__

    static const char* format(const char* format, ...);
    static int       unformat(const char* buffer, const char* format, ...);

    static strings      codecs(void);
    static std::wstring decode(const std::string&  text, const char* codec);
    static std::string  encode(const std::wstring& text, const char* codec);
    static std::string  convert(const std::string& text,
                                const char* text_codec, const char* ret_codec);

    template <typename CharType>
    static std::vector<std::basic_string<CharType>>
        split(const std::basic_string<CharType>& str,
              const std::basic_string<CharType>& token,
              int max_split, bool compress_token = true)
    {
        typedef std::basic_string<CharType> string_type;
        typename string_type::size_type pos = 0, last_pos = pos;
        std::vector<string_type> result;

        while ((pos = str.find(token, pos)) != string_type::npos
               && result.size() < static_cast<unsigned int>(max_split))
        {
            if (!compress_token || pos != last_pos || pos == 0)
                result.push_back(str.substr(last_pos, pos - last_pos));
            last_pos = pos += token.size();
        }

        result.push_back(str.substr(last_pos));

        return result;
    }

    template <typename CharType>
    static std::vector<std::basic_string<CharType>>
        split(const std::basic_string<CharType>& str,
              const std::basic_string<CharType>& token,
              bool compress_token = true)
    {
        return split(str, token, -1, compress_token);
    }

    template <typename CharType>
    static inline std::vector<std::basic_string<CharType>>
        split(const std::basic_string<CharType>& str, CharType token,
              int max_split, bool compress_token = true)
    {
        return split(str, std::basic_string<CharType>(1, token),
                     max_split, compress_token);
    }

    template <typename CharType>
    static inline std::vector<std::basic_string<CharType>>
        split(const std::basic_string<CharType>& str, CharType token,
              bool compress_token = true)
    {
        return split(str, std::basic_string<CharType>(1, token),
                     compress_token);
    }

    template <typename CharType>
    static inline std::vector<std::basic_string<CharType>>
        split(const CharType* str, const CharType* token,
              int max_split, bool compress_token = true)
    {
        return split(std::basic_string<CharType>(str),
                     std::basic_string<CharType>(token),
                     max_split, compress_token);
    }

    template <typename CharType>
    static std::vector<std::basic_string<CharType>>
        split(const CharType* str, const CharType* token,
              bool compress_token = true)
    {
        return split(std::basic_string<CharType>(str),
                     std::basic_string<CharType>(token), -1, compress_token);
    }

    template <typename CharType>
    static inline std::vector<std::basic_string<CharType>>
        split(const CharType* str, CharType token,
              int max_split, bool compress_token = true)
    {
        return split(std::basic_string<CharType>(str),
                     std::basic_string<CharType>(1, token),
                     max_split, compress_token);
    }

    template <typename CharType>
    static inline std::vector<std::basic_string<CharType>>
        split(const CharType* str, CharType token,
              bool compress_token = true)
    {
        return split(std::basic_string<CharType>(str),
                     std::basic_string<CharType>(1, token), compress_token);
    }

    static inline std::string& lower(std::string& str)
    {
        for (std::string::value_type& ch : str)
            ch = tolower(ch);
        return str;
    }

    static inline std::string& upper(std::string& str)
    {
        for (std::string::value_type& ch : str)
            ch = toupper(ch);
        return str;
    }

    static inline std::wstring& lower(std::wstring& str)
    {
        for (std::wstring::value_type& ch : str)
            ch = towlower(ch);
        return str;
    }

    static inline std::wstring& upper(std::wstring& str)
    {
        for (std::wstring::value_type& ch : str)
            ch = towupper(ch);
        return str;
    }

    static inline std::string lowerc(std::string str)
    {
        return lower(str);
    }

    static inline std::string upperc(std::string str)
    {
        return upper(str);
    }

    static inline std::wstring lowerc(std::wstring str)
    {
        return lower(str);
    }

    static inline std::wstring upperc(std::wstring str)
    {
        return upper(str);
    }

    template <typename CharType>
    static inline std::basic_string<CharType>&
        ltrim(std::basic_string<CharType>& str)
        { return ltrim(str, (CharType)0x20); }
    template <typename CharType>
    static inline std::basic_string<CharType>&
        ltrim(std::basic_string<CharType>& str, CharType ch)
        { return str.erase(0, str.find_first_not_of(ch)); }
    template <typename CharType>
    static inline std::basic_string<CharType>&
        rtrim(std::basic_string<CharType>& str)
        { return rtrim(str, (CharType)0x20); }
    template <typename CharType>
    static inline std::basic_string<CharType>&
        rtrim(std::basic_string<CharType>& str, CharType ch)
        { return str.erase(str.find_last_not_of(ch) + 1); }
    template <typename CharType>
    static inline std::basic_string<CharType>&
        trim(std::basic_string<CharType>& str)
        { return trim(str, (CharType)0x20); }
    template <typename CharType>
    static inline std::basic_string<CharType>&
        trim(std::basic_string<CharType>& str, CharType ch)
        { return ltrim(rtrim(str, ch), ch); }
    template <typename CharType>
    static inline std::basic_string<CharType>
        ltrimc(std::basic_string<CharType> str)
        { return ltrimc(str, (CharType)0x20); }
    template <typename CharType>
    static inline std::basic_string<CharType>
        ltrimc(const CharType* str)
        { return ltrimc(std::basic_string<CharType>(str), (CharType)0x20); }
    template <typename CharType>
    static inline std::basic_string<CharType>
        ltrimc(std::basic_string<CharType> str, CharType ch)
        { return ltrim(str, ch); }
    template <typename CharType>
    static inline std::basic_string<CharType>
        ltrimc(const CharType* str, CharType ch)
        { return ltrim(std::basic_string<CharType>(str), ch); }
    template <typename CharType>
    static inline std::basic_string<CharType>
        rtrimc(std::basic_string<CharType> str)
        { return rtrimc(str, (CharType)0x20); }
    template <typename CharType>
    static inline std::basic_string<CharType>
        rtrimc(const CharType* str)
        { return rtrimc(std::basic_string<CharType>(str), (CharType)0x20); }
    template <typename CharType>
    static inline std::basic_string<CharType>
        rtrimc(std::basic_string<CharType> str, CharType ch)
        { return rtrim(str, ch); }
    template <typename CharType>
    static inline std::basic_string<CharType>
        rtrimc(const CharType* str, CharType ch)
        { return rtrim(std::basic_string<CharType>(str), ch); }
    template <typename CharType>
    static inline std::basic_string<CharType>
        trimc(std::basic_string<CharType> str)
        { return trimc(str, (CharType)0x20); }
    template <typename CharType>
    static inline std::basic_string<CharType>
        trimc(const CharType* str)
        { return trimc(std::basic_string<CharType>(str), (CharType)0x20); }
    template <typename CharType>
    static inline std::basic_string<CharType>
        trimc(std::basic_string<CharType> str, CharType ch)
        { return trim(str, ch); }
    template <typename CharType>
    static inline std::basic_string<CharType>
        trimc(const CharType* str, CharType ch)
        { return trim(std::basic_string<CharType>(str), ch); }
};

#endif //stralgo_hpp_20140508
