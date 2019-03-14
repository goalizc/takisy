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
    template <typename Array>
    struct push;

public:
    class bin;
    class hexbin;
    class zlibbin;
    class json;
    class xml;
};

template <template <class...> class Array, typename... T>
struct deserialize::push<Array<T...>>
{
    typedef Array<T...> ArrayType;
    typedef typename ArrayType::value_type ValueType;

    static void apply(ArrayType& array, ValueType&& value)
    {
        array.push_back(std::move(value));
    }
};

#define insert_pusher(SET)                                     \
    template <typename... T>                                   \
    struct deserialize::push<SET<T...>>                        \
    {                                                          \
        typedef SET<T...> ArrayType;                           \
        typedef typename ArrayType::value_type ValueType;      \
                                                               \
        static void apply(ArrayType& array, ValueType&& value) \
        {                                                      \
            array.insert(std::move(value));                    \
        }                                                      \
    };

    insert_pusher(std::set)
    insert_pusher(std::multiset)
    insert_pusher(std::unordered_set)
    insert_pusher(std::unordered_multiset)
#undef insert_pusher

class deserialize::bin
{
    template <typename T, bool reflexible = reflect::info<T>::reflexible>
    class dispatch;

public:
    bin(std::string&& buffer)
        : tell_(0), buffer_(std::move(buffer))
    {}

    bin(const std::string& buffer)
        : tell_(0), buffer_(buffer)
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
    const char* pop(unsigned int n)
    {
        const char* ptr = buffer_.data() + tell_;

        tell_ += n;

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

template <template <class...> class Array, typename... T>
struct deserialize::bin::dispatch<Array<T...>, false>
{
    typedef Array<T...> ArrayType;
    typedef typename ArrayType::value_type ValueType;

    static inline void deserialize(bin* bin, ArrayType& array)
    {
        array.clear();
        for (auto i = bin->deserialize<unsigned int>(); i > 0; --i)
            push<ArrayType>::apply(array, bin->deserialize<ValueType>());
    }
};

template <typename T, std::size_t N>
struct deserialize::bin::dispatch<std::array<T, N>, false>
{
    static inline void deserialize(bin* bin, std::array<T, N>& array)
    {
        for (unsigned int i = 0; i < N; ++i)
            array[i] = bin->deserialize<T>();
    }
};

#define deserialize(MAP)                                                \
    template <typename T1, typename T2>                                 \
    struct deserialize::bin::dispatch<MAP<T1, T2>, false>               \
    {                                                                   \
        static inline void deserialize(bin* bin, MAP<T1, T2>& map)      \
        {                                                               \
            map.clear();                                                \
            for (auto i = bin->deserialize<unsigned int>(); i > 0; --i) \
                map[bin->deserialize<T1>()] = bin->deserialize<T2>();   \
        }                                                               \
    };

    deserialize(std::map)
    deserialize(std::unordered_map)
#undef deserialize

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
        if (unsigned int n = bin->deserialize<unsigned int>())
        {
            if (bin->left() < n)
                bin->tell_ = -1;
            else
                str.assign(bin->pop(n), n);
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
            endian_type<T, etLittleEndian> value(0);                \
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
        T value {};
        deserialize(value);
        return std::move(value);
    }

    template <typename T>
    inline void deserialize(T& value)
    {
        if (json_.type() != ::json::vtUndefined)
            dispatch<T>::deserialize(this, value);
    }

private:
    ::json json_;
};

template <typename Reflect>
struct deserialize::json::dispatch<Reflect, true>
{
    static inline void deserialize(json* obj, Reflect& reflect)
    {
        reflect::for_each(reflect, *obj);
    }
};

template <template <class...> class Array, typename... T>
struct deserialize::json::dispatch<Array<T...>, false>
{
    typedef Array<T...> ArrayType;
    typedef typename ArrayType::value_type ValueType;

    static inline void deserialize(json* obj, ArrayType& array)
    {
        array.clear();
        for (unsigned int n = obj->json_.size(), i = 0; i < n; ++i)
            push<ArrayType>::apply(
                array,
                json(obj->json_[i]).deserialize<ValueType>()
            );
    }
};

template <typename T, std::size_t N>
struct deserialize::json::dispatch<std::array<T, N>, false>
{
    static inline void deserialize(json* obj, std::array<T, N>& array)
    {
        for (unsigned int n = obj->json_.size(), i = 0; i < n; ++i)
            array[i] = json(obj->json_[i]).deserialize<T>();
    }
};

#define deserialize(MAP)                                                   \
    template <typename T1, typename T2>                                    \
    struct deserialize::json::dispatch<MAP<T1, T2>, false>                 \
    {                                                                      \
        typedef MAP<T1, T2> MapType;                                       \
        typedef typename MapType::value_type::second_type ValueType;       \
                                                                           \
        static inline void deserialize(json* obj, MapType& map)            \
        {                                                                  \
            map.clear();                                                   \
            for (const std::string& key : obj->json_.keys())               \
                map[key] = json(obj->json_[key]).deserialize<ValueType>(); \
        }                                                                  \
    };

    deserialize(std::map)
    deserialize(std::unordered_map)
#undef deserialize

template <>
struct deserialize::json::dispatch<bool>
{
    static inline void deserialize(json* obj, bool& boolean)
    {
        boolean = obj->json_.as_bool();
    }
};

template <>
struct deserialize::json::dispatch<char>
{
    static inline void deserialize(json* obj, char& character)
    {
        character = obj->json_.as_string()[0];
    }
};

#define deserialize(T)                                       \
    template <>                                              \
    struct deserialize::json::dispatch<T>                    \
    {                                                        \
        static inline void deserialize(json* obj, T& number) \
        {                                                    \
            number = obj->json_.as_double();                 \
        }                                                    \
    };

    deserialize(float)
    deserialize(double)
    deserialize(long double)
#undef deserialize

#define deserialize(T)                                       \
    template <>                                              \
    struct deserialize::json::dispatch<T>                    \
    {                                                        \
        static inline void deserialize(json* obj, T& number) \
        {                                                    \
            number = obj->json_.as_integer();                \
        }                                                    \
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
#undef deserialize

#define deserialize(str_t)                                    \
    template <>                                               \
    struct deserialize::json::dispatch<str_t>                 \
    {                                                         \
        static inline void deserialize(json* obj, str_t& str) \
        {                                                     \
            str = obj->json_.as_string();                     \
        }                                                     \
    };

    deserialize(std::string)
    deserialize(stralgo::string)
#undef deserialize

#define deserialize(ptr_t)                                    \
    template <typename T>                                     \
    struct deserialize::json::dispatch<ptr_t, false>          \
    {                                                         \
        static inline void deserialize(json* obj, ptr_t& ptr) \
        {                                                     \
            if (obj->json_.type() == ::json::vtNull)          \
                ptr = nullptr;                                \
            else                                              \
            {                                                 \
                ptr_t newptr(new T);                          \
                obj->deserialize(*(ptr = newptr));            \
            }                                                 \
        }                                                     \
    };

    deserialize(T*)
    deserialize(std::shared_ptr<T>)
    deserialize(std::unique_ptr<T>)
#undef deserialize

#endif // deserialize_h_20170203
