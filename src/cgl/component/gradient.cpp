#include <takisy/core/math.h>
#include <takisy/core/algorithm.h>
#include <takisy/cgl/component/transformer.h>
#include <takisy/cgl/component/gradient.h>

class gradient::implement
{
    friend class gradient;

public:
    implement(pointf::axis_type center_x, pointf::axis_type center_y,
              double radius, double angle, gradient::GradientType gradient_type)
        : center_(center_x, center_y), radius_(radius), angle_(angle)
        , gradient_type_(gradient_type), calculator_(nullptr)
    {
        refresh_transformer();
    }

public:
    inline void center(pointf::axis_type center_x, pointf::axis_type center_y)
    {
        center_.x = center_x;
        center_.y = center_y;

        refresh_transformer();
    }

    inline void radius(double radius)
    {
        radius_ = radius;

        refresh_transformer();
    }

    inline void angle(double angle)
    {
        angle_ = angle;

        refresh_transformer();
    }

    inline double gradual_value(pointf::axis_type x, pointf::axis_type y) const
    {
        pointf foobar = transformer_.transform(x, y);

        switch (gradient_type_)
        {
        case gtLinear:
            return foobar.x;
        case gtCircle:
            return math::sqrt(foobar.x * foobar.x + foobar.y * foobar.y);
        case gtMinXY:
            return algorithm::min(math::abs(foobar.x), math::abs(foobar.y));
        case gtMaxXY:
            return algorithm::max(math::abs(foobar.x), math::abs(foobar.y));
        case gtDiamond:
            return math::abs(foobar.x) + math::abs(foobar.y);
        case gtSqrtXY:
            return math::sqrt(math::abs(foobar.x) * math::abs(foobar.y));
        case gtConic:
            {
                double bar = math::atan2(x - center_.x, y - center_.y);
                if (bar < angle_)
                    bar += math::deg2rad(360);
                return (bar - angle_) / math::deg2rad(360);
            }
        case gtCustom:
            return calculator_ ? calculator_(foobar.x, foobar.y) : 0;
        default:
            return 0;
        }
    }

private:
    inline void refresh_transformer(void)
    {
        transformer_.reset()
                    .offset(-center_)
                    .rotate(-angle_)
                    .scale(1 / radius_);
    }

private:
    pointf center_;
    double radius_;
    double angle_;
    transformer transformer_;
    gradient::GradientType gradient_type_;
    gradient::FnCalculator calculator_;
};

gradient::gradient(void)
    : gradient(0, 0, 100, 0, gtLinear)
{}

gradient::gradient(const pointf& center, double radius, double angle,
                   GradientType gradient_type)
    : gradient(center.x, center.y, radius, angle, gradient_type)
{}

gradient::gradient(pointf::axis_type center_x, pointf::axis_type center_y,
                   double radius, double angle, GradientType gradient_type)
    : impl_(new implement(center_x, center_y, radius, angle, gradient_type))
{}

gradient::gradient(const gradient& _gradient)
    : gradient()
{
    operator=(_gradient);
}

gradient::~gradient(void)
{
    delete impl_;
}

gradient& gradient::operator=(const gradient& gradient)
{
    if (this != &gradient)
    {
        impl_->center_        = gradient.impl_->center_;
        impl_->radius_        = gradient.impl_->radius_;
        impl_->angle_         = gradient.impl_->angle_;
        impl_->gradient_type_ = gradient.impl_->gradient_type_;
        impl_->calculator_    = gradient.impl_->calculator_;

        impl_->refresh_transformer();
    }

    return *this;
}

gradient& gradient::gradient_type(GradientType gradient_type)
{
    impl_->gradient_type_ = gradient_type;

    return *this;
}

gradient& gradient::center(const pointf& center)
{
    impl_->center(center.x, center.y);

    return *this;
}

gradient& gradient::center(pointf::axis_type center_x,
                           pointf::axis_type center_y)
{
    impl_->center(center_x, center_y);

    return *this;
}

gradient& gradient::radius(double radius)
{
    impl_->radius(radius);

    return *this;
}

gradient& gradient::angle(double angle)
{
    impl_->angle(angle);

    return *this;
}

gradient& gradient::custom_calculator(FnCalculator calculator)
{
    impl_->calculator_ = calculator;

    gradient_type(gtCustom);

    return *this;
}

gradient::GradientType gradient::gradient_type(void) const
{
    return impl_->gradient_type_;
}

pointf gradient::center(void) const
{
    return impl_->center_;
}

double gradient::radius(void) const
{
    return impl_->radius_;
}

double gradient::angle(void) const
{
    return impl_->angle_;
}

gradient::FnCalculator gradient::custom_calculator(void) const
{
    return impl_->calculator_;
}

double gradient::gradual_value(const pointf& dott) const
{
    return impl_->gradual_value(dott.x, dott.y);
}

double gradient::gradual_value(double x, double y) const
{
    return impl_->gradual_value(x, y);
}

double gradient::normalized_gradual_value(const pointf& dott) const
{
    double gradual_value = impl_->gradual_value(dott.x, dott.y);
           gradual_value = math::abs(math::fmod(gradual_value, 2));

    return gradual_value > 1 ? 2 - gradual_value : gradual_value;
}

double gradient::normalized_gradual_value(double x, double y) const
{
    return normalized_gradual_value(pointf(x, y));
}
