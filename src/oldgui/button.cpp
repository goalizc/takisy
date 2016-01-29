#include <takisy/core/math.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/cgl/font/tfont_simple.h>
#include <takisy/oldgui/widget/button.h>

class Button::Implement
{
    friend class Button;

public:
    enum ButtonStatus
    {
        bsNormal    = 1,
        bsMoving    = 2,
        bsPressed   = 3,
    };

public:
    Implement(void)
        : bottom_color_(Color::black()), button_status_(bsNormal)
        , on_click_listener_(nullptr)
    {}

private:
    Color bottom_color_;
    ButtonStatus button_status_;
    std::shared_ptr<Button::OnClickListener> on_click_listener_;
};

Button::Button(void)
    : Widget(0, 0, 50, 20, nullptr, true), impl_(new Implement)
{}

Button::Button(const Button& button)
    : impl_(new Implement)
{
    operator=(button);
}

Button::~Button(void)
{
    delete impl_;
}

Button& Button::operator=(const Button& button)
{
    if (this != &button)
    {
        impl_->bottom_color_      = button.impl_->bottom_color_;
        impl_->on_click_listener_ = button.impl_->on_click_listener_;

        rect(button.x(), button.y(), button.width(), button.height());
        parent(button.parent());
        visible(button.visible());
    }

    return *this;
}

void Button::bottom_color(Color bottom_color)
{
    impl_->bottom_color_ = bottom_color;
}

Color Button::bottom_color(void) const
{
    return impl_->bottom_color_;
}

void Button::onPaint(Graphics graphics)
{
    Color color = impl_->bottom_color_;

    switch (impl_->button_status_)
    {
    case Implement::bsNormal:  break;
    case Implement::bsMoving:  color.blend(Color::white(50)); break;
    case Implement::bsPressed: color.blend(Color::black(50)); break;
    }

    graphics.clear(color);

    // double from_alpha = 100, to_alpha = 20;
    // Color edge_color = Color::white(50);

    // switch (impl_->button_status_)
    // {
    // case Implement::bsNormal: break;
    // case Implement::bsMoving:  edge_color = Color::white(100); break;
    // case Implement::bsPressed:
    //     {
    //         double bar = from_alpha;
    //         from_alpha = to_alpha;
    //         to_alpha   = bar;
    //     }
    //     break;
    // }

    // graphics.fillRoundRectangle(0, 0, width(), height(), 1, impl_->bottom_color_);
    // graphics.fillRoundRectangle(0, 0, width(), height(), 1, makeLambdaBrush([&](int, double y) {
    //     return Color::white(y / height() * (to_alpha - from_alpha) + from_alpha);
    // }));
    // graphics.drawRoundRectangle(0, 0, width() - 1, height() - 1, 1, edge_color);
}

bool Button::onMouseDown(MouseButton mb, KeyState ks, Point point)
{
    if (mb == mbLeftButton)
    {
        impl_->button_status_ = Implement::bsPressed;
        repaint();
    }

    return true;
}

bool Button::onMouseUp(MouseButton mb, KeyState ks, Point point)
{
    if (mb == mbLeftButton)
    {
        impl_->button_status_ = Implement::bsMoving;
        repaint();
    }

    return true;
}

bool Button::onMouseEnter(KeyState ks, Point point)
{
    impl_->button_status_ = Implement::bsMoving;
    repaint();

    return true;
}

bool Button::onMouseLeave(KeyState ks, Point point)
{
    impl_->button_status_ = Implement::bsNormal;
    repaint();

    return true;
}

void Button::onClick(const std::shared_ptr<OnClickListener>& listener)
{
    impl_->on_click_listener_ = listener;
}

bool Button::onClick(MouseButton mb, KeyState ks, int times, Point point)
{
    if (impl_->on_click_listener_)
        impl_->on_click_listener_->onClick(mb, ks, times, point);

    return true;
}

TextButton::TextButton(void)
    : Button()
{}

TextButton::TextButton(const std::wstring& caption)
    : TextButton(caption, Color::white())
{}

TextButton::TextButton(const std::wstring& caption, Color foreground_color)
    : TextButton()
{
    caption_          = caption;
    foreground_color_ = foreground_color;
}

TextButton::TextButton(const TextButton& button)
    : TextButton()
{
    operator=(button);
}

TextButton::~TextButton(void) {}

TextButton& TextButton::operator=(const TextButton& button)
{
    if (this != &button)
    {
        caption_          = button.caption_;
        foreground_color_ = button.foreground_color_;
        Button::operator=(button);
    }

    return *this;
}

void TextButton::caption(const std::wstring& caption)
{
    caption_ = caption;
}

std::wstring TextButton::caption(void) const
{
    return caption_;
}

void TextButton::foreground_color(Color foreground_color)
{
    foreground_color_ = foreground_color;
}

Color TextButton::foreground_color(void) const
{
    return foreground_color_;
}

void TextButton::onPaint(Graphics graphics)
{
    Button::onPaint(graphics);

    tfont_simple font;
    unsigned int caption_width = 0;

    for (wchar_t chr : caption_)
        caption_width += font.get_bitmap(chr)->advance;

    graphics.drawText((width()  - caption_width) / 2,
                      (height() - font.emheight()) / 2,
                      caption_.c_str(), font, foreground_color_);
}
