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
    implement(widget* content)
        : content_(content), state_(bsIdle)
    {}

private:
    widget* content_;
    ButtonState state_;
};

button::button(void)
    : button(nullptr)
{}

button::button(widget* content)
    : impl_(new implement(content))
{
    if (content)
        add(content);
}

button::~button(void)
{
    delete impl_;
}

widget* button::content(void) const
{
    return impl_->content_;
}

Size button::optimal(OptimalPolicy policy) const
{
    if (impl_->content_)
        return impl_->content_->optimal(policy);
    else
        return optimal();
}

void button::content(widget* content)
{
    remove(impl_->content_);
    add((impl_->content_ = content));

    onSize();
}

void button::onSize(void)
{
    if (impl_->content_)
    {
        impl_->content_->size(size());
        impl_->content_->x((width() - impl_->content_->width()) / 2);
        impl_->content_->y((height() - impl_->content_->height()) / 2);
    }
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

bool button::onMouseUp(sys::Button button, Point point)
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

class text_button::implement
{
    friend class text_button;

private:
    label label_;
};

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
    : button(), impl_(new implement)
{
    impl_->label_.text(caption);
    impl_->label_.margin(3);
    impl_->label_.word_wrap(true);
    impl_->label_.alignment(aCenter);
    impl_->label_.show();

    content(&impl_->label_);
}

text_button::~text_button(void)
{
    delete impl_;
}

label& text_button::text(void)
{
    return impl_->label_;
}

const label& text_button::text(void) const
{
    return impl_->label_;
}

Size text_button::optimal(OptimalPolicy policy) const
{
    return impl_->label_.optimal(policy);
}
