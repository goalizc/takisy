#ifndef text_edit_h_20151228
#define text_edit_h_20151228

#include <string>
#include <takisy/gui/basic/define.h>
#include <takisy/gui/widget/widget.h>
#include <takisy/gui/widget/scroll.h>

class text_edit : public widget
{
    class implement;

public:
    text_edit(void);
    text_edit(const std::string& text);
    text_edit(const std::string& text, const std::string& codec);
    text_edit(const std::wstring& text);
   ~text_edit(void);

public:
    std::string  text(const std::string& codec) const;
    std::wstring text(void) const;
    std::string  selected_text(const std::string& codec) const;
    std::wstring selected_text(void) const;
    unsigned int caret(void) const;
    Margin       margin(void) const;
    int          margin_left(void) const;
    int          margin_top(void) const;
    int          margin_right(void) const;
    int          margin_bottom(void) const;
    unsigned int indent(void) const;
    Alignment    alignment(void) const;
    bool         readonly(void) const;
    bool         multiline(void) const;
    bool         word_wrap(void) const;
    unsigned int line_spacing(void) const;
    unsigned int word_spacing(void) const;
    const font&  font(void) const;
    color        border_color(void) const;
    brush_sptr   selection_brush(void) const;
    brush_sptr   background_brush(void) const;
    brush_sptr   foreground_brush(void) const;
    bool         fixed_brush(void) const;
    Point        offset(void) const;
    int          offset_x(void) const;
    int          offset_y(void) const;
    unsigned int max_length(void) const;
    unsigned int blink_interval(void) const;

public:
    vertical_scroll& vertical_scroll(void);
    horizontal_scroll& horizontal_scroll(void);

public:
    void text(const std::string& text);
    void text(const std::string& text, const std::string& codec);
    void text(const std::wstring& text);
    void select(unsigned int offset, unsigned int count);
    void caret(unsigned int caret_position);
    void margin(int margin);
    void margin(const Margin& margin);
    void margin(int left, int top, int right, int bottom);
    void margin_left(int left);
    void margin_top(int top);
    void margin_right(int right);
    void margin_bottom(int bottom);
    void indent(unsigned int indent);
    void alignment(Alignment alignment);
    void readonly(bool readonly);
    void multiline(bool multiline);
    void word_wrap(bool word_wrap);
    void line_spacing(unsigned int line_spacing);
    void word_spacing(unsigned int word_spacing);
    void font(const class font& font);
    void border_color(const color& color);
    void selection_color(const color& color);
    void selection_brush(const brush_sptr& brush);
    void background_color(const color& color);
    void background_brush(const brush_sptr& brush);
    void foreground_color(const color& color);
    void foreground_brush(const brush_sptr& brush);
    void fixed_brush(bool fixed);
    void max_length(unsigned int max_length);
    void blink_interval(unsigned int blink_interval);

public:
    void onSize(void) override;
    void onPaint(graphics graphics, Rect rect) override;
    bool onFocus(bool focus) override;
    bool onSetCursor(void) override;
    bool onKeyDown(sys::VirtualKey vkey) override;
    bool onKeyPress(unsigned int chr) override;
    bool onMouseDown(sys::MouseButton button, int times, Point point) override;
    bool onMouseUp(sys::MouseButton button, Point point) override;
    bool onMouseMove(Point point) override;
    bool onMouseWheel(int delta, Point point) override;

private:
    class implement* impl_;
};

#endif // text_edit_h_20151228
