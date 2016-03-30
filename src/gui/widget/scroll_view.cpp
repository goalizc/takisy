#include <tuple>
#include <takisy/gui/basic/cursor.h>
#include <takisy/gui/widget/scroll.h>
#include <takisy/gui/widget/scroll_view.h>

class scroll_view::implement
{
    friend class scroll_view;

public:
    implement(widget* content)
        : content_(content), content_moving_(false)
        , dragging_(std::make_tuple(false, 0, 0, Point(0, 0)))
    {
        vscroll_.show();
        vscroll_.step(32);
        vscroll_.range(0, content_->height());
        vscroll_.onScroll(
            [this](const scroll& scroll)
            {
                content_moving_ = true;
                content_->y(-scroll.value());
                content_moving_ = false;
            });

        hscroll_.show();
        hscroll_.step(32);
        hscroll_.range(0, content_->width());
        hscroll_.onScroll(
            [this](const scroll& scroll)
            {
                content_moving_ = true;
                content_->x(-scroll.value());
                content_moving_ = false;
            });
    }

private:
    widget* content_;
    bool content_moving_;
    class vertical_scroll vscroll_;
    class horizontal_scroll hscroll_;
    std::tuple<bool, int, int, Point> dragging_;
};

scroll_view::scroll_view(widget* content)
    : impl_(new implement(content))
{
    add(content);
    add(&impl_->vscroll_);
    add(&impl_->hscroll_);
}

scroll_view::~scroll_view(void)
{
    delete impl_;
}

widget* scroll_view::content(void) const
{
    return impl_->content_;
}

vertical_scroll& scroll_view::vertical_scroll(void)
{
    return impl_->vscroll_;
}

horizontal_scroll& scroll_view::horizontal_scroll(void)
{
    return impl_->hscroll_;
}

void scroll_view::onSize(void)
{
    impl_->vscroll_.rect(width() - 7, 0, 7, height() - 7);
    impl_->hscroll_.rect(0, height() - 7, width() - 7, 7);
    impl_->vscroll_.page(height());
    impl_->hscroll_.page(width());
}

bool scroll_view::onChildMoving(widget* child, Point& point)
{
    if (child == impl_->content_)
        return impl_->content_moving_;

    return true;
}

void scroll_view::onChildSize(widget* child)
{
    if (child == impl_->content_)
    {
        impl_->vscroll_.range(0, impl_->content_->height());
        impl_->hscroll_.range(0, impl_->content_->width());
    }
}

bool scroll_view::onSetCursor(void)
{
    if (std::get<0>(impl_->dragging_))
    {
        cursor::set(cursor::ctSizeAll);

        return true;
    }

    return false;
}

bool scroll_view::onMouseDown(sys::MouseButton button, int times, Point point)
{
    if (button != sys::mbLButton
        || !impl_->content_ || !impl_->content_->inside(point.x, point.y))
        return false;

    std::get<0>(impl_->dragging_) = true;
    std::get<1>(impl_->dragging_) = impl_->hscroll_.value();
    std::get<2>(impl_->dragging_) = impl_->vscroll_.value();
    std::get<3>(impl_->dragging_) = point;
    capture(true);

    return true;
}

bool scroll_view::onMouseUp(sys::MouseButton button, Point point)
{
    if (std::get<0>(impl_->dragging_))
    {
        std::get<0>(impl_->dragging_) = false;
        capture(false);

        return true;
    }

    return false;
}

bool scroll_view::onMouseMove(Point point)
{
    if (std::get<0>(impl_->dragging_))
    {
        Point offset = point - std::get<3>(impl_->dragging_);

        impl_->hscroll_.value(std::get<1>(impl_->dragging_) - offset.x);
        impl_->vscroll_.value(std::get<2>(impl_->dragging_) - offset.y);

        return true;
    }

    return false;
}

bool scroll_view::onMouseWheel(int delta, Point point)
{
    if (impl_->vscroll_.scrollable())
    {
        if (delta < 0)
            impl_->vscroll_.step_down();
        else
            impl_->vscroll_.step_up();
    }
    else
    if (impl_->hscroll_.scrollable())
    {
        if (delta < 0)
            impl_->hscroll_.step_down();
        else
            impl_->hscroll_.step_up();
    }

    return true;
}
