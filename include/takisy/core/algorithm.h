#ifndef algorithm_h_20130815
#define algorithm_h_20130815

#include <algorithm>

class algorithm
{
public:
    template <typename T>
    static inline T min(const T& _1, const T& _2)
    {
        return std::min(_1, _2);
    }

    template <typename T>
    static inline T max(const T& _1, const T& _2)
    {
        return std::max(_1, _2);
    }

    template <typename T>
    static inline T clamp(const T& n, const T& aside, const T& bside)
    {
        if (aside < bside)
            return min(max(n, aside), bside);
        else
            return min(max(n, bside), aside);
    }

    template <typename T>
    static inline void swap(T& _1, T& _2)
    {
    #if 0
        T& foobar = _1; _1 = _2; _2 = foobar;
    #else
        std::swap(_1, _2);
    #endif
    }

    template <typename T>
    static void sort(T vector, int size)
    {
    #if 0
        for (int min, i = 0; i < size - 1; ++i)
        {
            min = i;
            for (int j = i + 1; j < size; ++j)
                if (vector[j] < vector[min])
                    min = j;
            if (i != min)
                swap(vector[i], vector[min]);
        }
    #else
        std::sort(vector, vector + size);
    #endif
    }
};

#endif //algorithm_h_20130815
