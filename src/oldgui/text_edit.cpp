#include <list>
#include <vector>
#include <memory>
#include <takisy/core/sys.h>
#include <takisy/core/codec.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/cgl/basic/canvas.h>
#include <takisy/cgl/font/tfont_simple.h>
#include <takisy/oldgui/widget/scroll_bar.h>
#include <takisy/oldgui/widget/text_edit.h>

class TextEdit::Implement
{
    friend class TextEdit;

    #include "text.hpp"

public:
    Implement(const std::wstring& text)
        : text_(text), offset_(3, 3), readonly_(false)
        , background_brush_(nullptr)
        , sb_visible_({false, false})
        , timestamp_(0), blink_interval_(500)
    {
        ver_scroll_bar_.vertical(true);
        ver_scroll_bar_.hide();
        hor_scroll_bar_.hide();
    }

public:
    void update(Widget* thisw)
    {
        unsigned int width   = text_.width();
        unsigned int height  = text_.height();
        unsigned int cwidth  = text_.canvas().width;
        unsigned int cheight = text_.canvas().height;

        if (width > cwidth)
        {
            hor_scroll_bar_.range(0, width);
            hor_scroll_bar_.page(cwidth);
            hor_scroll_bar_.visible(sb_visible_.h);
            hor_scroll_bar_.position(-text_.offset().x);
        }
        else
            hor_scroll_bar_.hide();

        if (height > cheight)
        {
            ver_scroll_bar_.range(0, height);
            ver_scroll_bar_.page(cheight);
            ver_scroll_bar_.visible(sb_visible_.v);
            ver_scroll_bar_.position(-text_.offset().y);
        }
        else
            ver_scroll_bar_.hide();

        timestamp_ = GetTickCount();
        thisw->repaint();
    }

    unsigned int timestamp(void) const
    {
        return GetTickCount() - timestamp_;
    }

private:
    Text text_;
    Point offset_;
    bool readonly_;
    std::shared_ptr<Brush> background_brush_;
    ScrollBar ver_scroll_bar_, hor_scroll_bar_;
    struct { bool v, h; } sb_visible_;

    unsigned int timestamp_;
    unsigned int blink_interval_;
};

TextEdit::TextEdit(void)
    : TextEdit("")
{}

TextEdit::TextEdit(const std::string& _text)
    : impl_(new Implement(stralgo::decode(_text, sys::default_codec())))
{
    impl_->ver_scroll_bar_.onScroll([&](ScrollBar& scroll_bar) {
        impl_->text_.offset_top(-scroll_bar.position());
        repaint();
    });
    impl_->hor_scroll_bar_.onScroll([&](ScrollBar& scroll_bar) {
        impl_->text_.offset_left(-scroll_bar.position());
        repaint();
    });

    add(&impl_->ver_scroll_bar_);
    add(&impl_->hor_scroll_bar_);
    onSize(*std::shared_ptr<Size>(new Size(size())));
}

TextEdit::TextEdit(const TextEdit& text_edit)
    : TextEdit()
{
    operator=(text_edit);
}

TextEdit::~TextEdit(void)
{
    delete impl_;
}

TextEdit& TextEdit::operator=(const TextEdit& text_edit)
{
    if (this != &text_edit)
        text(text_edit.text());

    return *this;
}

void TextEdit::selection_brush(const std::shared_ptr<Brush>& brush)
{
    impl_->text_.brush1(brush);
    impl_->update(this);
}

void TextEdit::background_brush(const std::shared_ptr<Brush>& brush)
{
    impl_->background_brush_ = brush;
    impl_->update(this);
}

void TextEdit::foreground_brush(const std::shared_ptr<Brush>& brush)
{
    impl_->text_.brush2(brush);
    impl_->update(this);
}

void TextEdit::multiline(bool multiline)
{
    impl_->text_.multiline(multiline);
    impl_->update(this);
}

void TextEdit::word_wrap(bool word_wrap)
{
    impl_->text_.word_wrap(word_wrap);
    impl_->update(this);
}

void TextEdit::text(const std::string& text)
{
    impl_->text_.text(stralgo::decode(text, sys::default_codec()));
    impl_->update(this);
}

void TextEdit::font(const Font* font)
{
    impl_->text_.font(font);
    impl_->update(this);
}

void TextEdit::readonly(bool readonly)
{
    impl_->readonly_ = readonly;
}

void TextEdit::enable_hor_scroll_bar(bool enable)
{
    impl_->sb_visible_.h = enable;
    impl_->update(this);
}

void TextEdit::enable_ver_scroll_bar(bool enable)
{
    impl_->sb_visible_.v = enable;
    impl_->update(this);
}

void TextEdit::select(unsigned int offset, unsigned int count)
{
    impl_->text_.select(offset, offset + count);
    impl_->update(this);
}

bool TextEdit::multiline(void) const
{
    return impl_->text_.multiline();
}

bool TextEdit::word_wrap(void) const
{
    return impl_->text_.word_wrap();
}

bool TextEdit::readonly(void) const
{
    return impl_->readonly_;
}

std::string TextEdit::text(void) const
{
    return stralgo::encode(impl_->text_.text(), sys::default_codec());
}

std::string TextEdit::selected_text(void) const
{
    return stralgo::encode(impl_->text_.selected_text(), sys::default_codec());
}

void TextEdit::onFocus(bool focus)
{}

void TextEdit::onSize(Size& size)
{
    Point offsetx2 = impl_->offset_ * 2;

    int min_width = 10 + offsetx2.x;
    if (size.width < min_width)
        size.width = min_width;

    int min_height = impl_->text_.font()->emheight() + offsetx2.y;
    if (size.height < min_height)
        size.height = min_height;

    impl_->text_.resize(size.width - offsetx2.x, size.height - offsetx2.y);
    impl_->ver_scroll_bar_.rect(size.width - 3, 0, 3, size.height - 3);
    impl_->hor_scroll_bar_.rect(0, size.height - 3, size.width - 3, 3);
    impl_->update(this);
}

void TextEdit::onPaint(Graphics graphics)
{
    if (impl_->background_brush_)
    {
        Rect paint_rect = graphics.paintRect();
        Point offset = impl_->text_.offset();

        for (int y = paint_rect.top; y < paint_rect.bottom; ++y)
        for (int x = paint_rect.left; x < paint_rect.right; ++x)
            graphics.pixel(x, y, impl_->background_brush_->
                                    operator()(x - offset.x, y - offset.y));
    }

    impl_->text_.draw(graphics, impl_->offset_, graphics.paintRect());

    if (true)
    {
        Point caret = impl_->text_.caret_point()
                    + impl_->text_.offset() + impl_->offset_;

        graphics.drawLine(caret.x, caret.y,
                          caret.x,
                          caret.y + (int)impl_->text_.font()->emheight(),
                          makeLambdaBrushSP([&](int x, int y) {
                              return ~graphics.pixel(x, y);
                          }));
    }
}

bool TextEdit::onKeyDown(VirtualKey vk, KeyState ks)
{
    switch (vk)
    {
    case vkLeft:
        impl_->text_.move(impl_->text_.caret() - 1, ks.shift, ks.ctrl);
        impl_->update(this);
        break;
    case vkRight:
        impl_->text_.move(impl_->text_.caret() + 1, ks.shift, ks.ctrl);
        impl_->update(this);
        break;
    case vkUp:
    case vkDown:
        {
            Point point = impl_->text_.caret_point();

            if (vk == vkUp)
                point.y -= impl_->text_.font()->height();
            else
                point.y += impl_->text_.font()->height();

            impl_->text_.move(impl_->text_.hit_test(point), ks.shift, ks.ctrl);
            impl_->update(this);
        }
        break;
    case vkHome:
    case vkEnd:
        if (ks.ctrl)
            impl_->text_.move(vk == vkHome ? 0 : impl_->text_.text().size(),
                              ks.shift, ks.ctrl);
        else
        {
            unsigned int index = impl_->text_.line_index(impl_->text_.caret());
            std::pair<unsigned int, unsigned int>
                    range = impl_->text_.line_range(index);
            impl_->text_.move(vk == vkHome ? range.first : range.second,
                              ks.shift);
        }
        impl_->update(this);
        break;
    case vkDelete:
        if (!impl_->readonly_)
        {
            impl_->text_.erase(1);
            impl_->update(this);
        }
        break;
    default:
        break;
    }

    return true;
}

bool TextEdit::onKeyPress(unsigned int chr, KeyState ks)
{
    if (ks.ctrl)
    {
        chr += 'a' - 1;

        switch (chr)
        {
        case 'a':
            impl_->text_.select(0, impl_->text_.text().size());
            impl_->update(this);
            break;
        case 'c':
        case 'x':
            if (OpenClipboard(nullptr))
            {
                std::wstring selected_text = impl_->text_.selected_text();
                std::string str = stralgo::encode
                        (selected_text, sys::default_codec());
                EmptyClipboard();
                HANDLE clip = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,
                                          str.size() + 1);
                strncpy((char*)GlobalLock(clip), str.c_str(), str.size());
                GlobalUnlock(clip);
                SetClipboardData(CF_TEXT, clip);
                CloseClipboard();
            }
            if (chr == 'x' && !impl_->readonly_)
            {
                impl_->text_.erase(0);
                impl_->update(this);
            }
            break;
        case 'v':
            if (!impl_->readonly_ && OpenClipboard(nullptr))
            {
                if (IsClipboardFormatAvailable(CF_TEXT))
                {
                    HANDLE clip = GetClipboardData(CF_TEXT);
                    impl_->text_.typewrite(stralgo::decode
                            ((char*)GlobalLock(clip), sys::default_codec()));
                    GlobalUnlock(clip);
                    impl_->update(this);
                }
                CloseClipboard();
            }
            break;
        case 'y':
            if (!impl_->readonly_ && impl_->text_.redo())
                impl_->update(this);
            break;
        case 'z':
            if (!impl_->readonly_ && impl_->text_.undo())
                impl_->update(this);
            break;
        default:
            break;
        }
    }
    else
    if (chr == 8 && !impl_->readonly_) // backspace
    {
        impl_->text_.erase(-1);
        impl_->update(this);
    }
    else
    if (chr == 27) // esc
    {
        impl_->text_.cancel_select();
        impl_->update(this);
    }
    else
    if (!impl_->readonly_)
    {
        if (chr == 13) // enter
            chr = '\n';

        impl_->text_.typewrite(codec::gbk2unicode(chr));
        impl_->update(this);
    }

    return true;
}

bool TextEdit::onMouseDown(MouseButton mb, KeyState ks, Point point)
{
    if (mb == mbLeftButton)
    {
        if (!ks.shift)
            impl_->text_.cancel_select();
        point -= impl_->text_.offset() + impl_->offset_;
        impl_->text_.move(impl_->text_.hit_test(point), ks.shift);
        impl_->update(this);

        set_capture();
    }

    return true;
}

bool TextEdit::onClick(MouseButton mb, KeyState ks, int times, Point point)
{
    if (mb == mbLeftButton)
    {
        point -= impl_->text_.offset() + impl_->offset_;

        switch ((times - 2) % 3)
        {
        case 0:
            impl_->text_.select_similar(impl_->text_.hit_test(point));
            impl_->update(this);
            break;
        case 1:
            {
                unsigned int caret = impl_->text_.hit_test(point);
                std::pair<unsigned int, unsigned int>
                    range = impl_->text_.paragraph_range(caret);
                impl_->text_.select(range.first, range.second);
                impl_->update(this);
            }
            break;
        case 2:
            impl_->text_.select(0, impl_->text_.text().size());
            impl_->update(this);
            break;
        default:
            break;
        }
    }

    return true;
}

bool TextEdit::onMouseMove(KeyState ks, Point point)
{
    if (ks.button_left)
    {
        point -= impl_->text_.offset() + impl_->offset_;
        impl_->text_.move(impl_->text_.hit_test(point), true);
        impl_->update(this);
    }

    return true;
}

bool TextEdit::onMouseUp(MouseButton mb, KeyState ks, Point point)
{
    release_capture();

    return true;
}

bool TextEdit::onMouseWheel(KeyState ks, int delta, Point point)
{
    ScrollBar* scroll_bar = nullptr;
    if ((int)impl_->text_.height() > impl_->text_.canvas().height)
        scroll_bar = &impl_->ver_scroll_bar_;
    else
    if ((int)impl_->text_.width() > impl_->text_.canvas().width)
        scroll_bar = &impl_->hor_scroll_bar_;

    if (scroll_bar)
    {
        int theta = impl_->text_.font()->height() * 3;
        if (delta > 0)
            theta = -theta;
        scroll_bar->position(scroll_bar->position() + theta);
    }

    return true;
}

void TextEdit::onSetCursor(void)
{
    SetCursor(LoadCursor(nullptr, IDC_IBEAM));
}
