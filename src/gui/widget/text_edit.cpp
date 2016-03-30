#include <ctime>
#include <takisy/core/sys.h>
#include <takisy/core/codec.h>
#include <takisy/core/timer.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/basic/cursor.h>
#include <takisy/gui/widget/scroll.h>
#include <takisy/gui/widget/text_edit.h>
#include <takisy/gui/cross_platform_window.h>
#include "../basic/text.hpp"

namespace takisy
{
    cross_platform_window::Handle handleFromLPWIDGET(const widget* widget);
}

class text_edit::implement
{
    friend class text_edit;

public:
    static const unsigned int lower_blink_interval = 200;

public:
    implement(text_edit* _this)
        : this_(_this), border_color_(color::black(0))
        , readonly_(false), focused_(false), caret_visible_(false)
        , blink_interval_(500), caret_timer_(100)
    {
        text_.margin(3);

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
        Point caret = text_.caret_point() + text_.offset();

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
    text_edit* this_;
    class text text_;
    color border_color_;
    brush_sptr background_brush_;
    class vertical_scroll vscroll_;
    class horizontal_scroll hscroll_;
    bool readonly_, focused_, caret_visible_;
    unsigned int blink_interval_;
    timer caret_timer_;
};

text_edit::text_edit(void)
    : text_edit(L"")
{}

text_edit::text_edit(const std::string& text)
    : text_edit(text, sys::default_codec())
{}

text_edit::text_edit(const std::string& text, const std::string& codec)
    : text_edit(stralgo::decode(text, codec))
{}

text_edit::text_edit(const std::wstring& _text)
    : impl_(new implement(this))
{
    attribute("intercept.onClick", true);
    add(&impl_->hscroll_);
    add(&impl_->vscroll_);
    text(_text);

    impl_->text_.onCaretPositionChanged(
        [this](class text& text)
        {
            cross_platform_window::Handle handle =
                    takisy::handleFromLPWIDGET(forefather());
            if (handle)
            {
                Point point = window_xy()
                            + impl_->text_.caret_point()
                            + impl_->text_.offset();

            #if defined(__WINNT__) || defined(__CYGWIN__)
                HIMC himc = ImmGetContext(handle);

                LOGFONT lf;
                if (ImmGetCompositionFont(himc, &lf))
                    point.y -= lf.lfHeight;

                COMPOSITIONFORM cf;
                cf.dwStyle = CFS_POINT;
                cf.ptCurrentPos.x = point.x;
                cf.ptCurrentPos.y = point.y + impl_->text_.font()->emheight();

                ImmSetCompositionWindow(himc, &cf);
            #endif
            }
        });
}

text_edit::~text_edit(void)
{
    delete impl_;
}

std::string text_edit::text(const std::string& codec) const
{
    return stralgo::encode(impl_->text_.content(), codec);
}

std::wstring text_edit::text(void) const
{
    return impl_->text_.content();
}

std::string text_edit::selected_text(const std::string& codec) const
{
    return stralgo::encode(impl_->text_.selected_content(), codec);
}

std::wstring text_edit::selected_text(void) const
{
    return impl_->text_.selected_content();
}

unsigned int text_edit::caret(void) const
{
    return impl_->text_.caret();
}

Margin text_edit::margin(void) const
{
    return impl_->text_.margin();
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

Alignment text_edit::alignment(void) const
{
    return impl_->text_.alignment();
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

color text_edit::border_color(void) const
{
    return impl_->border_color_;
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

Point text_edit::offset(void) const
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

vertical_scroll& text_edit::vertical_scroll(void)
{
    return impl_->vscroll_;
}

horizontal_scroll& text_edit::horizontal_scroll(void)
{
    return impl_->hscroll_;
}

void text_edit::text(const std::string& _text)
{
    text(_text, sys::default_codec());
}

void text_edit::text(const std::string& _text, const std::string& codec)
{
    text(stralgo::decode(_text, codec));
}

void text_edit::text(const std::wstring& text)
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

void text_edit::margin(const Margin& _margin)
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
    impl_->text_.alignment(alignment);
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

void text_edit::border_color(const color& color)
{
    impl_->border_color_ = color;
    repaint();
}

void text_edit::selection_color(const color& color)
{
    selection_brush(make_color_brush_sptr(color));
}

void text_edit::selection_brush(const brush_sptr& brush)
{
    impl_->text_.selection_brush(brush);
    repaint();
}

void text_edit::background_color(const color& color)
{
    background_brush(make_color_brush_sptr(color));
}

void text_edit::background_brush(const brush_sptr& brush)
{
    impl_->background_brush_ = brush;
    repaint();
}

void text_edit::foreground_color(const color& color)
{
    foreground_brush(make_color_brush_sptr(color));
}

void text_edit::foreground_brush(const brush_sptr& brush)
{
    impl_->text_.writing_brush(brush);
    repaint();
}

void text_edit::fixed_brush(bool fixed)
{
    impl_->text_.fixed_brush(fixed);
    repaint();
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

void text_edit::onSize(void)
{
    impl_->vscroll_.rect(width() - 7, 0, 7, height() - 7);
    impl_->hscroll_.rect(0, height() - 7, width() - 7, 7);
    impl_->text_.view(width(), height());
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
        Point caret = impl_->text_.caret_point() + impl_->text_.offset();

        graphics.draw_line(caret.x, caret.y,
                           caret.x, caret.y + impl_->text_.font()->emheight(),
                           make_lambda_brush(
                                [&graphics](int x, int y) -> color
                                {
                                    return ~graphics.pixel(x, y).opaque();
                                }));
    }

    if (impl_->border_color_.a > 0)
        graphics.draw_rectangle(client_rect().expand(-1), impl_->border_color_);
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
            Point point = impl_->text_.caret_point();
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
    #if defined(__WINNT__) || defined(__CYGWIN__)
        case 'c':
        case 'x':
            if (OpenClipboard(nullptr))
            {
                const text_edit* constthis = const_cast<const text_edit*>(this);
                std::string selected_text  = constthis->selected_text("gbk");
                if (!selected_text.empty())
                {
                    EmptyClipboard();
                    HGLOBAL data = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,
                                               selected_text.size() + 1);
                    strncpy(reinterpret_cast<char*>(GlobalLock(data)),
                            selected_text.c_str(), selected_text.size());
                    GlobalUnlock(data);
                    SetClipboardData(CF_TEXT, data);
                    CloseClipboard();
                }
                if (!impl_->readonly_ && chr == 'x')
                {
                    impl_->text_.erase(0);
                    impl_->update();
                }
            }
            break;
        case 'v':
            if (!impl_->readonly_ && OpenClipboard(nullptr))
            {
                if (IsClipboardFormatAvailable(CF_TEXT))
                {
                    HANDLE data = GetClipboardData(CF_TEXT);
                    std::wstring text = stralgo::decode(
                            reinterpret_cast<char*>(GlobalLock(data)), "gbk");
                    if (impl_->text_.typewrite(text))
                        impl_->update();
                    GlobalUnlock(data);
                }
                CloseClipboard();
            }
            break;
    #endif
        }
    }
    else
    if (!impl_->readonly_ && chr == 8) // backspace
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
            impl_->update();
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
