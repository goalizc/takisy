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
   ~button(void);

private:
    using widget::add;
    using widget::remove;

public:
    void onPaint(graphics graphics, Rect rect) override;
    void onEndPaint(graphics graphics, Rect rect) override;
    bool onMouseDown(sys::Button button, int times, Point point) override;
    bool onMouseUp(sys::Button button, int times, Point point) override;
    bool onMouseEnter(void) override;
    bool onMouseLeave(void) override;

private:
    class implement* impl_;
};

class widget_button : public button
{
    class implement;

public:
    widget_button(void);
    widget_button(class widget* widget);
   ~widget_button(void);

public:
    class widget* widget(void);
    const class widget* widget(void) const;
    Size optimal(OptimalPolicy policy=opUnset) const override;

public:
    void widget(class widget* widget);

public:
    void onSize(void) override;

private:
    class implement* impl_;
};

class text_button : public widget_button
{
public:
    text_button(void);
    text_button(const std::string& caption);
    text_button(const std::string& caption, const std::string& codec);
    text_button(const std::wstring& caption);

private:
    using widget_button::widget;

public:
    label& text(void);
    const label& text(void) const;
    Size optimal(OptimalPolicy policy=opUnset) const override;

private:
    label label_;
};

#endif // button_h_20151231
