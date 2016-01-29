#ifndef text_edit_h_20150115
#define text_edit_h_20150115

#include <string>
#include <memory>
#include <takisy/oldgui/basic/brush.h>
#include <takisy/oldgui/widget/widget.h>

class TextEdit : public Widget
{
    class Implement;

public:
    TextEdit(void);
    TextEdit(const std::string& text);
    TextEdit(const TextEdit& text_edit);
   ~TextEdit(void);

    TextEdit& operator=(const TextEdit& text_edit);

public:
    void selection_brush(const std::shared_ptr<Brush>& brush);
    void background_brush(const std::shared_ptr<Brush>& brush);
    void foreground_brush(const std::shared_ptr<Brush>& brush);
    void multiline(bool multiline);
    void word_wrap(bool word_wrap);
    void text(const std::string& text);
    void font(const Font* font);
    void readonly(bool readonly);
    void enable_hor_scroll_bar(bool enable);
    void enable_ver_scroll_bar(bool enable);

public:
    void select(unsigned int offset, unsigned int count);

public:
    bool multiline(void) const;
    bool word_wrap(void) const;
    bool readonly(void) const;
    std::string text(void) const;
    std::string selected_text(void) const;

public:
    void onFocus(bool focus) override;
    void onSize(Size& size) override;
    void onPaint(Graphics graphics) override;
    bool onKeyDown(VirtualKey vk, KeyState ks);
    bool onKeyPress(unsigned int ch, KeyState ks) override;
    bool onMouseDown(MouseButton mb, KeyState ks, Point point) override;
    bool onClick(MouseButton mb, KeyState ks, int times, Point point) override;
    bool onMouseUp(MouseButton mb, KeyState ks, Point point) override;
    bool onMouseMove(KeyState ks, Point point) override;
    bool onMouseWheel(KeyState ks, int delta, Point point) override;
    void onSetCursor(void) override;

private:
    class Implement* impl_;
};

#endif // text_edit_h_20150115
