#ifndef size_h_20130722
#define size_h_20130722

#include <takisy/cgl/basic/point.h>

template <typename T>
struct t_size
{
    typedef t_size self_type;
    typedef T      value_type;

    value_type width, height;

public:
    inline t_size(void)
        : width(0), height(0)
    {}

    inline t_size(value_type width, value_type height)
        : width(width), height(height)
    {}

    template <typename TT, typename T3>
    inline t_size(const t_point<TT>& lt, const t_point<T3>& rb)
        : width(rb.x - lt.x), height(rb.y - lt.y)
    {}

    template <typename Size>
    inline t_size(const Size& size)
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
    inline bool operator==(const t_size<TT>& size) const
    {
        return width == size.width && height == size.height;
    }

    template <typename TT>
    inline bool operator!=(const t_size<TT>& size) const
    {
        return width != size.width || height != size.height;
    }

public:
    inline void expand(value_type dx, value_type dy)
    {
        width  += dx;
        height += dy;
    }

public:
    template <typename NumbericType>
    inline self_type operator*(NumbericType ratio)
    {
        return self_type(width * ratio, height * ratio);
    }

    template <typename NumbericType>
    inline self_type operator/(NumbericType ratio)
    {
        return self_type(width / ratio, height / ratio);
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
inline t_size<T> operator*(NumbericType ratio, t_size<T> size)
{
    return size * ratio;
}

typedef t_size<int>          size;
typedef t_size<unsigned int> sizeu;
typedef t_size<double>       sizef;

#endif //size_h_20130722
