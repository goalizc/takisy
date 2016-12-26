#ifndef button_h_20151231
#define button_h_20151231

#include <string>
#include <takisy/core/handler.h>
#include <takisy/gui/widget/widget.h>
#include <takisy/gui/widget/label.h>

class button : public widget
{
    class implement;

public:
    DECLARE_HANDLER(onClick);

public:
    button(void);
    button(widget* content);
   ~button(void);

private:
    using widget::add;
    using widget::remove;

public:
    widget* content(void) const;
    Size    optimal_size(OptimalPolicy policy=opUnset) const override;

public:
    void content(widget* content);

public:
    void onSize(void) override;
    void onPaint(graphics graphics, Rect rect) override;
    void onEndPaint(graphics graphics, Rect rect) override;
    bool onMouseDown(sys::Button button, int times, Point point) override;
    bool onMouseUp(sys::Button button, Point point) override;
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
    label&       text(void);
    const label& text(void) const;
    Size         optimal_size(OptimalPolicy policy=opUnset) const override;

private:
    class implement* impl_;
};

#endif // button_h_20151231
