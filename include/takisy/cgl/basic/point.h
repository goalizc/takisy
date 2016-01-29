#ifndef point_h_20130722
#define point_h_20130722

#include <takisy/core/math.h>

template <typename T>
struct t_point
{
    typedef t_point self_type;
    typedef T       axis_type;

    axis_type x, y;

public:
    inline t_point(void)
        : x(0), y(0)
    {}

    inline t_point(axis_type x, axis_type y)
        : x(x), y(y)
    {}

    template <typename Point>
    inline t_point(const Point& point)
        : x(point.x), y(point.y)
    {}

    inline self_type& operator=(const self_type& point)
    {
        if (this != &point)
        {
            x = point.x;
            y = point.y;
        }

        return *this;
    }

    template <typename Point>
    inline self_type& operator=(const Point& point)
    {
        x = point.x;
        y = point.y;

        return *this;
    }

public:
    template <typename TT>
    inline bool operator==(const t_point<TT>& point) const
    {
        return x == point.x && y == point.y;
    }

    template <typename TT>
    inline bool operator!=(const t_point<TT>& point) const
    {
        return x != point.x || y != point.y;
    }

public:
    static inline self_type polar(double angle, double module)
    {
        return self_type(module * math::cos(angle), module * math::sin(angle));
    }

    static inline self_type rand(axis_type xto, axis_type yto)
    {
        return self_type(math::frand(xto), math::frand(yto));
    }

    static inline self_type rand(axis_type xfrom, axis_type xto,
                                 axis_type yfrom, axis_type yto)
    {
        return self_type(math::frand(xfrom, xto), math::frand(yfrom, yto));
    }

public:
    template <typename TT>
    inline double distance(const t_point<TT>& point) const
    {
        return math::sqrt(math::sqr(x - point.x) + math::sqr(y - point.y));
    }

    inline double distance(axis_type _x, axis_type _y)
    {
        return math::sqrt(math::sqr(x - _x) + math::sqr(y - _y));
    }

public:
    inline self_type operator-(void) const
    {
        return self_type(-x, -y);
    }

    template <typename TT>
    inline self_type operator+(const t_point<TT>& point) const
    {
        return self_type(x + point.x, y + point.y);
    }

    template <typename TT>
    inline self_type operator-(const t_point<TT>& point) const
    {
        return self_type(x - point.x, y - point.y);
    }

    template <typename NumbericType>
    inline self_type operator*(NumbericType ratio) const
    {
        return self_type(x * ratio, y * ratio);
    }

    template <typename NumbericType>
    inline self_type operator/(NumbericType ratio) const
    {
        return self_type(x / ratio, y / ratio);
    }

    template <typename TT>
    inline self_type& operator+=(const t_point<TT>& point)
    {
        x += point.x;
        y += point.y;

        return *this;
    }

    template <typename TT>
    inline self_type& operator-=(const t_point<TT>& point)
    {
        x -= point.x;
        y -= point.y;

        return *this;
    }

    template <typename NumbericType>
    inline self_type& operator*=(NumbericType ratio)
    {
        x *= ratio;
        y *= ratio;

        return *this;
    }

    template <typename NumbericType>
    inline self_type& operator/=(NumbericType ratio)
    {
        x /= ratio;
        y /= ratio;

        return *this;
    }
};

template <typename NumbericType, typename T>
inline t_point<T> operator*(NumbericType ratio, const t_point<T>& point)
{
    return point * ratio;
}

typedef t_point<int>          point;
typedef t_point<unsigned int> pointu;
typedef t_point<double>       pointf;

#endif //point_h_20130722
