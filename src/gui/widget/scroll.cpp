#include <tuple>
#include <utility>
#include <takisy/gui/basic/cursor.h>
#include <takisy/gui/widget/scroll.h>

class scroll::implement
{
    friend class scroll;
    friend class vertical_scroll;
    friend class horizontal_scroll;

    static constexpr int slider_radius = 3;

public:
    implement(void)
        : in_scroll_(false)
        , scrolling_(std::make_tuple(false, 0, Point(0, 0)))
    {}

public:
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
    std::tuple<bool, double, Point> scrolling_;
};

scroll::scroll(void)
    : scroll(0, 100)
{}

scroll::scroll(double min, double max)
    : scroll(min, max, min)
{}

scroll::scroll(double min, double max, double value)
    : scroll(min, max, value, (max - min) / 100, (max - min) / 10)
{}

scroll::scroll(double min, double max, double value, double step, double page)
    : impl_(new implement)
{
    impl_->min_   = min;
    impl_->max_   = max;
    impl_->value_ = value;
    impl_->step_  = step;
    impl_->page_  = page;
}

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
    return impl_->page_ < impl_->max_ - impl_->min_;
}

void scroll::range(double _min, double _max)
{
    min(_min);
    max(_max);
}

void scroll::min(double min)
{
    if (impl_->min_ != min)
    {
        impl_->min_ = min;
        value(value());
        repaint();
    }
}

void scroll::max(double max)
{
    if (impl_->max_ != max)
    {
        impl_->max_ = max;
        value(value());
        repaint();
    }
}

void scroll::value(double value)
{
    if (value > impl_->max_ - impl_->page_)
        value = impl_->max_ - impl_->page_;
    if (value < impl_->min_)
        value = impl_->min_;

    if (impl_->value_ != value)
    {
        impl_->value_ = value;
        onScroll();
        repaint();
    }
}

void scroll::step(double step)
{
    if (impl_->step_ != step)
        impl_->step_ = step;
}

void scroll::page(double page)
{
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
    value(max());
}

bool scroll::onSetCursor(void)
{
    if (scrollable())
    {
        cursor::set(cursor::ctArrow);
        return true;
    }

    return false;
}

bool scroll::onClick(sys::MouseButton, int, Point)
{
    return scrollable();
}

bool scroll::onMouseUp(sys::MouseButton button, Point point)
{
    if (!scrollable())
        return false;

    std::get<0>(impl_->scrolling_) = false;
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

    color color = color_scheme()->main();
    std::pair<double, double> pair = impl_->slider_range(height());
    Rect block_rect = client_rect();
    register int radius = implement::slider_radius;
    block_rect.top    = pair.first;
    block_rect.bottom = pair.second;

    if (impl_->in_scroll_)
    {
        graphics.fill_round_rectangle(client_rect(), radius, color * 100);
        graphics.fill_round_rectangle(block_rect,    radius, color);
    }
    else
        graphics.fill_round_rectangle(block_rect,    radius, color * 128);
}

bool vertical_scroll::onMouseDown(sys::MouseButton button, int times,
                                  Point point)
{
    if (button != sys::mbLButton || !scrollable())
        return false;

    std::pair<double, double> pair = impl_->slider_range(height());

    if (pair.first > point.y)
        page_up();
    else
    if (pair.second < point.y)
        page_down();
    else
    {
        std::get<0>(impl_->scrolling_) = true;
        std::get<1>(impl_->scrolling_) = value();
        std::get<2>(impl_->scrolling_) = point;
        capture(true);
    }

    return true;
}

bool vertical_scroll::onMouseMove(Point point)
{
    if (!scrollable())
        return false;

    if (!std::get<0>(impl_->scrolling_))
        return true;

    double delta;

    delta  = ((double)point.y - std::get<2>(impl_->scrolling_).y) / height();
    delta *= impl_->max_ - impl_->min_;
    value(std::get<1>(impl_->scrolling_) + delta);

    return true;
}

void horizontal_scroll::onPaint(graphics graphics, Rect rect)
{
    if (!scrollable())
        return;

    color color = color_scheme()->main();
    std::pair<double, double> pair = impl_->slider_range(width());
    Rect block_rect = client_rect();
    register int radius = implement::slider_radius;
    block_rect.left  = pair.first;
    block_rect.right = pair.second;

    if (impl_->in_scroll_)
    {
        graphics.fill_round_rectangle(client_rect(), radius, color * 100);
        graphics.fill_round_rectangle(block_rect,    radius, color);
    }
    else
        graphics.fill_round_rectangle(block_rect,    radius, color * 128);
}

bool horizontal_scroll::onMouseDown(sys::MouseButton button, int times,
                                    Point point)
{
    if (button != sys::mbLButton || !scrollable())
        return false;

    std::pair<double, double> pair = impl_->slider_range(width());

    if (pair.first > point.x)
        page_up();
    else
    if (pair.second < point.x)
        page_down();
    else
    {
        std::get<0>(impl_->scrolling_) = true;
        std::get<1>(impl_->scrolling_) = value();
        std::get<2>(impl_->scrolling_) = point;
        capture(true);
    }

    return true;
}

bool horizontal_scroll::onMouseMove(Point point)
{
    if (!scrollable())
        return false;

    if (!std::get<0>(impl_->scrolling_))
        return true;

    double delta;

    delta  = ((double)point.x - std::get<2>(impl_->scrolling_).x) / width();
    delta *= impl_->max_ - impl_->min_;
    value(std::get<1>(impl_->scrolling_) + delta);

    return true;
}
