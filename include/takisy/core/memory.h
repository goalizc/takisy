#ifndef memory_h_20131107
#define memory_h_20131107

#include <new>
#include <cstdlib>
#include <cstring>

class memory
{
public:
    class std
    {
    public:
        template <typename T>
        static T* malloc(unsigned int count)
        {
            return reinterpret_cast<T*>(::malloc(count * sizeof(T)));
        }

        template <typename T>
        static T* calloc(unsigned int count, unsigned int size)
        {
            return reinterpret_cast<T*>(::calloc(count, size));
        }

        template <typename T>
        static T* realloc(void* address, unsigned int count)
        {
            return reinterpret_cast<T*>(::realloc(address, count * sizeof(T)));
        }

        template <typename T>
        static void free(T*& address)
        {
            ::free(address);

            address = nullptr;
        }

        static void* memcpy(void* dest, const void* src, unsigned int count)
        {
            return ::memcpy(dest, src, count);
        }

        static void* memset(void* dest, char ch, unsigned int count)
        {
            return ::memset(dest, ch, count);
        }

        static void* memmove(void* dest, const void* src, unsigned count)
        {
            return ::memmove(dest, src, count);
        }
    };

public:
    template <typename T, typename... ConstructArgs>
    static T* malloc(unsigned int count, const ConstructArgs&... construct_args)
    {
        T* address = std::malloc<T>(count);

        if (address)
            for (unsigned int i = 0; i < count; ++i)
                new (&address[i]) T(construct_args...);

        return address;
    }

    template <typename T>
    static T* malloc(void)
    {
        return malloc<T>(1);
    }

    template <typename T>
    static T* calloc(unsigned int count)
    {
        return std::calloc<T>(count, sizeof(T));
    }

    template <typename T>
    static unsigned int realloc
        (T*& address, unsigned int old_count, unsigned int new_count)
    {
        T* reallocated_address = std::realloc<T>(address, new_count);

        if (!reallocated_address)
            return old_count;
        else
            address = reallocated_address;

        for (unsigned int i = old_count; i < new_count; ++i)
            new (&address[i]) T();

        return new_count;
    }

    template <typename T>
    static void free(T*& address)
    {
        free(address, 1);
    }

    template <typename T>
    static void free(T*& address, unsigned int count)
    {
        for (unsigned int i = 0; i < count; ++i)
            address[i].~T();

        std::free(address);
    }

    template <typename T>
    static void* memmove(T* dest, const T* src, unsigned count)
    {
        return std::memmove(dest, src, count * sizeof(T));
    }

    template <typename T>
    static T* memset(T* dest, const T& t, unsigned int count)
    {
        if (count > 0)
        {
            dest[0] = t;
            unsigned int i = 1;
            while ((i << 1) < count)
                std::memcpy(dest + i, dest, i * sizeof(T)), i <<= 1;
            std::memcpy(dest + i, dest, (count - i) * sizeof(T));
        }

        return dest;
    }
};

#endif //memory_h_20131107
