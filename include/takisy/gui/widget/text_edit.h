#ifndef text_edit_h_20151228
#define text_edit_h_20151228

#include <string>
#include <takisy/core/handler.h>
#include <takisy/gui/basic/define.h>
#include <takisy/gui/widget/scroll.h>

class text_edit : public scroll_area
{
    class implement;

public:
    DECLARE_HANDLER(onCaretChanged);
    DECLARE_HANDLER(onTypewriting, std::wstring& /* text */);
    DECLARE_HANDLER(onErased, const std::wstring& /* text */);
    DECLARE_HANDLER(onTextChanged);
    DECLARE_HANDLER(onSelectionChanged);
    DECLARE_HANDLER(onCopyAvailable, bool /* yes */);
    DECLARE_HANDLER(onUndoAvailable, bool /* yes */);
    DECLARE_HANDLER(onRedoAvailable, bool /* yes */);

public:
    text_edit(void);
    text_edit(const std::string& text);
    text_edit(const std::string& text, const std::string& codec);
    text_edit(const std::wstring& text);
   ~text_edit(void);

public:
    const std::wstring& text(void) const;
    std::wstring        selected_text(void) const;
    unsigned int        caret(void) const;
    Margin              margin(void) const;
    int                 margin_left(void) const;
    int                 margin_top(void) const;
    int                 margin_right(void) const;
    int                 margin_bottom(void) const;
    unsigned int        indent(void) const;
    unsigned int        alignment(void) const;
    bool                readonly(void) const;
    bool                multiline(void) const;
    bool                word_wrap(void) const;
    unsigned int        line_spacing(void) const;
    unsigned int        word_spacing(void) const;
    const class font*   font(void) const;
    bool                border_visible(void) const;
    bool                fixed_brush(void) const;
    brush_sptr          text_brush(void) const;
    brush_sptr          background_brush(void) const;
    brush_sptr          selection_brush(void) const;
    Point               offset(void) const;
    int                 offset_x(void) const;
    int                 offset_y(void) const;
    unsigned int        max_length(void) const;
    unsigned int        blink_interval(void) const;

public:
    void text(const std::string& text);
    void text(const std::string& text, const std::string& codec);
    void text(const std::wstring& text);
    void select(unsigned int offset, unsigned int count);
    void caret(unsigned int caret);
    void margin(int margin);
    void margin(const Margin& margin);
    void margin(int left, int top, int right, int bottom);
    void margin_left(int left);
    void margin_top(int top);
    void margin_right(int right);
    void margin_bottom(int bottom);
    void indent(unsigned int indent);
    void alignment(unsigned int alignment);
    void readonly(bool readonly);
    void multiline(bool multiline);
    void word_wrap(bool word_wrap);
    void line_spacing(unsigned int line_spacing);
    void word_spacing(unsigned int word_spacing);
    void font(const class font* font);
    void border_visible(bool visible);
    void show_border(void);
    void hide_border(void);
    void text_color(const color& color);
    void text_brush(const brush_sptr& brush);
    void background_color(const color& color);
    void background_brush(const brush_sptr& brush);
    void selection_brush(const brush_sptr& brush);
    void selection_color(const color& color);
    void fixed_brush(bool fixed);
    void max_length(unsigned int max_length);
    void blink_interval(unsigned int blink_interval);

public:
    void undo(void);
    void redo(void);
    unsigned int hittest(Point point) const;

public:
    void onSize(void) override;
    void onPaint(graphics graphics, Rect rect) override;
    bool onFocus(bool focus) override;
    bool onSetCursor(Point point) override;
    bool onKeyDown(sys::VirtualKey vkey) override;
    bool onKeyPress(unsigned int chr) override;
    bool onMouseDown(sys::Button button, int times, Point point) override;
    bool onMouseUp(sys::Button button, Point point) override;
    bool onMouseMove(Point point) override;

protected:
    class implement* impl_;
};

#endif // text_edit_h_20151228
