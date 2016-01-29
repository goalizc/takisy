#include <takisy/core/sys.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/label.h>
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
        : color_(sys::default_interface_color()), state_(bsIdle)
    {}

private:
    class color color_;
    ButtonState state_;
};

button::button(void)
    : impl_(new implement)
{}

button::~button(void)
{
    delete impl_;
}

color button::color(void) const
{
    return impl_->color_;
}

void button::color(const class color& color)
{
    impl_->color_ = color;
    repaint();
}

void button::onPaint(graphics graphics, Rect rect)
{
    class color color = impl_->color_;

    switch (impl_->state_)
    {
    case implement::bsInside:  color.blend(color::white(85)); break;
    case implement::bsPressed: color.blend(color::black(85)); break;
    case implement::bsIdle:
    default: break;
    }

    graphics.fill_rectangle(rect, color);
}

bool button::onMouseDown(sys::MouseButton button, int times, Point point)
{
    if (button == sys::mbLButton)
    {
        impl_->state_ = implement::bsPressed;
        repaint();
    }

    return true;
}

bool button::onMouseUp(sys::MouseButton button, Point point)
{
    if (button == sys::mbLButton)
    {
        if (impl_->state_ == implement::bsPressed)
            onClick();

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

text_button::text_button(const char* caption)
    : text_button(caption, sys::default_codec())
{}

text_button::text_button(const char* caption, const char* codec)
    : text_button(stralgo::decode(caption, codec).c_str())
{}

text_button::text_button(const wchar_t* caption)
    : button(), impl_(new implement)
{
    impl_->label_.xy(0, 0);
    impl_->label_.text(caption);
    impl_->label_.margin(3);
    impl_->label_.word_wrap(true);
    impl_->label_.horizontal_alignment(label::haCenter);
    impl_->label_.vertical_alignment(label::vaCenter);
    impl_->label_.show();

    add(&impl_->label_);
}

text_button::~text_button(void)
{
    delete impl_;
}

const char* text_button::caption(const char* codec) const
{
    return stralgo::encode(impl_->label_.text(), codec).c_str();
}

const wchar_t* text_button::caption(void) const
{
    return impl_->label_.text();
}

unsigned int text_button::margin(void) const
{
    return impl_->label_.margin().left;
}

const font& text_button::font(void) const
{
    return impl_->label_.font();
}

brush_sptr text_button::foreground_brush(void) const
{
    return impl_->label_.foreground_brush();
}

widget::Size text_button::optimal(void) const
{
    return impl_->label_.optimal();
}

void text_button::caption(const char* caption)
{
    impl_->label_.text(stralgo::decode(caption, sys::default_codec()).c_str());
}

void text_button::caption(const char* caption, const char* codec)
{
    impl_->label_.text(stralgo::decode(caption, codec).c_str());
}

void text_button::caption(const wchar_t* caption)
{
    impl_->label_.text(caption);
}

void text_button::margin(unsigned int margin)
{
    impl_->label_.margin(margin);
}

void text_button::font(const class font& font)
{
    impl_->label_.font(font);
}

void text_button::foreground_color(const class color& foreground_color)
{
    impl_->label_.foreground_color(foreground_color);
}

void text_button::onSize(Size size)
{
    impl_->label_.size(size);
}
