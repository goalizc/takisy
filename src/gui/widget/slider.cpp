#include <takisy/gui/widget/slider.h>

class slider::implement
{
    friend class slider;
    friend class vertical_slider;
    friend class horizontal_slider;

public:
    implement(double min, double max)
        : dragging_(false), radius_(6), min_(min), max_(max), value_(min)
    {}

public:
    inline double value(double position, double length) const
    {
        if (length < radius_ * 2)
            return min_;
        else
            length -= radius_ * 2;

        return (position - radius_) / length * (max_ - min_) + min_;
    }

    inline double position(double length) const
    {
        return (value_ - min_) * (length - radius_ * 2) / (max_ - min_);
    }

private:
    bool dragging_;
    double radius_;
    double min_, max_, value_;
};

slider::slider(void)
    : slider(0, 100)
{}

slider::slider(double min, double max)
    : slider(min, max, min)
{}

slider::slider(double min, double max, double _value)
    : impl_(new implement(min, max))
{
    value(_value);
}

slider::~slider(void)
{
    delete impl_;
}

double slider::min(void) const
{
    return impl_->min_;
}

double slider::max(void) const
{
    return impl_->max_;
}

double slider::value(void) const
{
    return impl_->value_;
}

double slider::radius(void) const
{
    return impl_->radius_;
}

void slider::range(double min, double max)
{
    if (impl_->min_ == min && impl_->max_ == max)
        return;

    if (impl_->min_ != min)
        impl_->min_ = min;
    if (impl_->max_ != max)
        impl_->max_ = max;

    value(value());
    repaint();
}

void slider::min(double min)
{
    range(min, max());
}

void slider::max(double max)
{
    range(min(), max);
}

void slider::value(double value)
{
    value = algorithm::clamp(value, impl_->min_, impl_->max_);

    if (impl_->value_ != value)
    {
        impl_->value_ = value;
        onSlideHandle();
        repaint();
    }
}

void slider::radius(double radius)
{
    radius = algorithm::clamp(radius, 1.0, 24.0);

    if (impl_->radius_ != radius)
    {
        impl_->radius_ = radius;
        repaint();
    }
}

void slider::home(void)
{
    value(min());
}

void slider::end(void)
{
    value(max());
}

bool slider::onMouseUp(sys::Button button, Point point)
{
    impl_->dragging_ = false;
    capture(false);

    return true;
}

bool slider::onMouseDown(sys::Button button, int times, Point point)
{
    if (button != sys::btnLeft)
        return false;

    impl_->dragging_ = true;
    capture(true);
    onMouseMove(point);

    return true;
}

void vertical_slider::onPaint(graphics graphics, Rect rect)
{
    double left = (width() - impl_->radius_) / 2, right = left + impl_->radius_;
    double position = impl_->position(height()) + impl_->radius_;
    double half_radius = impl_->radius_ / 2;
    color color = color_scheme().theme();

    graphics.fill_round_rectangle
        (left, 0, right, height(), half_radius, color * 50);
    graphics.fill_round_rectangle
        (left, 0, right, position, half_radius, color * 150);

    graphics.fill_circle(width() / 2, position, impl_->radius_, color);
}

bool vertical_slider::onMouseMove(Point point)
{
    if (impl_->dragging_)
        value(impl_->value(point.y, height()));

    return true;
}

void horizontal_slider::onPaint(graphics graphics, Rect rect)
{
    double top = (height() - impl_->radius_) / 2, bottom = top + impl_->radius_;
    double position = impl_->position(width()) + impl_->radius_;
    double half_radius = impl_->radius_ / 2;
    color color = color_scheme().theme();

    graphics.fill_round_rectangle
        (0, top, width(),  bottom, half_radius, color * 50);
    graphics.fill_round_rectangle
        (0, top, position, bottom, half_radius, color * 150);

    graphics.fill_circle(position, height() / 2, impl_->radius_, color);
}

bool horizontal_slider::onMouseMove(Point point)
{
    if (impl_->dragging_)
        value(impl_->value(point.x, width()));

    return true;
}
