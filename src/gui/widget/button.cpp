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
    class color color = color_scheme()->main();

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

text_button::text_button(const std::string& caption)
    : text_button(caption, sys::default_codec())
{}

text_button::text_button(const std::string& caption, const std::string& codec)
    : text_button(stralgo::decode(caption, codec))
{}

text_button::text_button(const std::wstring& caption)
    : button(), impl_(new implement)
{
    impl_->label_.xy(0, 0);
    impl_->label_.text(caption);
    impl_->label_.margin(3);
    impl_->label_.word_wrap(true);
    impl_->label_.alignment(aCenter);
    impl_->label_.show();

    add(&impl_->label_);
}

text_button::~text_button(void)
{
    delete impl_;
}

std::string text_button::caption(const std::string& codec) const
{
    return stralgo::encode(impl_->label_.text(), codec);
}

std::wstring text_button::caption(void) const
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

Size text_button::optimal_size(void) const
{
    return impl_->label_.optimal_size();
}

void text_button::caption(const std::string& caption)
{
    impl_->label_.text(stralgo::decode(caption, sys::default_codec()));
}

void text_button::caption(const std::string& caption, const std::string& codec)
{
    impl_->label_.text(stralgo::decode(caption, codec));
}

void text_button::caption(const std::wstring& caption)
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

void text_button::onSize(void)
{
    impl_->label_.size(size());
}
