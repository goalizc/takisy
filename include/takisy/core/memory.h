#ifndef memory_h_20131107
#define memory_h_20131107

#include <cstdlib>
#include <cstring>
#include <map>
#include <new>

class memory
{
public:
    class std
    {
    public:
        template <typename T>
        static T* malloc(unsigned int count)
        {
            void* address = ::malloc(count * sizeof(T));

            if (address)
                alloced_memories_[address] = count;

            return reinterpret_cast<T*>(address);
        }

        template <typename T>
        static T* calloc(unsigned int count)
        {
            void* address = ::calloc(count, sizeof(T));

            if (address)
                alloced_memories_[address] = count;

            return reinterpret_cast<T*>(address);
        }

        template <typename T>
        static T* realloc(T* address, unsigned int count)
        {
            if (alloced_memories_.find(address) == alloced_memories_.end())
                return nullptr;

            unsigned int oldcount = alloced_memories_[address];
            if (count <= oldcount)
            {
                alloced_memories_[address] = count;
                return address;
            }

            void* realloced_address = ::realloc(address, count * sizeof(T));
            if (realloced_address)
            {
                alloced_memories_.erase(address);
                alloced_memories_[realloced_address] = count;
            }

            return reinterpret_cast<T*>(realloced_address);
        }

        static void free(void* address)
        {
            alloced_memories_.erase(address);
            ::free(address);
        }

        template <typename T>
        static T* memset(T* address, const T& t, unsigned int count)
        {
            if (count == 0)
                return address;

            unsigned int i = 1;

            address[0] = t;
            while ((i << 1) < count)
                memcpy(address + i, address, i * sizeof(T)), i <<= 1;
            memcpy(address + i, address, (count - i) * sizeof(T));

            return address;
        }
    };

public:
    template <typename T>
    static T* malloc(unsigned int count)
    {
        T* address = std::malloc<T>(count);

        if (address)
            new (address) T [count];

        return address;
    }

    template <typename T, typename... Args>
    static T* malloc(unsigned int count, const Args&... args)
    {
        T* address = std::malloc<T>(count);

        if (address)
            for (unsigned int i = 0; i < count; ++i)
                new (&address[i]) T(args...);

        return address;
    }

    template <typename T>
    static T* realloc(T* address, unsigned int count)
    {
        if (alloced_memories_.find(address) == alloced_memories_.end())
            return nullptr;

        unsigned int oldcount = alloced_memories_[address];
        if (count <= oldcount)
        {
            alloced_memories_[address] = count;

            for (unsigned int i = count; i < oldcount; ++i)
                address[i].~T();

            return address;
        }

        T* realloced_address =
            reinterpret_cast<T*>(::realloc(address, count * sizeof(T)));
        if (realloced_address)
        {
            alloced_memories_.erase(address);
            new (realloced_address + oldcount) T [count - oldcount];
            alloced_memories_[realloced_address] = count;
        }

        return realloced_address;
    }

    template <typename T, typename... Args>
    static T* realloc(T* address, unsigned int count, const Args&... args)
    {
        if (alloced_memories_.find(address) == alloced_memories_.end())
            return nullptr;

        unsigned int oldcount = alloced_memories_[address];
        if (count <= oldcount)
        {
            alloced_memories_[address] = count;

            for (unsigned int i = count; i < oldcount; ++i)
                address[i].~T();

            return address;
        }

        T* realloced_address =
            reinterpret_cast<T*>(::realloc(address, count * sizeof(T)));
        if (realloced_address)
        {
            alloced_memories_.erase(address);

            for (unsigned int i = oldcount; i < count; ++i)
                new (&realloced_address[i]) T(args...);

            alloced_memories_[realloced_address] = count;
        }

        return realloced_address;
    }

    template <typename T>
    static void free(T* address)
    {
        if (alloced_memories_.find(address) != alloced_memories_.end())
        {
            unsigned int count = alloced_memories_[address];

            for (unsigned int i = 0; i < count; ++i)
                address[i].~T();
        }

        std::free(address);
    }

private:
    static ::std::map<void*, unsigned int> alloced_memories_;
};

#endif //memory_h_20131107
