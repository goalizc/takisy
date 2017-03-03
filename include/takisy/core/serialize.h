#ifndef serialize_h_20170113
#define serialize_h_20170113

#include <cstring>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <takisy/core/reflect.h>
#include <takisy/core/endian_type.h>
#include <takisy/algorithm/zlib.h>
#include <takisy/algorithm/stralgo.h>

struct serialize
{
    class text;
    class bin;
    class json;
    class xml;

private:
    static std::string serialize_string(const std::string& str)
    {
        std::string buffer;

        buffer += '"';
        for (unsigned int i = 0; i < str.size(); ++i)
        {
            switch (str[i])
            {
            default: buffer += str[i]; break;

            case 0x22 /* " */: buffer += "\x5c\x22"; break;
            case 0x5c /* \ */: buffer += "\x5c\x5c"; break;
            case 0x08 /* b */: buffer += "\x5c\x62"; break;
            case 0x0c /* f */: buffer += "\x5c\x66"; break;
            case 0x0a /* n */: buffer += "\x5c\x6e"; break;
            case 0x0d /* r */: buffer += "\x5c\x72"; break;
            case 0x09 /* t */: buffer += "\x5c\x74"; break;
            }
        }
        buffer += '"';

        return std::move(buffer);
    }
};

class serialize::text
{
public:
    text(void)
        : level_(0)
    {}

public:
    inline std::string data(void) const
    {
        return stream_.str();
    }

public:
    template <typename Member>
    inline void operator()(const Member& member)
    {
        stream_ << indent() << member.type << ' ' << member.name << " = ";
        serialize(member.value);
        stream_ << ";\n";
    }

public:
    template <typename Reflect>
    inline text& serialize(const Reflect& reflect)
    {
        stream_ << "reflect " << reflect::info<Reflect>::name << " {\n";
        ++level_;
        reflect::for_each(reflect, *this);
        --level_;
        stream_ << indent() << "}";

        return *this;
    }

    template <template <class...> class Container, typename... T>
    inline text& serialize(const Container<T...>& container)
    {
        if (container.empty())
            stream_ << "{}";
        else
        {
            stream_ << "{\n";
            ++level_;
            for (const auto& item : container)
            {
                stream_ << indent();
                serialize(item);
                stream_ << ",\n";
            }
            --level_;
            stream_ << indent() << "}";
        }

        return *this;
    }

    template <typename... T>
    inline text& serialize(const std::pair<T...>& pair)
    {
        serialize(pair.first);
        stream_ << ": ";
        serialize(pair.second);

        return *this;
    }

    inline text& serialize(char ch)
    {
        return stream_ << '\'' << ch << '\'', *this;
    }

#define serialize(type)                                \
    inline text& serialize(type value)                 \
    {                                                  \
        return stream_ << stralgo::strf(value), *this; \
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

#define serialize(type)                                    \
    inline text& serialize(type string)                    \
    {                                                      \
        return stream_ << serialize_string(string), *this; \
    }

    serialize(const char*)
    serialize(const std::string&)
    serialize(const stralgo::string&)
#undef serialize

#define serialize(ptr_t)                        \
    template <typename T>                       \
    inline text& serialize(ptr_t const& ptr)    \
    {                                           \
        if (ptr)                                \
            stream_ << "new ", serialize(*ptr); \
        else                                    \
            stream_ << "nullptr";               \
                                                \
        return *this;                           \
    }

    serialize(T*)
    serialize(std::shared_ptr<T>)
    serialize(std::unique_ptr<T>)
#undef serialize

private:
    inline std::string indent(void) const
    {
        return std::string(level_ * 4, 0x20);
    }

private:
    unsigned int level_;
    std::ostringstream stream_;
};

class serialize::bin
{
public:
    inline std::string data(void) const
    {
        return buffer_;
    }

    inline std::string hexdata(void) const
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

    inline std::string zlibdata(void) const
    {
        return zlib::compress(buffer_.data(), buffer_.size());
    }

public:
    template <typename Member>
    inline void operator()(const Member& member)
    {
        serialize(member.value);
    }

public:
    template <typename Reflect>
    inline bin& serialize(const Reflect& reflect)
    {
        reflect::for_each(reflect, *this);

        return *this;
    }

    template <template <class...> class Container, typename... T>
    inline bin& serialize(const Container<T...>& container)
    {
        serialize((unsigned int)container.size());
        for (const auto& item : container)
            serialize(item);

        return *this;
    }

    template <typename... T>
    inline bin& serialize(const std::pair<T...>& pair)
    {
        serialize(pair.first);
        serialize(pair.second);

        return *this;
    }

    inline bin& serialize(bool value)
    {
        return serialize((unsigned char)value);
    }

    inline bin& serialize(const char* value)
    {
        unsigned int value_length = strlen(value);

        serialize(value_length);
        buffer_.append(value, value_length);

        return *this;
    }

    inline bin& serialize(const std::string& value)
    {
        serialize((unsigned int)value.size());
        buffer_.append(value.data(), value.size());

        return *this;
    }

    inline bin& serialize(const stralgo::string& value)
    {
        return serialize(dynamic_cast<const std::string&>(value));
    }

#define serialize(type)                                                 \
    inline bin& serialize(type value)                                   \
    {                                                                   \
        endian_type<type, etBigEndian> bevalue(value);                  \
        return buffer_.append((char*)&bevalue, sizeof(bevalue)), *this; \
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

#define serialize(type)                                             \
    inline bin& serialize(type value)                               \
    {                                                               \
        return buffer_.append((char*)&value, sizeof(value)), *this; \
    }

    serialize(float)
    serialize(double)
    serialize(long double)
#undef serialize

#define serialize(ptr_t)                    \
    template <typename T>                   \
    inline bin& serialize(ptr_t const& ptr) \
    {                                       \
        serialize((bool)ptr);               \
        if (ptr)                            \
            serialize(*ptr);                \
                                            \
        return *this;                       \
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
    json(unsigned int indent = 0)
        : indent_(indent > 8 ? 8 : indent), level_(0)
    {}

public:
    inline std::string data(void) const
    {
        return buffer_;
    }

public:
    template <typename Member>
    inline void operator()(const Member& member)
    {
        if (member.index > 0)
            buffer_ += ", ";
        if (indent_ > 0)
            buffer_ += '\n', buffer_.append(indent(), 0x20);
        serialize(member.name);
        buffer_ += ": ";
        serialize(member.value);
    }

public:
    template <typename Reflect>
    inline json& serialize(const Reflect& reflect)
    {
        buffer_ += '{';
        ++level_;
        reflect::for_each(reflect, *this);
        --level_;
        if (indent_ > 0)
            buffer_ += '\n', buffer_.append(indent(), 0x20);
        buffer_ += '}';

        return *this;
    }

    template <template <class...> class Array, typename... T>
    inline json& serialize(const Array<T...>& array)
    {
        bool is_first = true;

        buffer_ += '[';
        ++level_;
        for (const auto& item : array)
        {
            if (is_first)
                is_first = false;
            else
                buffer_ += ", ";
            if (indent_ > 0)
                buffer_ += '\n', buffer_.append(indent(), 0x20);
            serialize(item);
        }
        --level_;
        if (indent_ > 0 && !array.empty())
            buffer_ += '\n', buffer_.append(indent(), 0x20);
        buffer_ += ']';

        return *this;
    }

    template <typename... T>
    inline json& serialize(const std::map<T...>& dict)
    {
        bool is_first = true;

        buffer_ += '{';
        ++level_;
        for (const auto& pair : dict)
        {
            if (is_first)
                is_first = false;
            else
                buffer_ += ", ";
            if (indent_ > 0)
                buffer_ += '\n', buffer_.append(indent(), 0x20);
            serialize(stralgo::strf(pair.first));
            buffer_ += ": ";
            serialize(pair.second);
        }
        --level_;
        if (indent_ > 0 && !dict.empty())
            buffer_ += '\n', buffer_.append(indent(), 0x20);
        buffer_ += '}';

        return *this;
    }

    inline json& serialize(char value)
    {
        return buffer_ += serialize_string(stralgo::strf(value)), *this;
    }

#define serialize(type)                                \
    inline json& serialize(type value)                 \
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

#define serialize(type)                                 \
    inline json& serialize(type str)                    \
    {                                                   \
        return buffer_ += serialize_string(str), *this; \
    }

    serialize(const char*)
    serialize(const std::string&)
    serialize(const stralgo::string&)
#undef serialize

#define serialize(ptr_t)                     \
    template <typename T>                    \
    inline json& serialize(ptr_t const& ptr) \
    {                                        \
        if (ptr)                             \
            serialize(*ptr);                 \
        else                                 \
            buffer_ += "null";               \
                                             \
        return *this;                        \
    }

    serialize(T*)
    serialize(std::shared_ptr<T>)
    serialize(std::unique_ptr<T>)
#undef serialize

private:
    inline unsigned int indent(void) const
    {
        return level_ * indent_;
    }

private:
    std::string buffer_;
    unsigned int indent_, level_;
};

#endif // serialize_h_20170113
