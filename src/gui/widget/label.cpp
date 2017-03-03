#include <takisy/core/sys.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/label.h>
#include "../basic/text.hpp"

class label::implement
{
    friend class label;

public:
    implement(label& self)
        : self(self), fixed_(true)
    {}

public:
    void update(void)
    {
        int a = text_.height(), b = self.height();

        switch (text_.alignment() & aVertical)
        {
        default:
        case aTop:    text_.offset_top(0); break;
        case aCenter: text_.offset_top((a - b) / 2); break;
        case aBottom: text_.offset_top( a - b); break;
        }

        self.repaint();
    }

private:
    label& self;
    class text text_;
    brush_sptr text_brush_, background_brush_;
    bool fixed_;
};

label::label(void)
    : label("")
{}

label::label(const std::string& text)
    : label(text, sys::default_codec())
{}

label::label(const std::string& text, const std::string& codec)
    : label(stralgo::decode(text, codec))
{}

label::label(const std::wstring& _text)
    : impl_(new implement(*this))
{
    text(_text);

    impl_->text_.onContentChanged(
        [this](class text* self)
        {
            onTextChangedHandle();
        });
}

label::~label(void)
{
    delete impl_;
}

const std::wstring& label::text(void) const
{
    return impl_->text_.content();
}

Margin label::margin(void) const
{
    return impl_->text_.margin();
}

int label::margin_left(void) const
{
    return impl_->text_.margin().left;
}

int label::margin_top(void) const
{
    return impl_->text_.margin().top;
}

int label::margin_right(void) const
{
    return impl_->text_.margin().right;
}

int label::margin_bottom(void) const
{
    return impl_->text_.margin().bottom;
}

unsigned int label::indent(void) const
{
    return impl_->text_.indent();
}

bool label::word_wrap(void) const
{
    return impl_->text_.word_wrap();
}

unsigned int label::line_spacing(void) const
{
    return impl_->text_.line_spacing();
}

unsigned int label::word_spacing(void) const
{
    return impl_->text_.word_spacing();
}

const class font* label::font(void) const
{
    return impl_->text_.font();
}

brush_sptr label::text_brush(void) const
{
    return impl_->text_brush_;
}

brush_sptr label::background_brush(void) const
{
    return impl_->background_brush_;
}

bool label::fixed_brush(void) const
{
    return impl_->fixed_;
}

unsigned int label::alignment(void) const
{
    return impl_->text_.alignment();
}

Size label::optimal(OptimalPolicy policy) const
{
    if (policy == opFixedWidth || !impl_->text_.word_wrap())
        return impl_->text_.world();
    else
    {
        class text t = impl_->text_;
        t.word_wrap(false);
        return t.world();
    }
}

void label::text(const std::string& _text)
{
    text(_text, sys::default_codec());
}

void label::text(const std::string& _text, const std::string& codec)
{
    text(stralgo::decode(_text, codec));
}

void label::text(const std::wstring& text)
{
    impl_->text_.content(text);
    impl_->update();
}

void label::margin(int _margin)
{
    margin(_margin, _margin, _margin, _margin);
}

void label::margin(const Margin& _margin)
{
    margin(_margin.left, _margin.top, _margin.right, _margin.bottom);
}

void label::margin(int left, int top, int right, int bottom)
{
    impl_->text_.margin(left, top, right, bottom);
    impl_->update();
}

void label::margin_left(int left)
{
    impl_->text_.margin_left(left);
    impl_->update();
}

void label::margin_top(int top)
{
    impl_->text_.margin_top(top);
    impl_->update();
}

void label::margin_right(int right)
{
    impl_->text_.margin_right(right);
    impl_->update();
}

void label::margin_bottom(int bottom)
{
    impl_->text_.margin_bottom(bottom);
    impl_->update();
}

void label::indent(unsigned int indent)
{
    impl_->text_.indent(indent);
    impl_->update();
}

void label::word_wrap(bool word_wrap)
{
    impl_->text_.word_wrap(word_wrap);
    impl_->update();
}

void label::line_spacing(unsigned int line_spacing)
{
    impl_->text_.line_spacing(line_spacing);
    impl_->update();
}

void label::word_spacing(unsigned int word_spacing)
{
    impl_->text_.word_spacing(word_spacing);
    impl_->update();
}

void label::font(const class font* font)
{
    impl_->text_.font(font);
    impl_->update();
}

void label::text_color(const color& text_color)
{
    text_brush(make_brushsptr<color_brush>(text_color));
}

void label::text_brush(const brush_sptr& text_brush)
{
    if (impl_->text_brush_ != text_brush)
    {
        impl_->text_brush_ = text_brush;
        repaint();
    }
}

void label::background_color(const color& background_color)
{
    background_brush(make_brushsptr<color_brush>(background_color));
}

void label::background_brush(const brush_sptr& background_brush)
{
    if (impl_->background_brush_ != background_brush)
    {
        impl_->background_brush_ = background_brush;
        repaint();
    }
}

void label::fixed_brush(bool fixed)
{
    if (impl_->fixed_ != fixed)
    {
        impl_->fixed_ = fixed;
        repaint();
    }
}

void label::alignment(unsigned int alignment)
{
    impl_->text_.alignment(alignment);
    impl_->update();
}

void label::onSize(void)
{
    impl_->text_.view(width(), height());
    impl_->update();
}

void label::onPaint(graphics graphics, Rect rect)
{
    Point offset(0, 0);
    if (!impl_->fixed_)
        offset = impl_->text_.offset();

    if (impl_->background_brush_)
    {
        offset_brush::set(impl_->background_brush_, offset);
        graphics.fill_rectangle(rect, *impl_->background_brush_);
    }

    brush_sptr wrtbrush;
    if (impl_->text_brush_)
    {
        offset_brush::set(impl_->text_brush_, offset);
        wrtbrush = impl_->text_brush_;
    }
    else
        wrtbrush.reset(new color_brush(color_scheme().text()));

    impl_->text_.draw(graphics, rect, nullptr, *wrtbrush);
}
