#include <takisy/gui/widget/slider.h>

class slider::implement
{
    friend class slider;
    friend class vertical_slider;
    friend class horizontal_slider;

public:
    implement(double min, double max)
        : min_(min), max_(max), value_(min)
        , scrolling_(std::make_tuple(false, 0, Point(0, 0)))
    {}

public:
    double foo(double length) const
    {
        return (value_ - min_) * (length - 5) / (max_ - min_) + 2;
    }

private:
    double min_, max_, value_;
    std::tuple<bool, double, Point> scrolling_;
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
    attribute("intercept.onClick", true);

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

void slider::range(double _min, double _max)
{
    min(_min);
    max(_max);
}

void slider::min(double min)
{
    if (impl_->min_ != min)
    {
        impl_->min_ = min;
        value(value());
        repaint();
    }
}

void slider::max(double max)
{
    if (impl_->max_ != max)
    {
        impl_->max_ = max;
        value(value());
        repaint();
    }
}

void slider::value(double value)
{
    if (value > impl_->max_)
        value = impl_->max_;
    if (value < impl_->min_)
        value = impl_->min_;

    if (impl_->value_ != value)
    {
        impl_->value_ = value;
        onSlide();
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

bool slider::onMouseUp(sys::MouseButton button, Point point)
{
    std::get<0>(impl_->scrolling_) = false;
    capture(false);

    return true;
}

void vertical_slider::onPaint(graphics graphics, Rect rect)
{
    double left = ((int)width() - 3) / 2, right = left + 3;
    double bar  = impl_->foo(height());
    double top  = bar - 2, bottom = bar + 3;

    graphics.fill_rectangle(left, 0, right, height(), color_scheme()->main());
    graphics.fill_rectangle(left - 3, top, right + 3, bottom,
                            color_scheme()->cool());
}

bool vertical_slider::onMouseDown(sys::MouseButton button, int times,
                                  Point point)
{
    if (button != sys::mbLButton)
        return false;

    double y = impl_->foo(height());

    if (y - 2 <= point.y && point.y < y + 3)
    {
        std::get<0>(impl_->scrolling_) = true;
        std::get<1>(impl_->scrolling_) = value();
        std::get<2>(impl_->scrolling_) = point;
        capture(true);
    }

    return true;
}

bool vertical_slider::onMouseMove(Point point)
{
    if (std::get<0>(impl_->scrolling_))
    {
        double delta;

        delta = ((double)point.y - std::get<2>(impl_->scrolling_).y) / height();
        delta *= impl_->max_ - impl_->min_;
        value(std::get<1>(impl_->scrolling_) + delta);
    }

    return true;
}

void horizontal_slider::onPaint(graphics graphics, Rect rect)
{
    double top  = ((int)height() - 3) / 2, bottom = top + 3;
    double bar  = impl_->foo(width());
    double left = bar - 2, right = bar + 3;

    graphics.fill_rectangle(0, top, width(), bottom, color_scheme()->main());
    graphics.fill_rectangle(left, top - 3, right, bottom + 3,
                            color_scheme()->cool());
}

bool horizontal_slider::onMouseDown(sys::MouseButton button, int times,
                                  Point point)
{
    if (button != sys::mbLButton)
        return false;

    double x = impl_->foo(width());

    if (x - 2 <= point.x && point.x < x + 3)
    {
        std::get<0>(impl_->scrolling_) = true;
        std::get<1>(impl_->scrolling_) = value();
        std::get<2>(impl_->scrolling_) = point;
        capture(true);
    }

    return true;
}

bool horizontal_slider::onMouseMove(Point point)
{
    if (std::get<0>(impl_->scrolling_))
    {
        double delta;

        delta = ((double)point.x - std::get<2>(impl_->scrolling_).x) / width();
        delta *= impl_->max_ - impl_->min_;
        value(std::get<1>(impl_->scrolling_) + delta);
    }

    return true;
}
