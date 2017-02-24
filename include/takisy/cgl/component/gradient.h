#ifndef gradient_h_20131112
#define gradient_h_20131112

#include <takisy/core/math.h>
#include <takisy/core/algo.h>
#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/component/transformer.h>

/*
 * Calculate
 */

struct calculate
{
    struct x
    {
        inline double operator()(double x, double y) const
        {
            return x;
        }
    };

    struct y
    {
        inline double operator()(double x, double y) const
        {
            return y;
        }
    };

    struct circle
    {
        inline double operator()(double x, double y) const
        {
            return math::sqrt(x * x + y * y);
        }
    };

    struct minxy
    {
        inline double operator()(double x, double y) const
        {
            return algo::min(math::abs(x), math::abs(y));
        }
    };

    struct maxxy
    {
        inline double operator()(double x, double y) const
        {
            return algo::max(math::abs(x), math::abs(y));
        }
    };

    struct diamond
    {
        inline double operator()(double x, double y) const
        {
            return math::abs(x) + math::abs(y);
        }
    };

    struct sqrtxy
    {
        inline double operator()(double x, double y) const
        {
            return math::sqrt(math::abs(x) * math::abs(y));
        }
    };

    struct conic
    {
        inline double operator()(double x, double y) const
        {
            static constexpr double pix2 = math::pi * 2;

            double angle = math::atan2(x, y);
            if (angle < 0)
                angle += pix2;

            return angle;
        }
    };

    struct conic0to1
    {
        inline double operator()(double x, double y) const
        {
            static constexpr double rpix2 = 1 / (math::pi * 2);

            double angle = math::atan2(x, y) * rpix2;
            if (angle < 0)
                angle += 1;

            return angle;
        }
    };
};

/*
 * Decorate
 */

struct decorate
{
    struct null
    {
        inline double operator()(double gradient) const
        {
            return gradient;
        }
    };

    class wave
    {
    public:
        inline wave(void)
            : wave(1)
        {}

        inline wave(double n)
            : n_(n), nx2_(n_ * 2)
        {}

    public:
        inline double operator()(double gradient) const
        {
            gradient = math::fmod(math::abs(gradient), nx2_);
            if (gradient > n_)
                gradient = nx2_ - gradient;

            return gradient;
        }

    private:
        double n_, nx2_;
    };
};

/*
 * Transform
 */

struct transform
{
    struct null
    {
        inline void operator()(double& x, double& y) const {}
    };

    struct affine : private transformer
    {
    public:
        inline void set(pointf center, double radius, double angle)
        {
            reset().offset(-center).scale(1 / radius).rotate(-angle);
        }

        inline void set(double cx, double cy, double radius, double angle)
        {
            reset().offset(-cx, -cy).scale(1 / radius).rotate(-angle);
        }

    public:
        inline void operator()(double& x, double& y) const
        {
            std::move(point_type<double&>(x, y)) = transform(x, y);
        }
    };
};

/*
 * Gradient framework
 */

template <typename Calculate,
          typename Decorate  = decorate::null,
          typename Transform = transform::null>
struct gradient
{
    typedef Calculate calculate_type;
    typedef Decorate  decorate_type;
    typedef Transform transform_type;

    calculate_type calculate;
    decorate_type  decorate;
    transform_type transform;

public:
    inline gradient(void)
    {}

    inline gradient(const calculate_type& calculate)
        : calculate(calculate)
    {}

    inline gradient(const calculate_type& calculate,
                    const decorate_type&  decorate,
                    const transform_type& transform)
        : calculate(calculate), decorate(decorate), transform(transform)
    {}

public:
    inline double value(double x, double y) const
    {
        return transform(x, y), decorate(calculate(x, y));
    }
};

#endif //gradient_h_20131112
