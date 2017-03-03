#include <takisy/core/sys.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/check.h>

class check::implement
{
    friend class check;

    static constexpr int boxsize = 12;
    static constexpr int spacing =  5;

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

Size check::optimal(OptimalPolicy policy) const
{
    Size optsize = impl_->content_->optimal(policy);

    optsize.width += implement::boxsize + implement::spacing;

    return optsize;
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
        onCheckedHandle();
        repaint();
    }
}

void check::onSize(void)
{
    if (impl_->content_)
    {
        widget*& content = impl_->content_;

        content->x(implement::boxsize + implement::spacing);
        content->width(width() - content->x());
        content->height(content->optimal(opFixedWidth).height);
        content->y(int(height() - content->height()) / 2);
    }
}

void check::onPaint(graphics graphics, Rect)
{
    color color = color_scheme().theme();
    rectf rect;

    rect.left = 0;
    rect.top  = ((int)height() - implement::boxsize) / 2;
    rect.width(implement::boxsize);
    rect.height(implement::boxsize);

    graphics.draw_rectangle(rect.inflate(-0.5), color);
    if (checked())
        graphics.fill_rectangle(rect.inflate(-3), color);
}

bool check::onMouseDown(sys::Button button, int times, Point point)
{
    if (button == sys::btnLeft)
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
    : text_check(text, sys::default_codec(), false)
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

label& text_check::text(void)
{
    return impl_->label_;
}

const label& text_check::text(void) const
{
    return impl_->label_;
}
