#ifndef serialize_h_20170113
#define serialize_h_20170113

#include <cstring>
#include <array>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <takisy/core/reflect.h>
#include <takisy/core/endian_type.h>
#include <takisy/algorithm/zlib.h>
#include <takisy/algorithm/stralgo.h>

struct serialize
{
    class bin;
    class json;
    class xml;
};

class serialize::bin
{
public:
    std::string data(void) const
    {
        return buffer_;
    }

    std::string hexdata(void) const
    {
        static constexpr const char literal[] = "0123456789abcdef";
        std::string hex;

        for (unsigned char c : buffer_)
        {
            hex += literal[c >> 4];
            hex += literal[c &  0x0f];
        }

        return std::move(hex);
    }

    std::string zlibdata(void) const
    {
        return zlib::compress(buffer_.data(), buffer_.size());
    }

public:
    template <typename Member>
    void operator()(const Member& member)
    {
        serialize(member.value);
    }

public:
    template <typename Reflect>
    bin& serialize(const Reflect& reflect)
    {
        reflect::for_each(reflect, *this);

        return *this;
    }

    template <template <class...> class Container, typename... T>
    bin& serialize(const Container<T...>& container)
    {
        serialize((unsigned int)container.size());
        for (auto& item : container)
            serialize(item);

        return *this;
    }

    template <typename T, std::size_t N>
    bin& serialize(const std::array<T, N>& array)
    {
        for (auto& item : array)
            serialize(item);

        return *this;
    }

    template <typename T1, typename T2>
    bin& serialize(const std::pair<T1, T2>& pair)
    {
        serialize(pair.first);
        serialize(pair.second);

        return *this;
    }

    bin& serialize(bool value)
    {
        return serialize((unsigned char)value);
    }

    bin& serialize(const char* value)
    {
        unsigned int len = strlen(value);

        serialize(len);
        buffer_.append(value, len);

        return *this;
    }

    bin& serialize(const std::string& value)
    {
        serialize((unsigned int)value.size());
        buffer_.append(value.data(), value.size());

        return *this;
    }

    bin& serialize(const stralgo::string& value)
    {
        return serialize(dynamic_cast<const std::string&>(value));
    }

#define  serialize(T)                                                   \
    bin& serialize(T value)                                             \
    {                                                                   \
        endian_type<T, etLittleEndian> levalue(value);                  \
        return buffer_.append((char*)&levalue, sizeof(levalue)), *this; \
    }

    serialize(char)
    serialize(signed char)
    serialize(unsigned char)
    serialize(short)
    serialize(unsigned short)
    serialize(int)
    serialize(unsigned int)
    serialize(long)
    serialize(unsigned long)
    serialize(long long)
    serialize(unsigned long long)
#undef serialize

#define  serialize(T)                                               \
    bin& serialize(T value)                                         \
    {                                                               \
        return buffer_.append((char*)&value, sizeof(value)), *this; \
    }

    serialize(float)
    serialize(double)
    serialize(long double)
#undef serialize

#define serialize(ptr_t)             \
    template <typename T>            \
    bin& serialize(ptr_t const& ptr) \
    {                                \
        serialize((bool)ptr);        \
        if (ptr)                     \
            serialize(*ptr);         \
                                     \
        return *this;                \
    }

    serialize(T*)
    serialize(std::shared_ptr<T>)
    serialize(std::unique_ptr<T>)
#undef serialize

private:
    std::string buffer_;
};

class serialize::json
{
public:
    json(unsigned int n = 0)
        : colon_(n ? ": " : ":"), indent_(n < 4 ? n : 4), level_(0)
    {}

public:
    std::string data(void) const
    {
        return buffer_;
    }

public:
    template <typename Member>
    void operator()(const Member& member)
    {
        if (member.index > 0)
            buffer_ += ',';
        if (indent_ > 0)
            buffer_ += '\n', buffer_.append(indent(), 0x20);
        serialize(member.name);
        buffer_ += colon_;
        serialize(member.value);
    }

public:
    template <typename Reflect>
    json& serialize(const Reflect& reflect)
    {
        buffer_ += '{';
        ++level_;
        reflect::for_each(reflect, *this);
        --level_;
        if (indent_ > 0)
        {
            buffer_ += '\n';
            buffer_.append(indent(), 0x20);
        }
        buffer_ += '}';

        return *this;
    }

    template <template <class...> class Array, typename... T>
    json& serialize(const Array<T...>& array)
    {
        return serialize_array(array);
    }

    template <typename T, std::size_t N>
    json& serialize(const std::array<T, N>& array)
    {
        return serialize_array(array);
    }

#define serialize(MAP)                      \
    template <typename T1, typename T2>     \
    json& serialize(const MAP<T1, T2>& map) \
    {                                       \
        return serialize_object(map);       \
    }

    serialize(std::map)
    serialize(std::unordered_map)
#undef serialize

    json& serialize(char value)
    {
        return serialize_string(stralgo::strf(value));
    }

#define   serialize(T)                                 \
    json& serialize(T value)                           \
    {                                                  \
        return buffer_ += stralgo::strf(value), *this; \
    }

    serialize(signed char)
    serialize(unsigned char)
    serialize(short)
    serialize(unsigned short)
    serialize(int)
    serialize(unsigned int)
    serialize(long)
    serialize(unsigned long)
    serialize(long long)
    serialize(unsigned long long)
    serialize(float)
    serialize(double)
    serialize(long double)
    serialize(bool)
#undef serialize

#define   serialize(T)                \
    json& serialize(T str)            \
    {                                 \
        return serialize_string(str); \
    }

    serialize(const char*)
    serialize(const std::string&)
    serialize(const stralgo::string&)
#undef serialize

#define serialize(ptr_t)              \
    template <typename T>             \
    json& serialize(ptr_t const& ptr) \
    {                                 \
        if (ptr)                      \
            serialize(*ptr);          \
        else                          \
            buffer_ += "null";        \
                                      \
        return *this;                 \
    }

    serialize(T*)
    serialize(std::shared_ptr<T>)
    serialize(std::unique_ptr<T>)
#undef serialize

private:
    unsigned int indent(void) const
    {
        return level_ * indent_;
    }

    json& serialize_string(const std::string& str)
    {
        buffer_ += '"';
        for (unsigned int i = 0; i < str.size(); ++i)
        {
            switch (str[i])
            {
            default:           buffer_ += str[i];     break;
            case 0x22 /* " */: buffer_ += "\x5c\x22"; break;
            case 0x5c /* \ */: buffer_ += "\x5c\x5c"; break;
            case 0x08 /* b */: buffer_ += "\x5c\x62"; break;
            case 0x0c /* f */: buffer_ += "\x5c\x66"; break;
            case 0x0a /* n */: buffer_ += "\x5c\x6e"; break;
            case 0x0d /* r */: buffer_ += "\x5c\x72"; break;
            case 0x09 /* t */: buffer_ += "\x5c\x74"; break;
            }
        }
        buffer_ += '"';

        return *this;
    }

    template <typename Array>
    json& serialize_array(const Array& array)
    {
        bool isfirst = true;

        buffer_ += '[';
        ++level_;
        for (auto& item : array)
        {
            if (isfirst)
                isfirst = false;
            else
                buffer_ += ',';
            if (indent_)
            {
                buffer_ += '\n';
                buffer_.append(indent(), 0x20);
            }
            serialize(item);
        }
        --level_;
        if (indent_ && !array.empty())
        {
            buffer_ += '\n';
            buffer_.append(indent(), 0x20);
        }
        buffer_ += ']';

        return *this;
    }

    template <typename Map>
    json& serialize_object(const Map& map)
    {
        bool isfirst = true;

        buffer_ += '{';
        ++level_;
        for (auto& pair : map)
        {
            if (isfirst)
                isfirst = false;
            else
                buffer_ += ',';
            if (indent_)
            {
                buffer_ += '\n';
                buffer_.append(indent(), 0x20);
            }
            serialize(stralgo::strf(pair.first).c_str());
            buffer_ += colon_;
            serialize(pair.second);
        }
        --level_;
        if (indent_ && !map.empty())
        {
            buffer_ += '\n';
            buffer_.append(indent(), 0x20);
        }
        buffer_ += '}';

        return *this;
    }

private:
    const char* colon_;
    std::string buffer_;
    unsigned int indent_, level_;
};

#endif // serialize_h_20170113
