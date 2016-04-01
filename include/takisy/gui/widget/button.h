#ifndef button_h_20151231
#define button_h_20151231

#include <string>
#include <takisy/core/handler.h>
#include <takisy/gui/widget/widget.h>

class button : public widget
{
    class implement;

    DECLARE_HANDLER(onClick);

public:
    button(void);
   ~button(void);

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
    text_button(const std::string& caption);
    text_button(const std::string& caption, const std::string& codec);
    text_button(const std::wstring& caption);
   ~text_button(void);

public:
    std::wstring caption(void) const;
    unsigned int margin(void) const;
    const font&  font(void) const;
    brush_sptr   foreground_brush(void) const;
    Size         optimal_size(void) const override;

public:
    void caption(const std::string& caption);
    void caption(const std::string& caption, const std::string& codec);
    void caption(const std::wstring& caption);
    void margin(unsigned int margin);
    void font(const class font& font);
    void foreground_color(const class color& foreground_color);

public:
    void onSize(void) override;

private:
    class implement* impl_;
};

#endif // button_h_20151231
