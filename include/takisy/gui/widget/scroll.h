#ifndef scroll_h_20151229
#define scroll_h_20151229

#include <takisy/core/handler.h>
#include <takisy/gui/widget/widget.h>

class scroll : public widget
{
protected:
    class implement;

    DECLARE_HANDLER(onScroll);

public:
    scroll(void);
    scroll(double min, double max);
    scroll(double min, double max, double value);
    scroll(double min, double max, double value, double step, double page);
   ~scroll(void);

public:
    double min(void) const;
    double max(void) const;
    double value(void) const;
    double step(void) const;
    double page(void) const;
    bool   scrollable(void) const;

public:
    void range(double min, double max);
    void min(double min);
    void max(double max);
    void value(double value);
    void step(double step);
    void page(double page);

public:
    void step_down(void);
    void step_up(void);
    void page_down(void);
    void page_up(void);
    void home(void);
    void end(void);

public:
    bool onSetCursor(void) override;
    bool onClick(sys::MouseButton button, int times, Point point) override;
    bool onMouseUp(sys::MouseButton button, Point point) override;
    bool onMouseEnter(void) override;
    bool onMouseLeave(void) override;
    bool onMouseWheel(int delta, Point point) override;

protected:
    class implement* impl_;
};

class vertical_scroll : public scroll
{
public:
    using scroll::scroll;

public:
    void onPaint(graphics graphics, Rect rect) override;
    bool onMouseDown(sys::MouseButton button, int times, Point point) override;
    bool onMouseMove(Point point) override;
};

class horizontal_scroll : public scroll
{
public:
    using scroll::scroll;

public:
    void onPaint(graphics graphics, Rect rect) override;
    bool onMouseDown(sys::MouseButton button, int times, Point point) override;
    bool onMouseMove(Point point) override;
};

#endif // scroll_h_20151229
