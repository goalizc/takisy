#include <takisy/gui/widget/scroll_view.h>

class scroll_view::implement
{
    friend class scroll_view;

public:
    implement(widget* content)
        : content_(content), content_moving_(false)
    {}

private:
    widget* content_;
    bool content_moving_;
};

scroll_view::scroll_view(widget* content)
    : impl_(new implement(content))
{
    vertical_scroll().min(0);
    vertical_scroll().step(32);
    vertical_scroll().show();
    vertical_scroll().onScroll(
        [this](const scroll* self)
        {
            impl_->content_moving_ = true;
            impl_->content_->y(-self->value());
            impl_->content_moving_ = false;
        });

    horizontal_scroll().min(0);
    horizontal_scroll().step(32);
    horizontal_scroll().show();
    horizontal_scroll().onScroll(
        [this](const scroll* self)
        {
            impl_->content_moving_ = true;
            impl_->content_->x(-self->value());
            impl_->content_moving_ = false;
        });

    add(content);
    add(&vertical_scroll());
    add(&horizontal_scroll());
    onChildSize(content);
}

scroll_view::~scroll_view(void)
{
    delete impl_;
}

widget* scroll_view::content(void) const
{
    return impl_->content_;
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
        vertical_scroll()
            .max(impl_->content_->height() + horizontal_scroll().height());
        horizontal_scroll()
            .max(impl_->content_->width() + vertical_scroll().width());
    }
}
