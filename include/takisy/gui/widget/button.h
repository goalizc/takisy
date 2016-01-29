#ifndef button_h_20151231
#define button_h_20151231

#include <takisy/core/handler.h>
#include <takisy/gui/widget/widget.h>

class button : public widget
{
    class implement;

    ENABLE_HANDLER(onClick);

public:
    button(void);
   ~button(void);

public:
    color color(void) const;

public:
    void color(const class color& color);

public:
    void onPaint(graphics graphics, Rect rect) override;
    bool onMouseDown(sys::MouseButton button, int times, Point point) override;
    bool onMouseUp(sys::MouseButton button, Point point) override;
    bool onMouseEnter(void) override;
    bool onMouseLeave(void) override;

private:
    class implement* impl_;
};

class text_button : public button
{
    class implement;

public:
    text_button(void);
    text_button(const char* caption);
    text_button(const char* caption, const char* codec);
    text_button(const wchar_t* caption);
   ~text_button(void);

public:
    const char*    caption(const char* codec) const;
    const wchar_t* caption(void) const;
    unsigned int   margin(void) const;
    const font&    font(void) const;
    brush_sptr     foreground_brush(void) const;
    widget::Size   optimal(void) const;

public:
    void caption(const char* caption);
    void caption(const char* caption, const char* codec);
    void caption(const wchar_t* caption);
    void margin(unsigned int margin);
    void font(const class font& font);
    void foreground_color(const class color& foreground_color);

public:
    void onSize(Size size) override;

private:
    class implement* impl_;
};

#endif // button_h_20151231
