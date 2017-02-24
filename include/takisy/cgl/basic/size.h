#ifndef size_h_20130722
#define size_h_20130722

#include <takisy/cgl/basic/point.h>

template <typename T>
struct size_type
{
    typedef size_type self_type;
    typedef T         value_type;

    value_type width, height;

public:
    inline size_type(void)
        : width(0), height(0)
    {}

    inline size_type(value_type width, value_type height)
        : width(width), height(height)
    {}

    template <typename TT, typename TTT>
    inline size_type(const point_type<TT>& lt, const point_type<TTT>& rb)
        : width(rb.x - lt.x), height(rb.y - lt.y)
    {}

    template <typename Size>
    inline size_type(const Size& size)
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
    inline bool operator==(const size_type<TT>& size) const
    {
        return width == size.width && height == size.height;
    }

    template <typename TT>
    inline bool operator!=(const size_type<TT>& size) const
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
    inline void expand(const size_type<TT>& dsize)
    {
        width  += dsize.width;
        height += dsize.height;
    }

public:
    template <typename TT>
    inline self_type operator+(const size_type<TT>& size) const
    {
        return self_type(width + size.width, height + size.height);
    }

    template <typename TT>
    inline self_type operator-(const size_type<TT>& size) const
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
    inline self_type& operator+=(const size_type<TT>& size)
    {
        width  += size.width;
        height += size.height;

        return *this;
    }

    template <typename TT>
    inline self_type& operator-=(const size_type<TT>& size)
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
inline size_type<T> operator*(NumbericType ratio, size_type<T> size)
{
    return size * ratio;
}

typedef size_type<int>          size;
typedef size_type<unsigned int> sizeu;
typedef size_type<double>       sizef;

#endif //size_h_20130722
