#ifndef stralgo_hpp_20140508
#define stralgo_hpp_20140508

#include <algorithm>
#include <cctype>
#include <cwctype>
#include <string>
#include <vector>

struct stralgo
{
    template <typename CharT>
    class basic_string : public std::basic_string<CharT> {
        typedef std::basic_string<CharT> base_type;
        typedef basic_string self_type;
    public:
        using base_type::base_type;
        using base_type::operator=;
    public:
        basic_string(void) : base_type() {}
        basic_string(const base_type& other) : base_type(other) {}
        basic_string(base_type&& other) : base_type(std::move(other)) {}
        self_type& operator=(const base_type& other) {
            base_type::operator=(other);
            return *this;
        }
        self_type& operator=(base_type&& other) {
            base_type::operator=(std::move(other));
            return *this;
        }
    public:
        bool startswith(const base_type& prefix) const {
            if (base_type::size() < prefix.size())
                return false;
            for (unsigned int i = 0; i < prefix.size(); ++i)
                if (base_type::operator[](i) != prefix[i])
                    return false;
            return true;
        }
        bool endswith(const base_type& suffix) const {
            if (base_type::size() < suffix.size())
                return false;
            for (unsigned int i = base_type::size() - suffix.size(), j = 0;
                    j < suffix.size(); ++i, ++j)
                if (base_type::operator[](i) != suffix[i])
                    return false;
            return true;
        }
    public:
        self_type lower(void) const { return stralgo::lower(*this); }
        self_type upper(void) const { return stralgo::upper(*this); }
    public:
        self_type trim (CharT ch = 0x20) const {
            return stralgo::trim (*this, ch);
        }
        self_type triml(CharT ch = 0x20) const {
            return stralgo::triml(*this, ch);
        }
        self_type trimr(CharT ch = 0x20) const {
            return stralgo::trimr(*this, ch);
        }
    public:
        self_type replace(const base_type& aim, const base_type& goal) const {
            return stralgo::replace(*this, aim, goal);
        }
    public:
        std::vector<self_type>
            split(const base_type& token, bool compress_token = true) const {
            return stralgo::split(*this, token, -1, compress_token);
        }
        std::vector<self_type>
            split(const base_type& token,
                  int max_split, bool compress_token = true) const {
            return stralgo::split(*this, token, max_split, compress_token);
        }
    };

    class string : public basic_string<char> {
        typedef basic_string<char> base_type;
        typedef string self_type;
    public:
        using base_type::base_type;
        using base_type::operator=;
    public:
        string(void) : base_type() {}
        string(const base_type& other) : base_type(other) {}
        string(base_type&& other) : base_type(std::move(other)) {}
        self_type& operator=(const base_type& other) {
            base_type::operator=(other);
            return *this;
        }
        self_type& operator=(base_type&& other) {
            base_type::operator=(std::move(other));
            return *this;
        }
    public:
        template <typename T>
        T as(void) const {
            return stralgo::frts<T>(base_type::c_str());
        }
        template <typename... Args>
        self_type sprintf(const Args&... args) const {
            return stralgo::sprintf(base_type::c_str(), args...);
        }
        template <typename... Args>
        self_type format(const Args&... args) const {
            return stralgo::format(base_type::c_str(), args...);
        }
    };

    typedef basic_string<wchar_t> wstring;
    typedef std::vector<basic_string<char>> strings;
    typedef std::vector<wstring> wstrings;

#define __wrap__(isxxx, ret, arg_t) \
    static ret isxxx(arg_t arg) { return ::isxxx(arg); }

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

    template <int base = 10, typename IntegralType>
    static stralgo::string strf(IntegralType integer)
    {
        static_assert(2 <= base && base <= 36,
                      "`int base' should be between 2 and 36.");
        static_assert(std::is_integral<IntegralType>::value,
                      "`typename IntegralType' must be an integral type.");

        static constexpr const char literal[] =
                "zyxwvutsrqponmlkjihgfedcba987654321" "0"
                "123456789abcdefghijklmnopqrstuvwxyz";
        static constexpr const char *zero = &literal[35];
        bool is_negative = std::is_signed<IntegralType>::value && integer < 0;
        stralgo::string result;

        do { result += zero[integer % base]; integer /= base; }
        while (integer);

        if (is_negative)
            result += '-';

        std::reverse(result.begin(), result.end());

        return result;
    }

    static string strf(char ch) { return string(1, ch); }
    static string strf(float number);
    static string strf(double number);
    static string strf(long double number);
    static string strf(bool boolean) { return boolean ? "true" : "false"; };
    static const char* strf(const char* str) { return str; }
    static const std::string& strf(const std::string& str) { return str; }
    static std::string&& strf(std::string&& str) { return std::move(str); }
    static const basic_string<char>& strf(const basic_string<char>& str)
        { return str; }
    static basic_string<char>&& strf(basic_string<char>&& str)
        { return std::move(str); }
    static const string& strf(const string& str) { return str; }
    static string&& strf(string&& str) { return std::move(str); }
    static string sprintf(const std::string& pattern, ...);
    template <typename... Args>
    static string format(const std::string& pattern, const Args&... args)
    {
        strings strings;
        strsf(strings, args...);
        return format_ss(pattern.c_str(), strings);
    }

    template <typename IntegralType, int base = 10, bool skip_space = true>
    static IntegralType frts(const char* str)
    {
        static_assert(2 <= base && base <= 36,
                      "`int base' should be between 2 and 36.");
        static_assert(std::is_integral<IntegralType>::value,
                      "`typename IntegralType' must be an integral type.");

        static constexpr const unsigned int implicit[] = {
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  0,  0,  0,  0,  0,  0,
            0,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
            25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 0,  0,  0,  0,  0,
            0,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
            25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 0,  0,  0,  0,  0, };
        IntegralType integer = 0;
        bool positive = true;

        while (skip_space && isspace(*str))
            ++str;

        if (std::is_signed<IntegralType>::value && *str == '-')
            positive = false, ++str;

        unsigned int len = 0;
        while (isfrts<base>(*str) && len++ < sizeof(IntegralType) * 8)
            integer = integer * base + implicit[(int)*str++];

        return positive ? integer : -integer;
    }

    template <typename IntegralType, int base = 10, bool skip_space = true>
    static IntegralType frts(const std::string& str)
    {
        return frts<IntegralType, base, skip_space>(str.c_str());
    }

    static double atof(const char* str) { return ::atof(str); }
    static double atof(const std::string& str) { return ::atof(str.c_str()); }

#define atox(name, type)                                          \
    static type name(const char* str) { return frts<type>(str); } \
    static type name(const std::string& str) { return frts<type>(str.c_str()); }

    atox(atoi,   int)
    atox(atoui,  unsigned int)
    atox(atol,   long)
    atox(atoul,  unsigned long)
    atox(atoll,  long long)
    atox(atoull, unsigned long long)
#undef atox

    static strings codecs (void);
    static wstring decode (const std::string&,  const string&);
    static  string encode (const std::wstring&, const string&);
    static  string convert(const std::string&,  const string&, const string&);

    template <typename CharT>
    static basic_string<CharT>
        replace(const std::basic_string<CharT>& str,
                const std::basic_string<CharT>& aim,
                const std::basic_string<CharT>& goal, int max_replace = -1)
    {
        typedef basic_string<CharT> string;
        typename string::size_type pos = 0, lastpos = pos;
        unsigned int replaced = 0;
        string result;

        while ((pos = str.find(aim, pos)) != string::npos
               && replaced < static_cast<unsigned int>(max_replace))
        {
            result += str.substr(lastpos, pos - lastpos);
            result += goal;
            lastpos = pos += aim.size();
            ++replaced;
        }

        result += str.substr(lastpos);

        return std::move(result);
    }

    template <typename CharT>
    static basic_string<CharT>
        replace(const std::basic_string<CharT>& str,
                const std::basic_string<CharT>& aim, const CharT* goal,
                int max_replace = -1)
    {
        return replace(str, aim, std::basic_string<CharT>(goal), max_replace);
    }

    template <typename CharT>
    static basic_string<CharT>
        replace(const std::basic_string<CharT>& str,
                const CharT* aim, const std::basic_string<CharT>& goal,
                int max_replace = -1)
    {
        return replace(str, std::basic_string<CharT>(aim), goal, max_replace);
    }

    template <typename CharT>
    static basic_string<CharT>
        replace(const std::basic_string<CharT>& str,
                const CharT* aim, const CharT* goal, int max_replace = -1)
    {
        return replace(str, std::basic_string<CharT>(aim),
                            std::basic_string<CharT>(goal), max_replace);
    }

    template <typename CharT>
    static basic_string<CharT>
        replace(const CharT* str,
                const std::basic_string<CharT>& aim,
                const std::basic_string<CharT>& goal,
                int max_replace = -1)
    {
        return replace(std::basic_string<CharT>(str), aim, goal, max_replace);
    }

    template <typename CharT>
    static basic_string<CharT>
        replace(const CharT* str,
                const std::basic_string<CharT>& aim, const CharT* goal,
                int max_replace = -1)
    {
        return replace(std::basic_string<CharT>(str),
                       aim, std::basic_string<CharT>(goal), max_replace);
    }

    template <typename CharT>
    static basic_string<CharT>
        replace(const CharT* str,
                const CharT* aim, const std::basic_string<CharT>& goal,
                int max_replace = -1)
    {
        return replace(std::basic_string<CharT>(str),
                       std::basic_string<CharT>(aim), goal, max_replace);
    }

    template <typename CharT>
    static basic_string<CharT>
        replace(const CharT* str, const CharT* aim, const CharT* goal,
                int max_replace = -1)
    {
        return replace(std::basic_string<CharT>(str),
                       std::basic_string<CharT>(aim),
                       std::basic_string<CharT>(goal), max_replace);
    }

    template <typename CharT>
    static std::vector<basic_string<CharT>>
        split(const std::basic_string<CharT>& str,
              const std::basic_string<CharT>& token,
              int max_split, bool compress_token = true)
    {
        typedef basic_string<CharT> string;
        typename string::size_type pos = 0, lastpos = pos;
        std::vector<string> result;

        while ((pos = str.find(token, pos)) != string::npos
               && result.size() < static_cast<unsigned int>(max_split))
        {
            if (!compress_token || pos != lastpos || pos == 0)
                result.emplace_back(str.substr(lastpos, pos - lastpos));
            lastpos = pos += token.size();
        }

        result.emplace_back(str.substr(lastpos));

        return std::move(result);
    }

    template <typename CharT>
    static std::vector<basic_string<CharT>>
        split(const std::basic_string<CharT>& str, const CharT* token,
              int max_split, bool compress_token = true)
    {
        return split(str, std::basic_string<CharT>(token),
                     max_split, compress_token);
    }

    template <typename CharT>
    static std::vector<basic_string<CharT>>
        split(const CharT* str, const std::basic_string<CharT>& token,
              int max_split, bool compress_token = true)
    {
        return split(std::basic_string<CharT>(str), token,
                     max_split, compress_token);
    }

    template <typename CharT>
    static std::vector<basic_string<CharT>>
        split(const CharT* str, const CharT* token,
              int max_split, bool compress_token = true)
    {
        return split(std::basic_string<CharT>(str),
                     std::basic_string<CharT>(token),
                     max_split, compress_token);
    }

    template <typename CharT>
    static std::vector<basic_string<CharT>>
        split(const std::basic_string<CharT>& str,
              const std::basic_string<CharT>& token,
              bool compress_token = true)
    {
        return split(str, token, -1, compress_token);
    }

    template <typename CharT>
    static std::vector<basic_string<CharT>>
        split(const std::basic_string<CharT>& str, const CharT* token,
              bool compress_token = true)
    {
        return split(str, token, -1, compress_token);
    }

    template <typename CharT>
    static std::vector<basic_string<CharT>>
        split(const CharT* str, const std::basic_string<CharT>& token,
              bool compress_token = true)
    {
        return split(str, token, -1, compress_token);
    }

    template <typename CharT>
    static std::vector<basic_string<CharT>>
        split(const CharT* str, const CharT* token, bool compress_token = true)
    {
        return split(str, token, -1, compress_token);
    }

    template <typename CharT>
    static basic_string<CharT> lower(std::basic_string<CharT>&& str)
    {
        for (CharT& ch : str)
            if ('A' <= ch && ch <= 'Z')
                ch += 32;

        return str;
    }

    template <typename CharT>
    static basic_string<CharT> upper(std::basic_string<CharT>&& str)
    {
        for (CharT& ch : str)
            if ('a' <= ch && ch <= 'z')
                ch -= 32;

        return str;
    }

    template <typename CharT>
    static basic_string<CharT> lower(const std::basic_string<CharT>& str)
    {
        return lower(std::basic_string<CharT>(str));
    }

    template <typename CharT>
    static basic_string<CharT> upper(const std::basic_string<CharT>& str)
    {
        return upper(std::basic_string<CharT>(str));
    }

    template <typename CharT>
    static basic_string<CharT> lower(const CharT* str)
    {
        return lower(std::basic_string<CharT>(str));
    }

    template <typename CharT>
    static basic_string<CharT> upper(const CharT* str)
    {
        return upper(std::basic_string<CharT>(str));
    }

    template <typename CharT>
    static basic_string<CharT>
        trim(std::basic_string<CharT>&& str, CharT ch = 0x20)
    {
        return triml(trimr(str, ch), ch);
    }

    template <typename CharT>
    static basic_string<CharT>
        triml(std::basic_string<CharT>&& str, CharT ch = 0x20)
    {
        return str.erase(0, str.find_first_not_of(ch));
    }

    template <typename CharT>
    static basic_string<CharT>
        trimr(std::basic_string<CharT>&& str, CharT ch = 0x20)
    {
        return str.erase(str.find_last_not_of(ch) + 1);
    }

    template <typename CharT>
    static basic_string<CharT>
        trim(const std::basic_string<CharT>& str, CharT ch = 0x20)
    {
        return trim(std::basic_string<CharT>(str), ch);
    }

    template <typename CharT>
    static basic_string<CharT>
        triml(const std::basic_string<CharT>& str, CharT ch = 0x20)
    {
        return triml(std::basic_string<CharT>(str), ch);
    }

    template <typename CharT>
    static basic_string<CharT>
        trimr(const std::basic_string<CharT>& str, CharT ch = 0x20)
    {
        return trimr(std::basic_string<CharT>(str), ch);
    }

    template <typename CharT>
    static basic_string<CharT> trim(const CharT* str, CharT ch = 0x20)
    {
        return trim(std::basic_string<CharT>(str), ch);
    }

    template <typename CharT>
    static basic_string<CharT> triml(const CharT* str, CharT ch = 0x20)
    {
        return triml(std::basic_string<CharT>(str), ch);
    }

    template <typename CharT>
    static basic_string<CharT> trimr(const CharT* str, CharT ch = 0x20)
    {
        return trimr(std::basic_string<CharT>(str), ch);
    }

private:
    template <char begin, int count>
    static bool between(char ch)
    {
        return begin <= ch && ch < (begin + count);
    }

    template <int base>
    static bool isfrts(char ch)
    {
        if (base <= 10)
            return between<'0', base>(ch);
        else
            return between<'0', base>(ch)
                || between<'a', base - 10>(ch) || between<'A', base - 10>(ch);
    }

    template <typename Arg>
    static void strsf(strings& strings, const Arg& arg)
    {
        strings.emplace_back(strf(arg));
    }

    template <typename Arg, typename... Others>
    static void strsf(strings& strings, const Arg& arg, const Others&... others)
    {
        strings.emplace_back(strf(arg)); strsf(strings, others...);
    }

    static string format_ss(const char* pattern, const strings& strings);
};

template <> bool stralgo::string::as<bool>(void) const;
template <> float stralgo::string::as<float>(void) const;
template <> double stralgo::string::as<double>(void) const;
template <> long double stralgo::string::as<long double>(void) const;

#endif //stralgo_hpp_20140508
