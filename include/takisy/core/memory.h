#ifndef memory_h_20131107
#define memory_h_20131107

#include <new>
#include <cstdlib>
#include <cstring>

class memory
{
public:
    template <typename ElementType> class smart_ptr;

public:
    template <typename T>
    static inline T* _malloc(unsigned int count)
    {
        return reinterpret_cast<T*>(::malloc(count * sizeof(T)));
    }

    template <typename T>
    static inline T* malloc(void)
    {
        return malloc<T>(1);
    }

    template <typename T, typename... ConstructArgs>
    static inline T* malloc_init(const ConstructArgs&... construct_args)
    {
        T* address = _malloc<T>(1);

        if (address)
            new (address) T(construct_args...);

        return address;
    }

    template <typename T>
    static T* malloc(unsigned int count)
    {
        T* address = _malloc<T>(count);

        if (address)
            for (unsigned int i = 0; i < count; ++i)
                new (&address[i]) T();

        return address;
    }

    template <typename T>
    static inline T* _calloc(unsigned int count, unsigned int size)
    {
        return reinterpret_cast<T*>(::calloc(count, size));
    }

    template <typename T>
    static inline T* calloc(unsigned int count)
    {
        return _calloc<T>(count, sizeof(T));
    }

    template <typename T>
    static inline T* _realloc(void* address, unsigned int count)
    {
        return reinterpret_cast<T*>(::realloc(address, count * sizeof(T)));
    }

    template <typename T>
    static unsigned int realloc
        (T*& address, unsigned int old_count, unsigned int new_count)
    {
        T* reallocated_address = _realloc<T>(address, new_count);

        if (!reallocated_address)
            return old_count;
        else
            address = reallocated_address;

        for (unsigned int i = old_count; i < new_count; ++i)
            new (&address[i]) T();

        return new_count;
    }

    template <typename T>
    static inline void _free(T*& address)
    {
        ::free(address);

        address = nullptr;
    }

    template <typename T>
    static inline void free(T*& address)
    {
        free(address, 1);
    }

    template <typename T>
    static void free(T*& address, unsigned int count)
    {
        for (unsigned int i = 0; i < count; ++i)
            address[i].~T();

        _free(address);
    }

    static inline void* _memcpy(void* dest, const void* src, unsigned int count)
    {
        return ::memcpy(dest, src, count);
    }

    static inline void* _memset(void* dest, char ch, unsigned int count)
    {
        return ::memset(dest, ch, count);
    }

    static inline void* _memmove(void* dest, const void* src, unsigned count)
    {
        return ::memmove(dest, src, count);
    }

    template <typename T>
    static inline void* memmove(T* dest, const T* src, unsigned count)
    {
        return _memmove(dest, src, count * sizeof(T));
    }

    template <typename T>
    static inline T* memset(T* dest, const T& t, unsigned int count)
    {
        if (count > 0)
        {
            dest[0] = t;
            unsigned int i = 1;
            while ((i << 1) < count)
                _memcpy(dest + i, dest, i * sizeof(T)), i <<= 1;
            _memcpy(dest + i, dest, (count - i) * sizeof(T));
        }

        return dest;
    }
};

#endif //memory_h_20131107
