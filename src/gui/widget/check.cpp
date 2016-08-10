#include <string>
#include <takisy/core/sys.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/label.h>
#include <takisy/gui/widget/check.h>

class check::implement
{
    friend class check;

    static constexpr int box_size = 12;
    static constexpr int spacing  =  4;

public:
    implement(widget* content, bool checked)
        : checked_(checked), content_(content)
    {}

private:
    bool checked_;
    widget* content_;
};

check::check(void)
    : check(nullptr)
{}

check::check(widget* content)
    : check(content, false)
{}

check::check(widget* content, bool checked)
    : impl_(new implement(content, checked))
{
    add(content);
}

check::~check(void)
{
    delete impl_;
}

widget* check::content(void) const
{
    return impl_->content_;
}

bool check::checked(void) const
{
    return impl_->checked_;
}

Size check::optimal_size(void) const
{
    Size optimal_size = impl_->content_->optimal_size();

    optimal_size.width += implement::box_size + implement::spacing;

    return optimal_size;
}

void check::content(widget* content)
{
    remove(impl_->content_);
    add((impl_->content_ = content));

    onSize();
}

void check::checked(bool checked)
{
    if (impl_->checked_ != checked)
    {
        impl_->checked_ = checked;
        onChecked();
        repaint();
    }
}

void check::onSize(void)
{
    if (impl_->content_)
    {
        impl_->content_->width(width() - impl_->content_->x());
        impl_->content_->height(impl_->content_->optimal_size().height);
        impl_->content_->x(implement::box_size + implement::spacing);
        impl_->content_->y(int(height() - impl_->content_->height()) / 2);
    }
}

void check::onPaint(graphics graphics, Rect rect)
{
    int   base = implement::box_size, bases1 = base - 1, bases3 = base - 3;
    int   y    = ((int)height() - base) / 2;
    color clr  = color_scheme()->main();

    graphics.draw_rectangle(0, y, bases1, y + bases1, clr);
    if (checked())
        graphics.fill_rectangle(3, y + 3, bases3, y + bases3, clr);
}

bool check::onClick(sys::MouseButton button, int times, Point point)
{
    if (button == sys::mbLButton)
    {
        checked(!checked());
        return true;
    }

    return false;
}

class text_check::implement
{
    friend class text_check;

public:
    implement(const std::wstring& text)
        : label_(text)
    {
        label_.show();
    }

private:
    label label_;
};

text_check::text_check(const std::string& text)
    : text_check(text, sys::default_codec())
{}

text_check::text_check(const std::string& text, bool checked)
    : text_check(text, sys::default_codec(), checked)
{}

text_check::text_check(const std::string& text, const std::string& codec)
    : text_check(stralgo::decode(text, codec))
{}

text_check::text_check(const std::string& text, const std::string& codec,
                       bool checked)
    : text_check(stralgo::decode(text, codec), checked)
{}

text_check::text_check(const std::wstring& text)
    : text_check(text, false)
{}

text_check::text_check(const std::wstring& text, bool checked)
    : check(nullptr, checked), impl_(new implement(text))
{
    content(&impl_->label_);
}

text_check::~text_check(void)
{
    delete impl_;
}

bool text_check::word_wrap(void) const
{
    return impl_->label_.word_wrap();
}

std::wstring text_check::text(void) const
{
    return impl_->label_.text();
}

const class font& text_check::font(void) const
{
    return impl_->label_.font();
}

void text_check::word_wrap(bool word_wrap)
{
    impl_->label_.word_wrap(word_wrap);

    onSize();
}

void text_check::text(const std::string& text)
{
    impl_->label_.text(text);

    onSize();
}

void text_check::text(const std::string& text, const std::string& codec)
{
    impl_->label_.text(text, codec);

    onSize();
}

void text_check::text(const std::wstring& text)
{
    impl_->label_.text(text);

    onSize();
}

void text_check::font(const class font& font)
{
    impl_->label_.font(font);

    onSize();
}
