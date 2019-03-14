#include <takisy/core/sys.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/button.h>

class button::implement
{
    friend class button;

public:
    enum ButtonState
    {
        bsIdle, bsInside, bsPressed
    };

public:
    implement(void)
        : state_(bsIdle)
    {}

private:
    ButtonState state_;
};

button::button(void)
    : impl_(new implement)
{}

button::~button(void)
{
    delete impl_;
}

void button::onPaint(graphics graphics, Rect rect)
{
    graphics.fill_rectangle(rect, color_scheme().theme());
}

void button::onEndPaint(graphics graphics, Rect rect)
{
    if (impl_->state_ == implement::bsInside)
        graphics.fill_rectangle(rect, color::white(85));
    else
    if (impl_->state_ == implement::bsPressed)
        graphics.fill_rectangle(rect, color::black(85));
}

bool button::onMouseDown(sys::Button button, int times, Point point)
{
    if (button == sys::btnLeft)
    {
        impl_->state_ = implement::bsPressed;
        repaint();
    }

    return true;
}

bool button::onMouseUp(sys::Button button, int times, Point point)
{
    if (button == sys::btnLeft)
    {
        if (impl_->state_ == implement::bsPressed)
            onClickHandle();

        impl_->state_ = implement::bsInside;
        repaint();
    }

    return true;
}

bool button::onMouseEnter(void)
{
    impl_->state_ = implement::bsInside;
    repaint();

    return true;
}

bool button::onMouseLeave(void)
{
    impl_->state_ = implement::bsIdle;
    repaint();

    return true;
}

class widget_button::implement
{
    friend class widget_button;

public:
    implement(class widget* widget)
        : widget_(widget)
    {}

private:
    class widget* widget_;
};

widget_button::widget_button(void)
    : widget_button(nullptr)
{}

widget_button::widget_button(class widget* widget)
    : impl_(new implement(widget))
{
    if (widget)
        widget::add(widget);
}

widget_button::~widget_button(void)
{
    delete impl_;
}

class widget* widget_button::widget(void)
{
    return impl_->widget_;
}

const class widget* widget_button::widget(void) const
{
    return impl_->widget_;
}

Size widget_button::optimal(OptimalPolicy policy) const
{
    if (impl_->widget_)
        return impl_->widget_->optimal(policy);
    else
        return optimal();
}

void widget_button::widget(class widget* widget)
{
    if (!impl_->widget_)
        widget::remove(impl_->widget_);

    impl_->widget_ = widget;

    if (impl_->widget_)
    {
        widget::add(impl_->widget_);
        onSize();
    }
}

void widget_button::onSize(void)
{
    if (impl_->widget_)
    {
        impl_->widget_->size(size());
        impl_->widget_->xy(
            (width()  - impl_->widget_->width())  / 2,
            (height() - impl_->widget_->height()) / 2
        );
    }
}

text_button::text_button(void)
    : text_button(L"")
{}

text_button::text_button(const std::string& caption)
    : text_button(caption, sys::default_codec())
{}

text_button::text_button(const std::string& caption, const std::string& codec)
    : text_button(stralgo::decode(caption, codec))
{}

text_button::text_button(const std::wstring& caption)
    : label_(caption)
{
    label_.margin(3);
    label_.word_wrap(true);
    label_.alignment(aCenter);
    label_.show();

    widget(&label_);
}

label& text_button::text(void)
{
    return label_;
}

const label& text_button::text(void) const
{
    return label_;
}

Size text_button::optimal(OptimalPolicy policy) const
{
    return label_.optimal(policy);
}
