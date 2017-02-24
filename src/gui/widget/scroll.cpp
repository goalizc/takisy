#include <utility>
#include <takisy/core/algo.h>
#include <takisy/core/math.h>
#include <takisy/gui/basic/cursor.h>
#include <takisy/gui/widget/scroll.h>

class scroll::implement
{
    friend class scroll;
    friend class vertical_scroll;
    friend class horizontal_scroll;

    static constexpr double radius = 3;

public:
    implement(double min, double max, double value, double step, double page)
        : in_scroll_(false)
        , min_(min), max_(max), value_(value), step_(step), page_(page)
        , dragging_({.underway = false})
    {}

public:
    inline int sign(void) const
    {
        return min_ <= max_ ? 1 : -1;
    }

    std::pair<double, double> slider_range(double length) const
    {
        double a = max_ - min_;
        double b = length * page_ / a;
               b = b < 10 ? 10 : b;
        double c = (value_ - min_) * (length - b) / (a - page_);

        return std::make_pair(c, c + b);
    }

private:
    bool in_scroll_;
    double min_, max_, value_, step_, page_;
    struct {
        bool underway;
        struct {
            double value;
            Point point;
        } milestone;
    } dragging_;
};

scroll::scroll(void)
    : scroll(0, 100)
{}

scroll::scroll(double min, double max)
    : scroll(min, max, min)
{}

scroll::scroll(double min, double max, double value)
    : impl_(new implement(min, max, value, (max - min) / 100, (max - min) / 10))
{}

scroll::~scroll(void)
{
    delete impl_;
}

double scroll::min(void) const
{
    return impl_->min_;
}

double scroll::max(void) const
{
    return impl_->max_;
}

double scroll::value(void) const
{
    return impl_->value_;
}

long long scroll::valued(void) const
{
    return impl_->value_ + 0.5;
}

double scroll::step(void) const
{
    return impl_->step_;
}

double scroll::page(void) const
{
    return impl_->page_;
}

bool scroll::scrollable(void) const
{
    return math::abs(impl_->page_) < math::abs(impl_->max_ - impl_->min_);
}

void scroll::range(double min, double max)
{
    if (impl_->min_ == min && impl_->max_ == max)
        return;

    if (impl_->min_ != min)
        impl_->min_ = min;
    if (impl_->max_ != max)
        impl_->max_ = max;

    step(step());
    page(page());
    value(value());
    repaint();
}

void scroll::min(double min)
{
    range(min, max());
}

void scroll::max(double max)
{
    range(min(), max);
}

void scroll::value(double value)
{
    if (scrollable())
        value = algo::clamp(value, impl_->min_, impl_->max_ - page());
    else
        value = impl_->min_;

    if (impl_->value_ != value)
    {
        impl_->value_ = value;
        onScrollHandle();
        repaint();
    }
}

void scroll::step(double step)
{
    step = impl_->sign() * math::abs(step);

    if (impl_->step_ != step)
        impl_->step_ = step;
}

void scroll::page(double page)
{
    page = impl_->sign() * math::abs(page);

    if (impl_->page_ != page)
    {
        impl_->page_ = page;
        value(value());
        repaint();
    }
}

void scroll::step_down(void)
{
    value(value() + step());
}

void scroll::step_up(void)
{
    value(value() - step());
}

void scroll::page_down(void)
{
    value(value() + page());
}

void scroll::page_up(void)
{
    value(value() - page());
}

void scroll::home(void)
{
    value(min());
}

void scroll::end(void)
{
    value(max() - page());
}

bool scroll::onSetCursor(Point)
{
    if (scrollable())
    {
        cursor::set(cursor::ctArrow);
        return true;
    }

    return false;
}

bool scroll::onMouseUp(sys::Button button, Point point)
{
    if (!scrollable())
        return false;

    impl_->dragging_.underway = false;
    capture(false);

    return true;
}

bool scroll::onMouseEnter(void)
{
    if (!scrollable())
        return false;

    impl_->in_scroll_ = true;
    repaint();

    return true;
}

bool scroll::onMouseLeave(void)
{
    if (!scrollable())
        return false;

    impl_->in_scroll_ = false;
    repaint();

    return true;
}

bool scroll::onMouseWheel(int delta, Point)
{
    if (!scrollable())
        return false;

    if (delta < 0)
        step_down();
    else
    if (delta > 0)
        step_up();

    return true;
}

void vertical_scroll::onPaint(graphics graphics, Rect rect)
{
    if (!scrollable())
        return;

    constexpr double radius = implement::radius;
    color color = color_scheme().theme();
    std::pair<double, double> pair = impl_->slider_range(height());
    rectf slider_rect  = client_rect();
    slider_rect.top    = pair.first;
    slider_rect.bottom = pair.second;

    if (impl_->in_scroll_)
    {
        graphics.fill_round_rectangle(client_rect(), radius, color * 100);
        graphics.fill_round_rectangle(slider_rect,   radius, color);
    }
    else
        graphics.fill_round_rectangle(slider_rect,   radius, color * 128);
}

bool vertical_scroll::onMouseDown(sys::Button button, int times, Point point)
{
    if (button != sys::btnLeft || !scrollable())
        return false;

    std::pair<double, double> pair = impl_->slider_range(height());

    if (pair.first > point.y)
        page_up();
    else
    if (pair.second < point.y)
        page_down();
    else
    {
        impl_->dragging_.underway = true;
        impl_->dragging_.milestone.value = value();
        impl_->dragging_.milestone.point = point;
        capture(true);
    }

    return true;
}

bool vertical_scroll::onMouseMove(Point point)
{
    if (!scrollable())
        return false;

    if (!impl_->dragging_.underway)
        return true;

    double
        delta  = point.y - impl_->dragging_.milestone.point.y;
        delta /= height();
        delta *= impl_->max_ - impl_->min_;

    value(impl_->dragging_.milestone.value + delta);

    return true;
}

void horizontal_scroll::onPaint(graphics graphics, Rect rect)
{
    if (!scrollable())
        return;

    constexpr double radius = implement::radius;
    color color = color_scheme().theme();
    std::pair<double, double> pair = impl_->slider_range(width());
    rectf slider_rect = client_rect();
    slider_rect.left  = pair.first;
    slider_rect.right = pair.second;

    if (impl_->in_scroll_)
    {
        graphics.fill_round_rectangle(client_rect(), radius, color * 100);
        graphics.fill_round_rectangle(slider_rect,   radius, color);
    }
    else
        graphics.fill_round_rectangle(slider_rect,   radius, color * 128);
}

bool horizontal_scroll::onMouseDown(sys::Button button, int times, Point point)
{
    if (button != sys::btnLeft || !scrollable())
        return false;

    std::pair<double, double> pair = impl_->slider_range(width());

    if (pair.first > point.x)
        page_up();
    else
    if (pair.second < point.x)
        page_down();
    else
    {
        impl_->dragging_.underway = true;
        impl_->dragging_.milestone.value = value();
        impl_->dragging_.milestone.point = point;
        capture(true);
    }

    return true;
}

bool horizontal_scroll::onMouseMove(Point point)
{
    if (!scrollable())
        return false;

    if (!impl_->dragging_.underway)
        return true;

    double
        delta  = point.x - impl_->dragging_.milestone.point.x;
        delta /= width();
        delta *= impl_->max_ - impl_->min_;

    value(impl_->dragging_.milestone.value + delta);

    return true;
}

class scroll_area::implement
{
    friend class scroll_area;

public:
    implement(void)
        : dragging_({.underway = false})
    {
        vscroll_.width(7);
        vscroll_.show();
        hscroll_.height(7);
        hscroll_.show();
    }

private:
    class vertical_scroll vscroll_;
    class horizontal_scroll hscroll_;
    struct {
        bool underway;
        struct {
            int vvalue, hvalue;
            Point point;
        } milestone;
    } dragging_;
};

scroll_area::scroll_area(void)
    : impl_(new implement)
{}

scroll_area::~scroll_area(void)
{
    delete impl_;
}

class vertical_scroll& scroll_area::vertical_scroll(void)
{
    return impl_->vscroll_;
}

const class vertical_scroll& scroll_area::vertical_scroll(void) const
{
    return impl_->vscroll_;
}

class horizontal_scroll& scroll_area::horizontal_scroll(void)
{
    return impl_->hscroll_;
}

const class horizontal_scroll& scroll_area::horizontal_scroll(void) const
{
    return impl_->hscroll_;
}

void scroll_area::onSize(void)
{
    impl_->vscroll_.x(width() - impl_->vscroll_.width());
    impl_->vscroll_.height(height() - impl_->hscroll_.height());
    impl_->vscroll_.page(height());
    impl_->hscroll_.y(height() - impl_->hscroll_.height());
    impl_->hscroll_.width(width() - impl_->vscroll_.width());
    impl_->hscroll_.page(width());
}

bool scroll_area::onSetCursor(Point)
{
    if (impl_->dragging_.underway)
    {
        cursor::set(cursor::ctSizeAll);

        return true;
    }

    return false;
}

bool scroll_area::onMouseDown(sys::Button button, int times, Point point)
{
    if (button != sys::btnLeft)
        return false;
    if (!vertical_scroll().scrollable() && !horizontal_scroll().scrollable())
        return false;

    impl_->dragging_.underway = true;
    impl_->dragging_.milestone.vvalue = vertical_scroll().value();
    impl_->dragging_.milestone.hvalue = horizontal_scroll().value();
    impl_->dragging_.milestone.point = point;
    capture(true);

    return true;
}

bool scroll_area::onMouseUp(sys::Button button, Point point)
{
    if (impl_->dragging_.underway)
    {
        impl_->dragging_.underway = false;
        capture(false);

        return true;
    }

    return false;
}

bool scroll_area::onMouseMove(Point point)
{
    if (impl_->dragging_.underway)
    {
        Point offset = point - impl_->dragging_.milestone.point;

        vertical_scroll().value(impl_->dragging_.milestone.vvalue - offset.y);
        horizontal_scroll().value(impl_->dragging_.milestone.hvalue - offset.x);

        return true;
    }

    return false;
}

bool scroll_area::onMouseWheel(int delta, Point point)
{
    if (impl_->vscroll_.scrollable())
    {
        if (delta < 0)
            impl_->vscroll_.step_down();
        else
            impl_->vscroll_.step_up();

        return true;
    }
    else
    if (impl_->hscroll_.scrollable())
    {
        if (delta < 0)
            impl_->hscroll_.step_down();
        else
            impl_->hscroll_.step_up();

        return true;
    }

    return false;
}
