#include <takisy/core/sys.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/check.h>

class check::implement
{
    friend class check;

    static constexpr int boxsize = 12;
    static constexpr int spacing =  5;
    static constexpr int widgetx = boxsize + spacing;

public:
    implement(class widget* widget, bool checked)
        : checked_(checked), widget_(widget)
    {}

private:
    bool checked_;
    class widget* widget_;
};

check::check(void)
    : check(nullptr)
{}

check::check(class widget* widget)
    : check(widget, false)
{}

check::check(class widget* widget, bool checked)
    : impl_(new implement(widget, checked))
{
    if (widget)
        add(widget);
}

check::~check(void)
{
    delete impl_;
}

class widget* check::widget(void)
{
    return impl_->widget_;
}

const class widget* check::widget(void) const
{
    return impl_->widget_;
}

bool check::checked(void) const
{
    return impl_->checked_;
}

Size check::optimal(OptimalPolicy policy) const
{
    if (!impl_->widget_)
        return Size(implement::boxsize, implement::boxsize);
    else
    {
        Size optsize = impl_->widget_->optimal(policy);

        optsize.width += implement::boxsize + implement::spacing;
        if (optsize.height < implement::boxsize)
            optsize.height = implement::boxsize;

        return optsize;
    }
}

void check::widget(class widget* widget)
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
    if (impl_->widget_)
    {
        impl_->widget_->size(width() - implement::widgetx, height());
        impl_->widget_->xy(
            implement::widgetx,
            (height() - impl_->widget_->height()) / 2
        );
    }
}

void check::onPaint(graphics graphics, Rect)
{
    color color = color_scheme().theme();
    rectf rect;

    rect.left = 0;
    rect.top  = (int)(height() - implement::boxsize) / 2;
    rect.width(implement::boxsize);
    rect.height(implement::boxsize);

    graphics.draw_rectangle(rect.inflate(-0.5), color);
    if (checked())
        graphics.fill_rectangle(rect.inflate(-3), color);
}

bool check::onMouseDown(sys::Button button, int times, Point point)
{
    if (button == sys::btnLeft)
        checked(!checked());

    return true;
}

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
    : check(nullptr, checked), label_(text)
{
    label_.alignment(aLeft);
    label_.show();

    widget(&label_);
}

label& text_check::text(void)
{
    return label_;
}

const label& text_check::text(void) const
{
    return label_;
}
