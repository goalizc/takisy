#include <ctime>
#include <takisy/core/sys.h>
#include <takisy/core/codec.h>
#include <takisy/core/timer.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/basic/cursor.h>
#include <takisy/gui/widget/scroll.h>
#include <takisy/gui/widget/text_edit.h>
#include "../basic/text.hpp"
#include "../basic/window.h"

class text_edit::implement
{
    friend class text_edit;

public:
    static const unsigned int lower_blink_interval = 200;

public:
    implement(text_edit* _this)
        : readonly_(false), this_(_this)
        , focused_(false), caret_visible_(false), blink_interval_(500)
        , caret_timer_(100)
    {
        vscroll_.show();
        vscroll_.step(text_.line_height() * 3);
        vscroll_.onScroll(
            [this](const scroll& scroll)
            {
                text_.offset_top(-scroll.value());
                this_->repaint();
            });

        hscroll_.show();
        hscroll_.step(36);
        hscroll_.onScroll(
            [this](const scroll& scroll)
            {
                text_.offset_left(-scroll.value());
                this_->repaint();
            });

        caret_timer_.start();
        caret_timer_.onTimer(
            [this](timer& timer)
            {
                bool visible = ((timer.elapse() / blink_interval_) % 2) == 0
                               && focused_ && !readonly_ && !outside_caret();

                if (caret_visible_ != visible)
                {
                    caret_visible_ = visible;
                    this_->repaint();
                }
            });
    }

public:
    inline bool outside_caret(void) const
    {
        widget::Point caret = text_.caret_point() + text_.offset();

        return caret.x < 0
            || caret.x > text_.view().width
            || caret.y + text_.font()->emheight() < 0
            || caret.y > text_.view().height;
    }

    void update(void)
    {
        caret_timer_.restart();

        vscroll_.range(0, text_.height());
        vscroll_.page (   text_.view().height);
        vscroll_.value(  -text_.offset().y);

        hscroll_.range(0, text_.width());
        hscroll_.page (   text_.view().width);
        hscroll_.value(  -text_.offset().x);

        this_->repaint();
    }

private:
    class text text_;
    bool readonly_;
    brush_sptr background_brush_;
    text_edit* this_;
    vertical_scroll vscroll_;
    horizontal_scroll hscroll_;
    bool focused_;
    bool caret_visible_;
    unsigned int blink_interval_;
    timer caret_timer_;
};

text_edit::text_edit(void)
    : text_edit(L"")
{}

text_edit::text_edit(const char* text)
    : text_edit(text, sys::default_codec())
{}

text_edit::text_edit(const char* text, const char* codec)
    : text_edit(stralgo::decode(text, codec).c_str())
{}

text_edit::text_edit(const wchar_t* _text)
    : impl_(new implement(this))
{
    add(&impl_->hscroll_);
    add(&impl_->vscroll_);

    margin(3);
    text(_text);

    attribute("intercept.onClick", true);
}

text_edit::~text_edit(void)
{
    delete impl_;
}

const char* text_edit::text(const char* codec) const
{
    return stralgo::encode(impl_->text_.content().c_str(), codec).c_str();
}

const wchar_t* text_edit::text(void) const
{
    return impl_->text_.content().c_str();
}

const char* text_edit::selected_text(const char* codec) const
{
    return stralgo::encode(impl_->text_.selected_content().c_str(),
                           codec).c_str();
}

const wchar_t* text_edit::selected_text(void) const
{
    return impl_->text_.selected_content().c_str();
}

unsigned int text_edit::caret(void) const
{
    return impl_->text_.caret();
}

struct text_edit::margin text_edit::margin(void) const
{
    struct margin margin;

    margin.left   = impl_->text_.margin().left;
    margin.top    = impl_->text_.margin().top;
    margin.right  = impl_->text_.margin().right;
    margin.bottom = impl_->text_.margin().bottom;

    return margin;
}

int text_edit::margin_left(void) const
{
    return impl_->text_.margin().left;
}

int text_edit::margin_top(void) const
{
    return impl_->text_.margin().top;
}

int text_edit::margin_right(void) const
{
    return impl_->text_.margin().right;
}

int text_edit::margin_bottom(void) const
{
    return impl_->text_.margin().bottom;
}

unsigned int text_edit::indent(void) const
{
    return impl_->text_.indent();
}

text_edit::Alignment text_edit::alignment(void) const
{
    switch (impl_->text_.alignment())
    {
    default:            return aLeft;
    case text::aCenter: return aCenter;
    case text::aRight:  return aRight;
    }
}

bool text_edit::readonly(void) const
{
    return impl_->readonly_;
}

bool text_edit::multiline(void) const
{
    return impl_->text_.multiline();
}

bool text_edit::word_wrap(void) const
{
    return impl_->text_.word_wrap();
}

unsigned int text_edit::line_spacing(void) const
{
    return impl_->text_.line_spacing();
}

unsigned int text_edit::word_spacing(void) const
{
    return impl_->text_.word_spacing();
}

const class font& text_edit::font(void) const
{
    return *impl_->text_.font();
}

brush_sptr text_edit::selection_brush(void) const
{
    return impl_->text_.selection_brush();
}

brush_sptr text_edit::background_brush(void) const
{
    return impl_->background_brush_;
}

brush_sptr text_edit::foreground_brush(void) const
{
    return impl_->text_.writing_brush();
}

bool text_edit::fixed_brush(void) const
{
    return impl_->text_.fixed_brush();
}

widget::Point text_edit::offset(void) const
{
    return -impl_->text_.offset();
}

int text_edit::offset_x(void) const
{
    return -impl_->text_.offset().x;
}

int text_edit::offset_y(void) const
{
    return -impl_->text_.offset().y;
}

unsigned int text_edit::max_length(void) const
{
    return impl_->text_.max_length();
}

unsigned int text_edit::blink_interval(void) const
{
    return impl_->blink_interval_;
}

void text_edit::text(const char* _text)
{
    text(_text, sys::default_codec());
}

void text_edit::text(const char* _text, const char* codec)
{
    text(stralgo::decode(_text, codec).c_str());
}

void text_edit::text(const wchar_t* text)
{
    impl_->text_.content(text);
    impl_->update();
}

void text_edit::select(unsigned int offset, unsigned int count)
{
    impl_->text_.select(offset, count);
    impl_->update();
}

void text_edit::caret(unsigned int caret_position)
{
    impl_->text_.move(caret_position);
    impl_->update();
}

void text_edit::margin(int _margin)
{
    margin(_margin, _margin, _margin, _margin);
}

void text_edit::margin(struct margin& _margin)
{
    margin(_margin.left, _margin.top, _margin.right, _margin.bottom);
}

void text_edit::margin(int left, int top, int right, int bottom)
{
    impl_->text_.margin(left, top, right, bottom);
    impl_->update();
}

void text_edit::margin_left(int left)
{
    impl_->text_.margin_left(left);
    impl_->update();
}

void text_edit::margin_top(int top)
{
    impl_->text_.margin_top(top);
    impl_->update();
}

void text_edit::margin_right(int right)
{
    impl_->text_.margin_right(right);
    impl_->update();
}

void text_edit::margin_bottom(int bottom)
{
    impl_->text_.margin_bottom(bottom);
    impl_->update();
}

void text_edit::indent(unsigned int indent)
{
    impl_->text_.indent(indent);
    impl_->update();
}

void text_edit::alignment(Alignment alignment)
{
    switch (alignment)
    {
    default:      impl_->text_.alignment(text::aLeft);   break;
    case aCenter: impl_->text_.alignment(text::aCenter); break;
    case aRight:  impl_->text_.alignment(text::aRight);  break;
    }

    repaint();
}

void text_edit::readonly(bool readonly)
{
    impl_->readonly_ = readonly;
}

void text_edit::multiline(bool multiline)
{
    impl_->text_.multiline(multiline);
    impl_->update();
}

void text_edit::word_wrap(bool word_wrap)
{
    impl_->text_.word_wrap(word_wrap);
    impl_->update();
}

void text_edit::line_spacing(unsigned int line_spacing)
{
    impl_->text_.line_spacing(line_spacing);
    impl_->update();
}

void text_edit::word_spacing(unsigned int word_spacing)
{
    impl_->text_.word_spacing(word_spacing);
    impl_->update();
}

void text_edit::font(const class font& font)
{
    impl_->text_.font(&font);
    impl_->vscroll_.step(impl_->text_.line_height() * 3);
    impl_->update();
}

void text_edit::selection_color(const color& selection_color)
{
    selection_brush(make_color_brush_sptr(selection_color));
}

void text_edit::selection_brush(const brush_sptr& selection_brush)
{
    impl_->text_.selection_brush(selection_brush);
    repaint();
}

void text_edit::background_color(const color& background_color)
{
    background_brush(make_color_brush_sptr(background_color));
}

void text_edit::background_brush(const brush_sptr& background_brush)
{
    impl_->background_brush_ = background_brush;
    repaint();
}

void text_edit::foreground_color(const color& foreground_color)
{
    foreground_brush(make_color_brush_sptr(foreground_color));
}

void text_edit::foreground_brush(const brush_sptr& foreground_brush)
{
    impl_->text_.writing_brush(foreground_brush);
    repaint();
}

void text_edit::fixed_brush(bool fixed)
{
    impl_->text_.fixed_brush(fixed);
    repaint();
}

void text_edit::scroll_color(const color& color)
{
    impl_->vscroll_.color(color);
    impl_->hscroll_.color(color);
}

void text_edit::scroll_visible(bool visible)
{
    impl_->vscroll_.visible(visible);
    impl_->hscroll_.visible(visible);
}

void text_edit::max_length(unsigned int max_length)
{
    impl_->text_.max_length(max_length);
}

void text_edit::blink_interval(unsigned int blink_interval)
{
    if (blink_interval < implement::lower_blink_interval)
        blink_interval = implement::lower_blink_interval;

    impl_->blink_interval_ = blink_interval;
}

void text_edit::onSize(Size size)
{
    impl_->vscroll_.rect(size.width - 7, 0, 7, size.height - 7);
    impl_->hscroll_.rect(0, size.height - 7, size.width - 7, 7);
    impl_->text_.view(size.width, size.height);
    impl_->update();
}

void text_edit::onPaint(graphics graphics, Rect rect)
{
    if (impl_->background_brush_)
    {
        if (impl_->text_.fixed_brush())
            impl_->background_brush_->offset(0, 0);
        else
            impl_->background_brush_->offset(offset_x(), offset_y());

        graphics.fill_rectangle(rect, *impl_->background_brush_);
    }

    impl_->text_.draw(graphics, rect);

    if (impl_->caret_visible_)
    {
        widget::Point caret = impl_->text_.caret_point()
                            + impl_->text_.offset();

        graphics.draw_line(caret.x, caret.y,
                           caret.x, caret.y + impl_->text_.font()->emheight(),
                           make_lambda_brush(
                                [&graphics](int x, int y) -> color
                                {
                                    return ~graphics.pixel(x, y);
                                }));
    }
}

bool text_edit::onFocus(bool focus)
{
    impl_->focused_ = focus;
    impl_->update();

    return true;
}

bool text_edit::onSetCursor(void)
{
    cursor::set(cursor::ctIBeam);

    return true;
}

bool text_edit::onKeyDown(sys::VirtualKey vkey)
{
    bool shift = sys::key_pressed(sys::vkShift);
    bool ctrl  = sys::key_pressed(sys::vkControl);

    switch (vkey)
    {
    case sys::vkLeft:
        impl_->text_.move(caret() - 1, shift, ctrl);
        impl_->update();
        break;
    case sys::vkRight:
        impl_->text_.move(caret() + 1, shift, ctrl);
        impl_->update();
        break;
    case sys::vkUp:
    case sys::vkDown:
        {
            widget::Point point = impl_->text_.caret_point();
            if (vkey == sys::vkUp)
                point.y -= impl_->text_.line_height();
            else
                point.y += impl_->text_.line_height();
            impl_->text_.move(impl_->text_.hittest(point), shift, ctrl);
            impl_->update();
        }
        break;
    case sys::vkHome:
    case sys::vkEnd:
        if (ctrl)
        {
            if (vkey == sys::vkHome)
                impl_->text_.move(0, shift, ctrl);
            else
                impl_->text_.move(impl_->text_.content().size(), shift, ctrl);
        }
        else
        {
            std::pair<unsigned int, text::line>
                range = impl_->text_.line_range(impl_->text_.caret_line());
            if (vkey == sys::vkHome)
                impl_->text_.move(range.first, shift, ctrl);
            else
                impl_->text_.move(range.first + range.second.words,
                                  shift, ctrl);
        }
        impl_->update();
        break;
    case sys::vkDelete:
        if (!impl_->readonly_)
        {
            impl_->text_.erase(1);
            impl_->update();
        }
    default:
        break;
    }

    return true;
}

bool text_edit::onKeyPress(unsigned int chr)
{
    static std::wstring clip_text;

    if (sys::key_pressed(sys::vkControl))
    {
        chr += 'a' - 1;

        switch (chr)
        {
        case 'a':
            impl_->text_.select(0, impl_->text_.content().size());
            repaint();
            break;
        case 'y':
            if (!impl_->readonly_ && impl_->text_.redo())
                impl_->update();
            break;
        case 'z':
            if (!impl_->readonly_ && impl_->text_.undo())
                impl_->update();
            break;
        case 'c':
        case 'x':
            {
                std::wstring selected_text = impl_->text_.selected_content();
                if (!selected_text.empty())
                    clip_text = selected_text;
                if (chr == 'x')
                {
                    impl_->text_.erase(0);
                    impl_->update();
                }
            }
            break;
        case 'v':
            if (!clip_text.empty() && impl_->text_.typewrite(clip_text.c_str()))
                impl_->update();
            break;
        }
    }
    else
    if (chr == 8 && !impl_->readonly_) // backspace
    {
        impl_->text_.erase(-1);
        impl_->update();
    }
    else
    if (chr == 27) // esc
    {
        impl_->text_.cancel_select();
        repaint();
    }
    else
    if (!impl_->readonly_)
    {
        if (chr == 13) // enter
            chr  = 10;
        if (impl_->text_.typewrite(codec::gbk2unicode(chr)))
        {
            impl_->update();

            Window* wnd = Window::find(forefather());
            if (wnd)
            {
                point cp = impl_->text_.caret_point() + impl_->text_.offset();
                cp = cp + screen_xy() - forefather()->xy();
                cp.y += impl_->text_.font()->emheight();
                wnd->setCompositionWindow(cp.x, cp.y);
            }
        }
    }

    return true;
}

bool text_edit::onMouseDown(sys::MouseButton button, int times, Point point)
{
    if (button == sys::mbLButton)
    {
        unsigned int caret =
                impl_->text_.hittest(point - impl_->text_.offset());

        if (times == 1)
        {
            bool shift = sys::key_pressed(sys::vkShift);
            if (!shift)
                impl_->text_.cancel_select();
            impl_->text_.move(caret, shift);
            impl_->update();
            capture(true);
        }
        else
        {
            switch ((times - 2) % 2)
            {
            case 0:
                impl_->text_.select_similar(caret);
                break;
            case 1:
                {
                    unsigned int index  = impl_->text_.caret_paragraph(caret);
                    unsigned int offset =
                            impl_->text_.paragraph_range(index).first;
                    unsigned int words  = impl_->text_.paragraph_words(index);
                    impl_->text_.select(offset, offset + words);
                    impl_->update();
                }
                break;
            }

            impl_->update();
        }
    }

    return true;
}

bool text_edit::onMouseUp(sys::MouseButton button, Point point)
{
    capture(false);

    return true;
}

bool text_edit::onMouseMove(Point point)
{
    if (sys::key_pressed(sys::vkLButton))
    {
        point -= impl_->text_.offset();
        impl_->text_.move(impl_->text_.hittest(point), true);
        impl_->update();
    }

    return true;
}

bool text_edit::onMouseWheel(int delta, Point point)
{
    if (impl_->vscroll_.scrollable())
    {
        if (delta < 0)
            impl_->vscroll_.step_down();
        else
            impl_->vscroll_.step_up();
    }
    else
    if (impl_->hscroll_.scrollable())
    {
        if (delta < 0)
            impl_->hscroll_.step_down();
        else
            impl_->hscroll_.step_up();
    }

    return true;
}
