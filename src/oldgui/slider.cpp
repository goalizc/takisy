#include <takisy/core/algorithm.h>
#include <takisy/cgl/path/builder/rectangle.h>
#include <takisy/oldgui/widget/slider.h>

class Slider::Implement
{
    friend class Slider;

public:
    Implement(void)
        : min_(0), max_(100), value_(10)
        , horizontal_(true), negative_direction_(false)
        , sliding_(false), on_slide_listener_(nullptr)
    {}

private:
    double  min_, max_;
    double  value_;
    bool    horizontal_;
    bool    negative_direction_;
    bool    sliding_;
    std::shared_ptr<OnSlideListener> on_slide_listener_;
};

Slider::Slider(void)
    : Slider(0.0, 100.0, 10.0)
{}

Slider::Slider(double min, double max)
    : Slider(min, max, min)
{}

Slider::Slider(double _min, double _max, double _value)
    : impl_(new Implement)
{
    min(_min);
    max(_max);
    value(_value);
}

Slider::Slider(const Slider& slider)
    : impl_(new Implement)
{
    operator=(slider);
}

Slider::~Slider(void)
{
    delete impl_;
}

Slider& Slider::operator=(const Slider& slider)
{
    if (this != &slider)
    {
        impl_->min_                = slider.impl_->min_;
        impl_->max_                = slider.impl_->max_;
        impl_->value_              = slider.impl_->value_;
        impl_->negative_direction_ = slider.impl_->negative_direction_;
    }

    return *this;
}

double Slider::min(void) const
{
    return impl_->min_;
}

double Slider::max(void) const
{
    return impl_->max_;
}

double Slider::value(void) const
{
    return impl_->value_;
}

bool Slider::horizontal(void) const
{
    return impl_->horizontal_;
}

bool Slider::negativeDirection(void) const
{
    return impl_->negative_direction_;
}

void Slider::min(double min)
{
    impl_->min_ = min;
}

void Slider::max(double max)
{
    impl_->max_ = max;
}

void Slider::minmax(double min, double max)
{
    impl_->min_ = min;
    impl_->max_ = max;
}

void Slider::value(double value)
{
    value = algorithm::clamp(value, impl_->min_, impl_->max_);

    if (value != impl_->value_)
    {
        impl_->value_ = value;

        repaint();

        if (impl_->on_slide_listener_)
            impl_->on_slide_listener_->onSlide(impl_->value_);
    }
}

void Slider::horizontal(bool horizontal)
{
    impl_->horizontal_ = horizontal;
}

void Slider::negativeDirection(bool negative_direction)
{
    impl_->negative_direction_ = negative_direction;
}

void Slider::onPaint(Graphics graphics)
{
    double base = impl_->horizontal_ ? width() : height();
    double pos  = (value() - min()) / (max() - min()) * base;
    Color fg_color = Color::green();
    Color bg_color = Color::light_gray();
    rectangle r;

    if (impl_->sliding_)
        fg_color.blend(Color::white(50));

    if (impl_->negative_direction_)
    {
        pos = base - pos;
        algorithm::swap(fg_color, bg_color);
    }

    if (impl_->horizontal_)
    {
        graphics.fillPath(r.build(0,   0, pos,     height()), fg_color);
        graphics.fillPath(r.build(pos, 0, width(), height()), bg_color);
    }
    else
    {
        graphics.fillPath(r.build(0, 0,   width(), pos     ), fg_color);
        graphics.fillPath(r.build(0, pos, width(), height()), bg_color);
    }
}

bool Slider::onMouseDown(MouseButton mb, KeyState ks, Point point)
{
    if (mb == mbLeftButton)
    {
        impl_->sliding_ = true;
        set_capture();
        onMouseMove(ks, point);
    }

    return true;
}

bool Slider::onMouseMove(KeyState ks, Point point)
{
    if (impl_->sliding_)
    {
        double percent = impl_->horizontal_
                ? (impl_->negative_direction_
                   ? double(width())  - point.x : double(point.x)) / width()
                : (impl_->negative_direction_
                   ? double(height()) - point.y : double(point.y)) / height();

        value(percent * (max() - min()) + min());
    }

    return true;
}

bool Slider::onMouseUp(MouseButton mb, KeyState ks, Point point)
{
    impl_->sliding_ = false;
    release_capture();
    repaint();

    return true;
}

void Slider::onSlide(const std::shared_ptr<OnSlideListener>& listener)
{
    impl_->on_slide_listener_ = listener;
}
