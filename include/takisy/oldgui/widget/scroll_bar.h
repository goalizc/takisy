#ifndef scroll_bar_h_20150119
#define scroll_bar_h_20150119

#include <takisy/oldgui/basic/handler.h>
#include <takisy/oldgui/widget/widget.h>

class ScrollBar : public Widget
{
    class Implement;

public:
    ScrollBar(void);
    ScrollBar(const ScrollBar& scroll_bar);
   ~ScrollBar(void);

    ScrollBar& operator=(const ScrollBar& scroll_bar);

public:
    void range(double min, double max);
    void min(double min);
    void max(double max);
    void step(double step);
    void page(double page);
    void position(double position);
    void vertical(bool vertical);
    void color(Color color);

public:
    double min(void) const;
    double max(void) const;
    double step(void) const;
    double page(void) const;
    double position(void) const;
    bool   vertical(void) const;
    Color  color(void) const;

public:
    template <typename Lambda>
    void onScroll(Lambda lambda);
    void onScroll(const std::shared_ptr<Handler>& handler);

public:
    void onPaint(Graphics graphics) override;
    bool onKeyDown(VirtualKey vk, KeyState ks) override;
    bool onMouseDown(MouseButton mb, KeyState ks, Point point) override;
    bool onMouseMove(KeyState ks, Point point) override;
    bool onMouseUp(MouseButton mb, KeyState ks, Point point) override;

private:
    class Implement* impl_;
};

template <typename Lambda>
inline void ScrollBar::onScroll(Lambda lambda)
{
    onScroll(get_lambda_handler<Lambda, ScrollBar>(lambda));
}

#endif // scroll_bar_h_20150119
