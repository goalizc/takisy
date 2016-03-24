#include <takisy/core/sys.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/label.h>
#include "../basic/text.hpp"

class label::implement
{
    friend class label;

public:
    implement(label* _this)
        : this_(_this)
    {}

public:
    void update(void)
    {
        int a = this_->height();
        int b = text_.height();

        switch (text_.alignment())
        {
        default:
        case aTop:    text_.offset_top(0); break;
        case aCenter: text_.offset_top((a - b) / 2); break;
        case aBottom: text_.offset_top( a - b); break;
        }

        this_->repaint();
    }

private:
    label* this_;
    class text text_;
    brush_sptr background_brush_;
};

label::label(void)
    : label("")
{}

label::label(const char* text)
    : label(text, sys::default_codec())
{}

label::label(const char* text, const char* codec)
    : label(stralgo::decode(text, codec).c_str())
{}

label::label(const wchar_t* _text)
    : impl_(new implement(this))
{
    text(_text);
}

label::~label(void)
{
    delete impl_;
}

const char* label::text(const char* codec) const
{
    return stralgo::encode(impl_->text_.content().c_str(), codec).c_str();
}

const wchar_t* label::text(void) const
{
    return impl_->text_.content().c_str();
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

const class font& label::font(void) const
{
    return *impl_->text_.font();
}

brush_sptr label::background_brush(void) const
{
    return impl_->background_brush_;
}

brush_sptr label::foreground_brush(void) const
{
    return impl_->text_.writing_brush();
}

bool label::fixed_brush(void) const
{
    return impl_->text_.fixed_brush();
}

Alignment label::alignment(void) const
{
    return impl_->text_.alignment();
}

Size label::optimal_size(void) const
{
    return impl_->text_.world().size();
}

void label::text(const char* _text)
{
    text(_text, sys::default_codec());
}

void label::text(const char* _text, const char* codec)
{
    text(stralgo::decode(_text, codec).c_str());
}

void label::text(const wchar_t* text)
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

void label::font(const class font& font)
{
    impl_->text_.font(&font);
    impl_->update();
}

void label::background_color(const color& background_color)
{
    background_brush(make_color_brush_sptr(background_color));
}

void label::background_brush(const brush_sptr& background_brush)
{
    impl_->background_brush_ = background_brush;
    repaint();
}

void label::foreground_color(const color& foreground_color)
{
    foreground_brush(make_color_brush_sptr(foreground_color));
}

void label::foreground_brush(const brush_sptr& foreground_brush)
{
    impl_->text_.writing_brush(foreground_brush);
    repaint();
}

void label::fixed_brush(bool fixed)
{
    impl_->text_.fixed_brush(fixed);
    repaint();
}

void label::alignment(Alignment alignment)
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
    if (impl_->background_brush_)
        graphics.clear(*impl_->background_brush_);
    impl_->text_.draw(graphics, rect);
}
