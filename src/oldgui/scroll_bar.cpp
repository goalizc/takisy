#include <takisy/oldgui/widget/scroll_bar.h>

class ScrollBar::Implement
{
    friend class ScrollBar;

public:
    Implement(void)
        : min_(0), max_(100), step_(1), page_(10), position_(0)
        , vertical_(false), mml_point_(-1e8, 0)
        , color_(Color::green()), handler_(nullptr)
    {}

public:
    double percent(double param) const
    {
        double percent = (param - min_) / (max_ - min_);

        return percent < 0 ? 0 : percent > 1 ? 1 : percent;
    }

private:
    double min_, max_;
    double step_, page_;
    double position_;
    bool vertical_;
    Point mml_point_;
    Color color_;
    std::shared_ptr<Handler> handler_;
};

ScrollBar::ScrollBar(void)
    : impl_(new Implement)
{}

ScrollBar::ScrollBar(const ScrollBar& scroll_bar)
    : ScrollBar()
{
    operator=(scroll_bar);
}

ScrollBar::~ScrollBar(void)
{
    delete impl_;
}

ScrollBar& ScrollBar::operator=(const ScrollBar& scroll_bar)
{
    if (this != &scroll_bar)
    {
        impl_->min_      = scroll_bar.impl_->min_;
        impl_->max_      = scroll_bar.impl_->max_;
        impl_->step_     = scroll_bar.impl_->step_;
        impl_->page_     = scroll_bar.impl_->page_;
        impl_->position_ = scroll_bar.impl_->position_;
        impl_->vertical_ = scroll_bar.impl_->vertical_;
        impl_->color_    = scroll_bar.impl_->color_;
        impl_->handler_  = scroll_bar.impl_->handler_;

        repaint();
    }

    return *this;
}

void ScrollBar::range(double _min, double _max)
{
    if (_min < _max)
        min(_min), max(_max);
    else
        min(_max), max(_min);

    repaint();
}

void ScrollBar::min(double min)
{
    impl_->min_ = min;

    repaint();
}

void ScrollBar::max(double max)
{
    impl_->max_ = max;

    repaint();
}

void ScrollBar::step(double step)
{
    impl_->step_ = step;

    repaint();
}

void ScrollBar::page(double page)
{
    impl_->page_ = page;

    repaint();
}

void ScrollBar::position(double position)
{
    if (position < impl_->min_)
        position = impl_->min_;
    else
    if (position > impl_->max_ - impl_->page_)
        position = impl_->max_ - impl_->page_;

    if (impl_->position_ != position)
    {
        impl_->position_  = position;
        if (impl_->handler_)
            impl_->handler_->process(this);
    }

    repaint();
}

void ScrollBar::vertical(bool vertical)
{
    impl_->vertical_ = vertical;

    repaint();
}

void ScrollBar::color(Color color)
{
    impl_->color_ = color;

    repaint();
}

double ScrollBar::min(void) const
{
    return impl_->min_;
}

double ScrollBar::max(void) const
{
    return impl_->max_;
}

double ScrollBar::step(void) const
{
    return impl_->step_;
}

double ScrollBar::page(void) const
{
    return impl_->page_;
}

double ScrollBar::position(void) const
{
    return impl_->position_;
}

bool ScrollBar::vertical(void) const
{
    return impl_->vertical_;
}

Color ScrollBar::color(void) const
{
    return impl_->color_;
}

void ScrollBar::onScroll(const std::shared_ptr<Handler>& handler)
{
    impl_->handler_ = handler;
}

void ScrollBar::onPaint(Graphics graphics)
{
    double base = impl_->vertical_ ? height() : width();
    double from = impl_->percent(impl_->position_) * base;
    double to   = impl_->percent(impl_->position_ + impl_->page_) * base;

    graphics.fillRectangle(0, 0, width(), height(), impl_->color_ * 50);

    if (impl_->vertical_)
        graphics.fillRectangle(0, from, width(), to, impl_->color_);
    else
        graphics.fillRectangle(from, 0, to, height(), impl_->color_);
}

bool ScrollBar::onKeyDown(VirtualKey vk, KeyState ks)
{
    double h_unit = impl_->vertical_ ? impl_->page_ : impl_->step_;
    double v_unit = impl_->vertical_ ? impl_->step_ : impl_->page_;

    switch (vk)
    {
    case vkLeft:  position(position() - h_unit); break;
    case vkRight: position(position() + h_unit); break;
    case vkUp:    position(position() - v_unit); break;
    case vkDown:  position(position() + v_unit); break;
    default: break;
    };

    return true;
}

bool ScrollBar::onMouseDown(MouseButton mb, KeyState ks, Point point)
{
    double from    = impl_->percent(impl_->position_);
    double to      = impl_->percent(impl_->position_ + impl_->page_);
    double current = 0;

    if (impl_->vertical_)
        current = static_cast<double>(point.y) / height();
    else
        current = static_cast<double>(point.x) / width();

    if (current < from)
        position(position() - page());
    else
    if (current >= to)
        position(position() + page());
    else
    {
        impl_->mml_point_ = point;
        set_capture();
    }

    return true;
}

bool ScrollBar::onMouseMove(KeyState ks, Point point)
{
    if (impl_->mml_point_.x < -1e7)
        return false;

    double delta = 0;
    if (impl_->vertical_)
        delta = static_cast<double>(point.y - impl_->mml_point_.y) / height();
    else
        delta = static_cast<double>(point.x - impl_->mml_point_.x) / width();

    position(position() + delta * (impl_->max_ - impl_->min_));
    impl_->mml_point_ = point;

    return true;
}

bool ScrollBar::onMouseUp(MouseButton mb, KeyState ks, Point point)
{
    impl_->mml_point_.x = -1e8;
    release_capture();

    return true;
}

