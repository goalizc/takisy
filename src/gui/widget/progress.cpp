#include <takisy/core/algo.h>
#include <takisy/gui/widget/progress.h>

class progress::implement
{
    friend class progress;
    friend class vertical_progress;
    friend class horizontal_progress;

    static constexpr double radius = 3;

public:
    implement(double min, double max)
        : min_(min), max_(max)
    {}

public:
    inline double progress(double length) const
    {
        return (value_ - min_) / (max_ - min_) * length;
    }

private:
    double value_;
    double min_, max_;
};

progress::progress(void)
    : progress(0, 100)
{}

progress::progress(double min, double max)
    : progress(min, max, min)
{}

progress::progress(double min, double max, double _value)
    : impl_(new implement(min, max))
{
    value(_value);
}

progress::~progress(void)
{
    delete impl_;
}

double progress::min(void) const
{
    return impl_->min_;
}

double progress::max(void) const
{
    return impl_->max_;
}

double progress::value(void) const
{
    return impl_->value_;
}

void progress::range(double _min, double _max)
{
    min(_min);
    max(_max);
}

void progress::min(double min)
{
    if (impl_->min_ != min)
    {
        impl_->min_ = min;
        value(value());
        repaint();
    }
}

void progress::max(double max)
{
    if (impl_->max_ != max)
    {
        impl_->max_ = max;
        value(value());
        repaint();
    }
}

void progress::value(double value)
{
    value = algo::clamp(value, impl_->min_, impl_->max_);

    if (impl_->value_ != value)
    {
        impl_->value_ = value;
        onProgressChangedHandle();
        repaint();
    }
}

void progress::home(void)
{
    value(impl_->min_);
}

void progress::end(void)
{
    value(impl_->max_);
}

void vertical_progress::onPaint(graphics graphics, Rect rect)
{
    constexpr double radius = implement::radius;
    color color = color_scheme().theme();
    rectf progress_rect = client_rect();

    if (impl_->min_ < impl_->max_)
        progress_rect.bottom = impl_->progress(height());
    else
        progress_rect.top = impl_->progress(height());

    graphics.fill_round_rectangle(client_rect(), radius, color * 100);
    graphics.fill_round_rectangle(progress_rect, radius, color);
}

void horizontal_progress::onPaint(graphics graphics, Rect rect)
{
    constexpr double radius = implement::radius;
    color color = color_scheme().theme();
    rectf progress_rect = client_rect();

    if (impl_->min_ < impl_->max_)
        progress_rect.right = impl_->progress(width());
    else
        progress_rect.left = impl_->progress(width());

    graphics.fill_round_rectangle(client_rect(), radius, color * 100);
    graphics.fill_round_rectangle(progress_rect, radius, color);
}
