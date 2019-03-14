#ifndef algo_h_20130815
#define algo_h_20130815

#include <algorithm>

class algo
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
        std::swap(_1, _2);
    }

    template <typename T>
    static void sort(T vector, int size)
    {
        std::sort(vector, vector + size);
    }
};

#endif //algo_h_20130815
