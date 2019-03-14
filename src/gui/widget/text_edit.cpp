#include <takisy/core/sys.h>
#include <takisy/core/codec.h>
#include <takisy/core/timer.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/basic/cursor.h>
#include <takisy/gui/widget/text_edit.h>
#include <takisy/gui/window.h>
#include "editbox.h"

namespace takisy {
namespace gui
{
    window::Handle handleFromLPWIDGET(const widget* widget);
}}

class text_edit::implement
{
    friend class editbox;
    friend class text_edit;

public:
    static const unsigned int lower_blink_interval = 200;

public:
    implement(text_edit& self)
        : self(self), border_visible_(false), fixed_brush_(false)
        , readonly_(false), caret_visible_(false)
        , blink_interval_(500), caret_timer_(100)
        , dragging_({.underway = false})
    {
        text_.margin(3);

        caret_timer_.start();
        caret_timer_.onTimer(
            [this, &self](timer* timer)
            {
                bool visible = ((timer->elapse() / blink_interval_) % 2) == 0
                               && self.focused()
                               && self.enabled()
                               && !readonly_
                               && !text_.outside_caret();

                if (caret_visible_ != visible)
                {
                    Point cp = text_.caret_point() - text_.offset();
                    self.repaint(cp.x, cp.y, 1, text_.font()->emheight());
                    caret_visible_ = visible;
                }
            });
    }

public:
    void update(void)
    {
        caret_timer_.restart();

        self.vertical_scroll().max(text_.height());
        self.vertical_scroll().value(text_.offset().y);
        self.horizontal_scroll().max(text_.width());
        self.horizontal_scroll().value(text_.offset().x);

        self.repaint();
    }

private:
    text_edit& self;
    class text text_;
    bool border_visible_;
    brush_sptr text_brush_, background_brush_, selection_brush_;
    bool fixed_brush_, readonly_, caret_visible_;
    unsigned int blink_interval_;
    timer caret_timer_;
    struct {
        bool underway;
        long mode;
        Point milestone;
    } dragging_;
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
    : impl_(new implement(*this))
{
    text(_text);

    impl_->text_.onCaretPointChanged(
        [this](class text* self)
        {
            window::Handle hdl = takisy::gui::handleFromLPWIDGET(forefather());
            if (hdl)
            {
                Point cp = window_xy()
                         + impl_->text_.caret_point()
                         - impl_->text_.offset();

                HIMC himc = ImmGetContext(hdl);

                LOGFONT lf;
                if (ImmGetCompositionFont(himc, &lf))
                    cp.y -= lf.lfHeight;

                COMPOSITIONFORM cf;
                cf.dwStyle = CFS_POINT;
                cf.ptCurrentPos.x = cp.x;
                cf.ptCurrentPos.y = cp.y + impl_->text_.font()->emheight();
                if (cf.ptCurrentPos.y < 0)
                    cf.ptCurrentPos.y = 0;

                ImmSetCompositionWindow(himc, &cf);
            }
        });
    impl_->text_.onCaretChanged(
        [this](class text* self)
        {
            onCaretChangedHandle();
        });
    impl_->text_.onTypewriting(
        [this](class text* self, std::wstring& text)
        {
            onTypewritingHandle(text);
        });
    impl_->text_.onErased(
        [this](class text* self, const std::wstring& text)
        {
            onErasedHandle(text);
        });
    impl_->text_.onContentChanged(
        [this](class text* self)
        {
            onTextChangedHandle();
        });
    impl_->text_.onSelectionChanged(
        [this](class text* self)
        {
            onSelectionChangedHandle();
        });
    impl_->text_.onCopyAvailable(
        [this](class text* self, bool yes)
        {
            onCopyAvailableHandle(yes);
        });
    impl_->text_.onUndoAvailable(
        [this](class text* self, bool yes)
        {
            onUndoAvailableHandle(yes);
        });
    impl_->text_.onRedoAvailable(
        [this](class text* self, bool yes)
        {
            onRedoAvailableHandle(yes);
        });

    vertical_scroll().min(0);
    vertical_scroll().step(impl_->text_.line_height() * 3);
    vertical_scroll().show();
    vertical_scroll().onScroll(
        [this](const scroll* self)
        {
            impl_->text_.offset_top(self->value());
            repaint();
        });

    horizontal_scroll().min(0);
    horizontal_scroll().step(36);
    horizontal_scroll().show();
    horizontal_scroll().onScroll(
        [this](const scroll* self)
        {
            impl_->text_.offset_left(self->value());
            repaint();
        });

    add(&vertical_scroll());
    add(&horizontal_scroll());
}

text_edit::~text_edit(void)
{
    delete impl_;
}

std::wstring text_edit::text(void) const
{
    return impl_->text_.content();
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

unsigned int text_edit::alignment(void) const
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

const class font* text_edit::font(void) const
{
    return impl_->text_.font();
}

bool text_edit::border_visible(void) const
{
    return impl_->border_visible_;
}

bool text_edit::fixed_brush(void) const
{
    return impl_->fixed_brush_;
}

brush_sptr text_edit::text_brush(void) const
{
    return impl_->text_brush_;
}

brush_sptr text_edit::background_brush(void) const
{
    return impl_->background_brush_;
}

brush_sptr text_edit::selection_brush(void) const
{
    return impl_->selection_brush_;
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

void text_edit::caret(unsigned int caret)
{
    impl_->text_.move(caret);
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

void text_edit::alignment(unsigned int alignment)
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

void text_edit::font(const class font* font)
{
    vertical_scroll().step(impl_->text_.line_height() * 3);
    impl_->text_.font(font);
    impl_->update();
}

void text_edit::border_visible(bool visible)
{
    if (impl_->border_visible_ != visible)
    {
        impl_->border_visible_ = visible;
        repaint();
    }
}

void text_edit::show_border(void)
{
    border_visible(true);
}

void text_edit::hide_border(void)
{
    border_visible(false);
}

void text_edit::text_color(const color& color)
{
    text_brush(make_spbrush<color_brush>(color));
}

void text_edit::text_brush(const brush_sptr& brush)
{
    if (impl_->text_brush_ != brush)
    {
        impl_->text_brush_ = brush;
        repaint();
    }
}

void text_edit::background_color(const color& color)
{
    background_brush(make_spbrush<color_brush>(color));
}

void text_edit::background_brush(const brush_sptr& brush)
{
    if (impl_->background_brush_ != brush)
    {
        impl_->background_brush_ = brush;
        repaint();
    }
}

void text_edit::selection_color(const color& color)
{
    selection_brush(make_spbrush<color_brush>(color));
}

void text_edit::selection_brush(const brush_sptr& brush)
{
    if (impl_->selection_brush_ != brush)
    {
        impl_->selection_brush_ = brush;
        repaint();
    }
}

void text_edit::fixed_brush(bool fixed)
{
    if (impl_->fixed_brush_ != fixed)
    {
        impl_->fixed_brush_ = fixed;
        repaint();
    }
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

void text_edit::undo(void)
{
    impl_->text_.undo();
}

void text_edit::redo(void)
{
    impl_->text_.redo();
}

unsigned int text_edit::hittest(Point point) const
{
    return impl_->text_.hittest(point);
}

void text_edit::cut(void)
{
    if (!impl_->readonly_)
    {
        copy();
        impl_->text_.erase(0);
        impl_->update();
    }
}

void text_edit::copy(void)
{
    std::string text = stralgo::encode(selected_text(), "gbk");
    if (!text.empty())
    {
        if (OpenClipboard(nullptr))
        {
            EmptyClipboard();
            HGLOBAL data = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,
                                       text.size() + 1);
            strncpy(reinterpret_cast<char*>(GlobalLock(data)),
                    text.c_str(), text.size());
            GlobalUnlock(data);
            SetClipboardData(CF_TEXT, data);
            CloseClipboard();
        }
    }
}

bool text_edit::paste(void)
{
    if (impl_->readonly_ || !OpenClipboard(nullptr))
        return false;
    else if (!IsClipboardFormatAvailable(CF_TEXT))
    {
        CloseClipboard();
        return false;
    }
    else
    {
        HANDLE data = GetClipboardData(CF_TEXT);
        std::wstring text =
            stralgo::decode(reinterpret_cast<char*>(GlobalLock(data)), "gbk");
        if (impl_->text_.typewrite(text))
            impl_->update();
        GlobalUnlock(data);
        CloseClipboard();
        return true;
    }
}

void text_edit::onSize(void)
{
    scroll_area::onSize();
    impl_->text_.view(width(), height());
    impl_->update();
}

void text_edit::onPaint(graphics graphics, Rect rect)
{
    Point offset(0, 0);
    if (!impl_->fixed_brush_)
        offset = impl_->text_.offset();

    if (impl_->background_brush_)
    {
        offset_brush::set(impl_->background_brush_, offset);
        graphics.fill_rectangle(rect, *impl_->background_brush_);
    }

    brush_sptr selbrush, wrtbrush;

    if (impl_->selection_brush_)
    {
        offset_brush::set(impl_->selection_brush_, offset);
        selbrush = impl_->selection_brush_;
    }
    else
    if (focused())
        selbrush.reset(new color_brush(color_scheme().selection()));
    else
        selbrush.reset(new color_brush(color_scheme().inactive_selection()));

    if (impl_->text_brush_)
    {
        offset_brush::set(impl_->text_brush_, offset);
        wrtbrush = impl_->text_brush_;
    }
    else
    if (focused())
        wrtbrush.reset(new color_brush(color_scheme().text()));
    else
        wrtbrush.reset(new color_brush(color_scheme().inactive_text()));

    impl_->text_.draw(graphics, rect, selbrush, *wrtbrush);

    if (impl_->caret_visible_)
    {
        Point cp = impl_->text_.caret_point() - impl_->text_.offset();

        graphics.draw_line(cp.x + 0.5, cp.y,
                           cp.x + 0.5, cp.y + impl_->text_.font()->emheight(),
                           make_spbrush<lambda_brush>(
            [&graphics](int x, int y) -> color
            {
                return ~graphics.pixel(x, y).opaque();
            }));
    }

    if (impl_->border_visible_)
    {
        color border = color_scheme().border();
        rectf rect   = client_rect();

        if (border.a)
            graphics.draw_rectangle(rect.inflate(-0.5), border);
    }
}

bool text_edit::onFocus(bool focus)
{
    impl_->update();

    return true;
}

bool text_edit::onSetCursor(Point)
{
    cursor::set(cursor::ctIBeam);

    return true;
}

bool text_edit::onKeyDown(sys::VirtualKey vkey)
{
    class text& text = impl_->text_;

    bool shift = window::key_pressed(sys::vkShift);
    bool ctrl  = window::key_pressed(sys::vkControl);

    switch (vkey)
    {
    case sys::vkLeft:
        if (ctrl)
            text.move_similar(true, shift);
        else
            text.move(text.caret() - 1, shift);
        impl_->update();
        break;
    case sys::vkRight:
        if (ctrl)
            text.move_similar(false, shift);
        else
            text.move(text.caret() + 1, shift);
        impl_->update();
        break;
    case sys::vkUp:
    case sys::vkDown:
        if (ctrl && !shift)
        {
            unsigned int height = text.line_height();
            if (vkey == sys::vkUp)
                vertical_scroll().value(vertical_scroll().value() - height);
            else
                vertical_scroll().value(vertical_scroll().value() + height);
        }
        else
        {
            Point cp = text.caret_point();
            if (vkey == sys::vkUp)
                cp.y -= text.line_height();
            else
                cp.y += text.line_height();
            text.move(text.hittest(cp), shift);
            impl_->update();
        }
        break;
    case sys::vkHome:
        if (ctrl)
            text.move(0, shift);
        else
            text.move(text.line_info(text.caret_line()).lrange.offset, shift);
        impl_->update();
        break;
    case sys::vkEnd:
        if (ctrl)
            text.move(text.content().size(), shift);
        else
            text.move([](const text::line_info::range& range)
                      {
                          return range.offset + range.words;
                      }(text.line_info(text.caret_line()).lrange), shift);
        impl_->update();
        break;
    case sys::vkPrior:
        vertical_scroll().page_up();
        break;
    case sys::vkNext:
        vertical_scroll().page_down();
        break;
    case sys::vkDelete:
        if (!impl_->readonly_)
        {
            if (ctrl)
            {
                if (!text.selected())
                    text.move_similar(false, true);
                text.erase();
            }
            else
                text.erase(1);
            impl_->update();
        }
    default:
        break;
    }

    return true;
}

bool text_edit::onKeyPress(unsigned int chr)
{
    bool ctrl;
    if (chr == 10)
        ctrl = false;
    else
        ctrl = window::key_pressed(sys::vkControl);

    if (ctrl)
    {
        chr += 'a' - 1;

        switch (chr)
        {
        case 223: // backspace
            if (!impl_->readonly_)
            {
                if (!impl_->text_.selected())
                    impl_->text_.move_similar(true, true);
                impl_->text_.erase();
                impl_->update();
            }
            break;
        case 'a':
            impl_->text_.select(0, impl_->text_.content().size());
            repaint();
            break;
        case 'd':
            impl_->text_.select_similar(impl_->text_.caret());
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
        case 'x':
            cut();
            break;
        case 'c':
            copy();
            break;
        case 'v':
            paste();
            break;
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
            chr =  10;
        if (impl_->text_.typewrite(codec::gbk2uni(chr)))
            impl_->update();
    }

    return true;
}

bool text_edit::onMouseDown(sys::Button button, int times, Point point)
{
    if (button == sys::btnLeft)
    {
        int caret = impl_->text_.hittest(point += impl_->text_.offset());

        impl_->dragging_.underway = true;

        if (times == 1)
        {
            bool shift = window::key_pressed(sys::vkShift);
            if (!shift)
                impl_->text_.cancel_select();
            impl_->text_.move(caret, shift);
            impl_->dragging_.mode = 1;
        }
        else
        {
            impl_->dragging_.mode = 2 + ((times - 2) % 2);

            switch (impl_->dragging_.mode)
            {
            case 2:
                impl_->text_.select_similar(caret);
                break;
            case 3:
                {
                    unsigned int lidx = impl_->text_.caret_line();
                    struct text::line_info li = impl_->text_.line_info(lidx);
                    impl_->text_.select(li.prange.offset,
                                        li.prange.offset + li.prange.words);
                }
                break;
            }
        }

        impl_->dragging_.milestone = point;
        impl_->update();

        capture(true);
    }

    return true;
}

bool text_edit::onMouseUp(sys::Button button, int times, Point point)
{
    capture(false);
    impl_->dragging_.underway = false;

    return true;
}

bool text_edit::onMouseMove(Point point)
{
    if (impl_->dragging_.underway)
    {
        int oldc = impl_->text_.hittest(impl_->dragging_.milestone);
        int nowc = impl_->text_.hittest(point + impl_->text_.offset());

        switch (impl_->dragging_.mode)
        {
        case 1:
            impl_->text_.move(nowc, true);
            break;
        case 2:
            {
                impl_->text_.select_similar(oldc);
                std::pair<int, int> sela = impl_->text_.selection();
                impl_->text_.select_similar(nowc);
                std::pair<int, int> selb = impl_->text_.selection();
                if (sela.first > selb.first)
                    impl_->text_.select(sela.second, selb.first);
                else
                    impl_->text_.select(sela.first, selb.second);
            }
            break;
        case 3:
            {
                struct text::line_info
                    lia = impl_->text_.line_info(impl_->text_.caret_line(oldc)),
                    lib = impl_->text_.line_info(impl_->text_.caret_line(nowc));
                if (lia.p > lib.p)
                    impl_->text_.select(lia.prange.offset + lia.prange.words,
                                        lib.prange.offset);
                else
                    impl_->text_.select(lia.prange.offset,
                                        lib.prange.offset + lib.prange.words);
            }
            break;
        }

        impl_->update();
    }

    return true;
}

editbox& editbox::pop(widget* father, const Rect& rect,
                      const class text& text, const std::wstring& content)
{
    static editbox eb;

    eb.horizontal_scroll().hide();
    eb.vertical_scroll().hide();

    eb.impl_->text_ = text;
    eb.impl_->text_.offset(0, 0);
    eb.impl_->text_.view(rect.width(), rect.height());
    if (!content.empty())
        eb.text(content);
    else
        eb.select(0, eb.text().size());

    if (father)
        father->add(&eb, 0);
    eb.rect(rect);
    eb.focus(true);
    eb.show();

    return eb;
}

bool editbox::onFocus(bool focus)
{
    if (!focus)
        return onKeyPress(27);

    return text_edit::onFocus(focus);
}

bool editbox::onKeyPress(unsigned int chr)
{
    if (chr == 13 || chr == 27)
    {
        if (chr == 13)
            onEditCompleteHandle(text());
        onEditFinishHandle();

        onEditCompleteReset();
        onEditFinishReset();

        father(nullptr);

        return true;
    }

    return text_edit::onKeyPress(chr);
}
