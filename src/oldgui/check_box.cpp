#include <takisy/core/sys.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/cgl/font/tfont_simple.h>
#include <takisy/oldgui/widget/check_box.h>

class CheckBox::Implement
{
    friend class CheckBox;

public:
    Implement(const std::string& caption, bool checked)
        : caption_(stralgo::decode(caption, sys::default_codec()))
        , checked_(checked)
        , handler_(nullptr)
    {}

private:
    std::wstring caption_;
    bool checked_;
    std::shared_ptr<Handler> handler_;
};

CheckBox::CheckBox(const std::string& caption)
    : CheckBox(caption, false)
{}

CheckBox::CheckBox(const std::string& caption, bool checked)
    : impl_(new Implement(caption, checked))
{}

CheckBox::~CheckBox(void)
{
    delete impl_;
}

bool CheckBox::checked(void) const
{
    return impl_->checked_;
}

std::string CheckBox::caption(void) const
{
    return stralgo::encode(impl_->caption_, sys::default_codec());
}

void CheckBox::check(bool checked)
{
    if (impl_->checked_ != checked)
    {
        impl_->checked_ = checked;
        if (impl_->handler_)
            impl_->handler_->process(this);
        repaint();
    }
}

void CheckBox::caption(const std::string& caption)
{
    impl_->caption_ = stralgo::decode(caption, sys::default_codec());
    repaint();
}

void CheckBox::onStatusChange(const std::shared_ptr<Handler>& handler)
{
    impl_->handler_ = handler;
}

void CheckBox::onPaint(Graphics graphics)
{
    static const tfont_simple* font = tfont_simple::get();
    int base = font->emheight(), y = (height() - base) / 2;
    int bases1 = base - 1, bases3 = base - 3;

    graphics.drawRectangle(0, y, bases1, y + bases1, Color::green());
    if (checked())
        graphics.fillRectangle(3, y + 3, bases3, y + bases3, Color::green());

    graphics.drawText(base + 5, y, impl_->caption_.c_str(),
                      *font, Color::black());
}

bool CheckBox::onClick(MouseButton mb, KeyState ks, int times, Point point)
{
    if (mb == mbLeftButton)
    {
        check(!checked());

        return true;
    }

    return false;
}
