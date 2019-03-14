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
        double operator()(double x, double y) const
        {
            return x;
        }
    };

    struct y
    {
        double operator()(double x, double y) const
        {
            return y;
        }
    };

    struct circle
    {
        double operator()(double x, double y) const
        {
            return math::hypot(x, y);
        }
    };

    struct minxy
    {
        double operator()(double x, double y) const
        {
            return algo::min(math::abs(x), math::abs(y));
        }
    };

    struct maxxy
    {
        double operator()(double x, double y) const
        {
            return algo::max(math::abs(x), math::abs(y));
        }
    };

    struct diamond
    {
        double operator()(double x, double y) const
        {
            return math::abs(x) + math::abs(y);
        }
    };

    struct sqrtxy
    {
        double operator()(double x, double y) const
        {
            return math::sqrt(math::abs(x) * math::abs(y));
        }
    };

    struct conic
    {
        double operator()(double x, double y) const
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
        double operator()(double x, double y) const
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
        double operator()(double gradient) const
        {
            return gradient;
        }
    };

    class wave
    {
    public:
        wave(void)
            : wave(1)
        {}

        wave(double n)
            : n_(n), nx2_(n_ * 2)
        {}

    public:
        double operator()(double gradient) const
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
        void operator()(double& x, double& y) const {}
    };

    struct affine : public transformer
    {
    public:
        void operator()(double& x, double& y) const
        {
            std::move(point_type<double&>(x, y)) = transform(x, y);
        }
    };
};

/*
 * Gradient framework
 */

template <typename C,
          typename T = transform::null,
          typename D = decorate::null>
struct gradient
{
    typedef T transform_type;
    typedef C calculate_type;
    typedef D decorate_type;

    calculate_type calculate;
    transform_type transform;
    decorate_type  decorate;

public:
    gradient(void)
    {}

    gradient(const calculate_type& calculate)
        : calculate(calculate)
    {}

    gradient(const calculate_type& calculate, const transform_type& transform)
        : calculate(calculate), transform(transform)
    {}

    gradient(const calculate_type& calculate, const decorate_type& decorate)
        : calculate(calculate), decorate(decorate)
    {}

    gradient(const calculate_type& calculate, const transform_type& transform,
             const decorate_type& decorate)
        : calculate(calculate), transform(transform), decorate(decorate)
    {}

public:
    double value(double x, double y) const
    {
        transform(x, y);

        return decorate(calculate(x, y));
    }
};

#endif //gradient_h_20131112
