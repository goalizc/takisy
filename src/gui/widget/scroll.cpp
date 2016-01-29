#include <tuple>
#include <utility>
#include <takisy/core/sys.h>
#include <takisy/gui/widget/scroll.h>

class scroll::implement
{
    friend class scroll;
    friend class vertical_scroll;

public:
    implement(void)
        : color_(sys::default_interface_color()), in_scroll_(false)
        , scrolling_(std::make_tuple(false, 0, widget::Point(0, 0)))
    {}

public:
    std::pair<double, double> piece(double length) const
    {
        double a = max_ - min_;
        double b = length * page_ / a;
               b = b < 10 ? 10 : b;
        double c = value_ * (length - b) / (a - page_);

        return std::make_pair(c, c + b);
    }

private:
    bool   vertical_;
    double min_, max_;
    double value_, step_, page_;
    class color color_;
    bool   in_scroll_;
    std::tuple<bool, double, widget::Point> scrolling_;
};

scroll::scroll(void)
    : scroll(0, 100)
{}

scroll::scroll(double min, double max)
    : scroll(min, max, min)
{}

scroll::scroll(double min, double max, double value)
    : scroll(min, max, value, 1, 10)
{}

scroll::scroll(double min, double max, double value, double step, double page)
    : impl_(new implement)
{
    impl_->vertical_ = false;
    impl_->min_      = min;
    impl_->max_      = max;
    impl_->value_    = value;
    impl_->step_     = step;
    impl_->page_     = page;

    attribute("intercept.onSetCursor", true);
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

class color scroll::color(void) const
{
    return impl_->color_;
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
        impl_->min_  = min;
        value(value());
        repaint();
    }
}

void scroll::max(double max)
{
    if (impl_->max_ != max)
    {
        impl_->max_  = max;
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
        impl_->value_  = value;
        onScroll();
        repaint();
    }
}

void scroll::step(double step)
{
    if (impl_->step_ != step)
        impl_->step_  = step;
}

void scroll::page(double page)
{
    if (impl_->page_ != page)
    {
        impl_->page_  = page;
        repaint();
    }
}

void scroll::color(const class color& color)
{
    if (impl_->color_ != color)
    {
        impl_->color_  = color;
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

void scroll::onPaint(graphics graphics, Rect)
{
    if (!scrollable())
        return;

    widget::Rect piece_rect = client_rect();
    if (impl_->vertical_)
    {
        std::pair<double, double> pair = impl_->piece(height());
        piece_rect.top    = pair.first;
        piece_rect.bottom = pair.second;
    }
    else
    {
        std::pair<double, double> pair = impl_->piece(width());
        piece_rect.left  = pair.first;
        piece_rect.right = pair.second;
    }

    if (impl_->in_scroll_)
    {
        graphics.fill_round_rectangle(client_rect(), 3, impl_->color_ * 100);
        graphics.fill_round_rectangle(piece_rect,    3, impl_->color_);
    }
    else
        graphics.fill_round_rectangle(piece_rect,    3, impl_->color_ * 128);
}

bool scroll::onMouseDown(sys::MouseButton button, int, Point point)
{
    if (button != sys::mbLButton || !scrollable())
        return false;

    register double length = impl_->vertical_ ? height() : width();
    register double foobar = impl_->vertical_ ? point.y : point.x;
    std::pair<double, double> pair = impl_->piece(length);

    if (pair.first > foobar)
        page_up();
    else
    if (pair.second < foobar)
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

bool scroll::onMouseMove(Point point)
{
    if (!scrollable())
        return false;

    if (!std::get<0>(impl_->scrolling_))
        return true;

    double delta;
    if (impl_->vertical_)
        delta = ((double)point.y - std::get<2>(impl_->scrolling_).y) / height();
    else
        delta = ((double)point.x - std::get<2>(impl_->scrolling_).x) / width();
    delta *= impl_->max_ - impl_->min_;

    value(std::get<1>(impl_->scrolling_) + delta);

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

vertical_scroll::vertical_scroll(void)
    : vertical_scroll(0, 100)
{}

vertical_scroll::vertical_scroll(double min, double max)
    : vertical_scroll(min, max, min)
{}

vertical_scroll::vertical_scroll(double min, double max, double value)
    : vertical_scroll(min, max, value, 1, 10)
{}

vertical_scroll::vertical_scroll(double min, double max, double value,
                                 double step, double page)
    : scroll(min, max, value, step, page)
{
    impl_->vertical_ = true;
}
