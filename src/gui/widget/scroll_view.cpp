#include <takisy/gui/widget/scroll.h>
#include <takisy/gui/widget/scroll_view.h>

class scroll_view::implement
{
    friend class scroll_view;

public:
    implement(widget* content)
        : content_(content)
    {
        vscroll_.show();
        vscroll_.step(32);
        vscroll_.range(0, content_->height());
        vscroll_.onScroll(
            [this](const scroll& scroll)
            {
                content_->y(-scroll.value());
            });

        hscroll_.show();
        hscroll_.step(32);
        hscroll_.range(0, content_->width());
        hscroll_.onScroll(
            [this](const scroll& scroll)
            {
                content_->x(-scroll.value());
            });
    }

private:
    widget* content_;
    class vertical_scroll vscroll_;
    class horizontal_scroll hscroll_;
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

void scroll_view::onChildSize(widget* child)
{
    if (child == impl_->content_)
    {
        impl_->vscroll_.range(0, impl_->content_->height());
        impl_->hscroll_.range(0, impl_->content_->width());
    }
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
