#ifndef deserialize_h_20170203
#define deserialize_h_20170203

#include <array>
#include <forward_list>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <takisy/core/reflect.h>
#include <takisy/core/endian_type.h>
#include <takisy/algorithm/zlib.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/parser/json.h>

class deserialize
{
    template <typename Container>
    struct clearer
    {
        static inline void clear(Container& container)
        {
            container.clear();
        }
    };

    template <typename Container>
    struct pusher;

public:
    class bin;
    class hexbin;
    class zlibbin;
    class json;
    class xml;
};

template <typename T, unsigned int N>
struct deserialize::clearer<std::array<T, N>>
{
    static inline void clear(std::array<T, N>& array)
    {}
};

template <template <class...> class Container, typename... T>
struct deserialize::pusher<Container<T...>>
{
    typedef Container<T...> container_type;
    typedef typename container_type::value_type value_type;

    inline void push(container_type& container, value_type&& value)
    {
        container.push_back(std::move(value));
    }
};

template <typename T, unsigned int N>
struct deserialize::pusher<std::array<T, N>>
{
    typedef std::array<T, N> container_type;
    typedef typename container_type::value_type value_type;

    unsigned int index = 0;

    inline void push(container_type& container, value_type&& value)
    {
        container[index++] = value;
    }
};

template <typename... T>
struct deserialize::pusher<std::forward_list<T...>>
{
    typedef std::forward_list<T...> container_type;
    typedef typename container_type::value_type value_type;

    inline void push(container_type& container, value_type&& value)
    {
        container.insert_after(container.end(), value);
    }
};

#define insert_pusher(type)                                             \
    template <typename... T>                                            \
    struct deserialize::pusher<type<T...>>                              \
    {                                                                   \
        typedef type<T...> container_type;                              \
        typedef typename container_type::value_type value_type;         \
                                                                        \
        inline void push(container_type& container, value_type&& value) \
        {                                                               \
            container.insert(std::move(value));                         \
        }                                                               \
    };

    insert_pusher(std::map)
    insert_pusher(std::multimap)
    insert_pusher(std::multiset)
    insert_pusher(std::set)
    insert_pusher(std::unordered_map)
    insert_pusher(std::unordered_multimap)
    insert_pusher(std::unordered_multiset)
    insert_pusher(std::unordered_set)
#undef insert_pusher

class deserialize::bin
{
    template <typename T, bool reflexible = reflect::info<T>::reflexible>
    class dispatch;

public:
    bin(const std::string& buffer)
        : tell_(0), buffer_(buffer)
    {}

    bin(std::string&& buffer)
        : tell_(0), buffer_(std::move(buffer))
    {}

public:
    inline bool eob(void) const
    {
        return tell_ == -1;
    }

    inline unsigned int left(void) const
    {
        return buffer_.size() - tell_;
    }

public:
    template <typename Member>
    inline void operator()(Member&& member)
    {
        deserialize(member.value);
    }

public:
    template <typename T>
    inline T deserialize(void)
    {
        T value;
        dispatch<T>::deserialize(this, value);
        return std::move(value);
    }

    template <typename T>
    inline void deserialize(T& value)
    {
        dispatch<T>::deserialize(this, value);
    }

private:
    const char* pop(unsigned int size)
    {
        const char* ptr = buffer_.data() + tell_;

        tell_ += size;

        return ptr;
    }

private:
    int tell_;
    std::string buffer_;
};

template <typename Reflect>
struct deserialize::bin::dispatch<Reflect, true>
{
    static inline void deserialize(bin* bin, Reflect& reflect)
    {
        reflect::for_each(reflect, *bin);
    }
};

template <template <class...> class Container, typename... T>
struct deserialize::bin::dispatch<Container<T...>, false>
{
    typedef Container<T...> container_type;
    typedef typename container_type::value_type value_type;

    static inline void deserialize(bin* bin, container_type& container)
    {
        clearer<container_type>::clear(container);
        pusher<container_type> pusher;
        for (unsigned int i = bin->deserialize<unsigned int>(); i > 0; --i)
            pusher.push(container, bin->deserialize<value_type>());
    }
};

template <typename T1, typename T2>
struct deserialize::bin::dispatch<std::pair<T1, T2>, false>
{
    typedef std::pair<T1, T2> pair_type;
    typedef typename std::remove_const<T1>::type T3;

    static inline void deserialize(bin* bin, pair_type& pair)
    {
        bin->deserialize(const_cast<T3&>(pair.first));
        bin->deserialize(pair.second);
    }
};

template <>
struct deserialize::bin::dispatch<bool>
{
    static inline void deserialize(bin* bin, bool& boolean)
    {
        boolean = bin->deserialize<unsigned char>();
    }
};

template <>
struct deserialize::bin::dispatch<std::string>
{
    static inline void deserialize(bin* bin, std::string& str)
    {
        unsigned int size = bin->deserialize<unsigned int>();

        if (size)
        {
            if (bin->left() < size)
                bin->tell_ = -1;
            else
                str.assign(bin->pop(size), size);
        }
    }
};

template <>
struct deserialize::bin::dispatch<stralgo::string>
{
    static inline void deserialize(bin* bin, stralgo::string& str)
    {
        bin->deserialize(dynamic_cast<std::string&>(str));
    }
};

#define deserialize(T)                                              \
    template <>                                                     \
    struct deserialize::bin::dispatch<T>                            \
    {                                                               \
        static inline void deserialize(bin* bin, T& number)         \
        {                                                           \
            endian_type<T, etBigEndian> value(0);                   \
                                                                    \
            if (bin->eob())                                         \
                ;                                                   \
            else if (bin->left() < sizeof(T))                       \
                bin->tell_ = -1;                                    \
            else                                                    \
                value.endian_value(*(const T*)bin->pop(sizeof(T))); \
                                                                    \
            number = value;                                         \
        }                                                           \
    };

    deserialize(char)
    deserialize(signed char)
    deserialize(unsigned char)
    deserialize(short)
    deserialize(unsigned short)
    deserialize(int)
    deserialize(unsigned int)
    deserialize(long)
    deserialize(unsigned long)
    deserialize(long long)
    deserialize(unsigned long long)
#undef deserialize

#define deserialize(T)                                      \
    template <>                                             \
    struct deserialize::bin::dispatch<T>                    \
    {                                                       \
        static inline void deserialize(bin* bin, T& number) \
        {                                                   \
            if (bin->eob())                                 \
                number = 0.0;                               \
            else if (bin->left() < sizeof(T))               \
                number = 0.0, bin->tell_ = -1;              \
            else                                            \
                number = *(const T*)bin->pop(sizeof(T));    \
        }                                                   \
    };

    deserialize(float)
    deserialize(double)
    deserialize(long double)
#undef deserialize

#define deserialize(ptr_t)                                   \
    template <typename T>                                    \
    struct deserialize::bin::dispatch<ptr_t, false>          \
    {                                                        \
        static inline void deserialize(bin* bin, ptr_t& ptr) \
        {                                                    \
            if (!bin->deserialize<bool>())                   \
                ptr = nullptr;                               \
            else                                             \
            {                                                \
                ptr_t newptr(new T);                         \
                bin->deserialize(*(ptr = newptr));           \
            }                                                \
        }                                                    \
    };

    deserialize(T*)
    deserialize(std::shared_ptr<T>)
    deserialize(std::unique_ptr<T>)
#undef deserialize

class deserialize::hexbin : public bin
{
public:
    hexbin(const std::string& buffer)
        : bin(unhex(buffer))
    {}

private:
    static std::string unhex(const std::string& hex)
    {
        std::string data;

        for (unsigned int i = 0; i < hex.size(); i += 2)
            data.push_back((unhex(hex[i]) << 4) | unhex(hex[i + 1]));

        return std::move(data);
    }

    static char unhex(char hex)
    {
        switch (hex)
        {
        case '0':           return  0;
        case '1':           return  1;
        case '2':           return  2;
        case '3':           return  3;
        case '4':           return  4;
        case '5':           return  5;
        case '6':           return  6;
        case '7':           return  7;
        case '8':           return  8;
        case '9':           return  9;
        case 'a': case 'A': return 10;
        case 'b': case 'B': return 11;
        case 'c': case 'C': return 12;
        case 'd': case 'D': return 13;
        case 'e': case 'E': return 14;
        case 'f': case 'F': return 15;
        }

        return 0;
    }
};

class deserialize::zlibbin : public bin
{
public:
    zlibbin(const std::string& buffer)
        : bin(zlib::decompress(buffer.data(), buffer.size()))
    {}
};

class deserialize::json
{
    template <typename T, bool reflexible = reflect::info<T>::reflexible>
    class dispatch;

    typedef ::json json_parser;

public:
    template <typename... Args>
    json(Args&&... args)
        : json_(std::forward<Args>(args)...)
    {}

public:
    template <typename Member>
    inline void operator()(Member&& member)
    {
        json(json_[member.name]).deserialize(member.value);
    }

public:
    template <typename T>
    inline T deserialize(void)
    {
        T value;
        deserialize(value);
        return std::move(value);
    }

    template <typename T>
    inline void deserialize(T& value)
    {
        if (json_.type() != json_parser::vtUndefined)
            dispatch<T>::deserialize(this, value);
    }

private:
    json_parser json_;
};

template <typename Reflect>
struct deserialize::json::dispatch<Reflect, true>
{
    static inline void deserialize(json* json, Reflect& reflect)
    {
        reflect::for_each(reflect, *json);
    }
};

template <template <class...> class Container, typename... T>
struct deserialize::json::dispatch<Container<T...>, false>
{
    typedef Container<T...> container_type;
    typedef typename container_type::value_type value_type;

    static inline void deserialize(json* json, container_type& container)
    {
        clearer<container_type>::clear(container);
        pusher<container_type> pusher;
        for (unsigned int size = json->json_.size(), i = 0; i < size; ++i)
            pusher.push(container,
                deserialize::json(json->json_[i]).deserialize<value_type>());
    }
};

template <typename T1, typename T2>
struct deserialize::json::dispatch<std::pair<T1, T2>, false>
{
    typedef std::pair<T1, T2> pair_type;
    typedef typename std::remove_const<T1>::type T3;

    static inline void deserialize(json* json, pair_type& pair)
    {
        const_cast<T3&>(pair.first) = json->json_["key"].as_string();
        deserialize::json(json->json_["value"]).deserialize(pair.second);
    }
};

template <>
struct deserialize::json::dispatch<bool>
{
    static inline void deserialize(json* json, bool& boolean)
    {
        boolean = json->json_.as_bool();
    }
};

template <>
struct deserialize::json::dispatch<char>
{
    static inline void deserialize(json* json, char& character)
    {
        character = json->json_.as_string()[0];
    }
};

#define deserialize(T)                                        \
    template <>                                               \
    struct deserialize::json::dispatch<T>                     \
    {                                                         \
        static inline void deserialize(json* json, T& number) \
        {                                                     \
            number = json->json_.as_double();                 \
        }                                                     \
    };

    deserialize(signed char)
    deserialize(unsigned char)
    deserialize(short)
    deserialize(unsigned short)
    deserialize(int)
    deserialize(unsigned int)
    deserialize(long)
    deserialize(unsigned long)
    deserialize(long long)
    deserialize(unsigned long long)
    deserialize(float)
    deserialize(double)
    deserialize(long double)
#undef deserialize

#define deserialize(str_t)                                     \
    template <>                                                \
    struct deserialize::json::dispatch<str_t>                  \
    {                                                          \
        static inline void deserialize(json* json, str_t& str) \
        {                                                      \
            str = json->json_.as_string();                     \
        }                                                      \
    };

    deserialize(std::string)
    deserialize(stralgo::string)
#undef deserialize

#define deserialize(ptr_t)                                     \
    template <typename T>                                      \
    struct deserialize::json::dispatch<ptr_t, false>           \
    {                                                          \
        static inline void deserialize(json* json, ptr_t& ptr) \
        {                                                      \
            if (json->json_.type() == json_parser::vtNull)     \
                ptr = nullptr;                                 \
            else                                               \
            {                                                  \
                ptr_t newptr(new T);                           \
                json->deserialize(*(ptr = newptr));            \
            }                                                  \
        }                                                      \
    };

    deserialize(T*)
    deserialize(std::shared_ptr<T>)
    deserialize(std::unique_ptr<T>)
#undef deserialize

#endif // deserialize_h_20170203
