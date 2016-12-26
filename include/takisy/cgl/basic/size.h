#ifndef size_h_20130722
#define size_h_20130722

#include <takisy/cgl/basic/point.h>

template <typename T>
struct base_size
{
    typedef base_size self_type;
    typedef T         value_type;

    value_type width, height;

public:
    inline base_size(void)
        : width(0), height(0)
    {}

    inline base_size(value_type width, value_type height)
        : width(width), height(height)
    {}

    template <typename TT, typename TTT>
    inline base_size(const base_point<TT>& lt, const base_point<TTT>& rb)
        : width(rb.x - lt.x), height(rb.y - lt.y)
    {}

    template <typename Size>
    inline base_size(const Size& size)
        : width(size.width), height(size.height)
    {}

    inline self_type& operator=(const self_type& size)
    {
        if (this != &size)
        {
            width  = size.width;
            height = size.height;
        }

        return *this;
    }

    template <typename Size>
    inline self_type& operator=(const Size& size)
    {
        width  = size.width;
        height = size.height;

        return *this;
    }

public:
    template <typename TT>
    inline bool operator==(const base_size<TT>& size) const
    {
        return width == size.width && height == size.height;
    }

    template <typename TT>
    inline bool operator!=(const base_size<TT>& size) const
    {
        return width != size.width || height != size.height;
    }

public:
    inline value_type area(void) const
    {
        return width * height;
    }

public:
    inline void expand(value_type dx, value_type dy)
    {
        width  += dx;
        height += dy;
    }

    template <typename TT>
    inline void expand(const base_size<TT>& dsize)
    {
        width  += dsize.width;
        height += dsize.height;
    }

public:
    template <typename TT>
    inline self_type operator+(const base_size<TT>& size) const
    {
        return self_type(width + size.width, height + size.height);
    }

    template <typename TT>
    inline self_type operator-(const base_size<TT>& size) const
    {
        return self_type(width - size.width, height - size.height);
    }

    template <typename NumbericType>
    inline self_type operator*(NumbericType ratio) const
    {
        return self_type(width * ratio, height * ratio);
    }

    template <typename NumbericType>
    inline self_type operator/(NumbericType ratio) const
    {
        return self_type(width / ratio, height / ratio);
    }

    template <typename TT>
    inline self_type& operator+=(const base_size<TT>& size)
    {
        width  += size.width;
        height += size.height;

        return *this;
    }

    template <typename TT>
    inline self_type& operator-=(const base_size<TT>& size)
    {
        width  -= size.width;
        height -= size.height;

        return *this;
    }

    template <typename NumbericType>
    inline self_type& operator*=(NumbericType ratio)
    {
        width  *= ratio;
        height *= ratio;

        return *this;
    }

    template <typename NumbericType>
    inline self_type& operator/=(NumbericType ratio)
    {
        width  /= ratio;
        height /= ratio;

        return *this;
    }
};

template <typename NumbericType, typename T>
inline base_size<T> operator*(NumbericType ratio, base_size<T> size)
{
    return size * ratio;
}

typedef base_size<int>          size;
typedef base_size<unsigned int> sizeu;
typedef base_size<double>       sizef;

#endif //size_h_20130722
