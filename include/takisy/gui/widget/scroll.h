#ifndef scroll_h_20151229
#define scroll_h_20151229

#include <takisy/core/handler.h>
#include <takisy/gui/widget/widget.h>

class scroll : public widget
{
protected:
    class implement;

public:
    DECLARE_HANDLER(onScroll);

public:
    scroll(void);
    scroll(double min, double max);
    scroll(double min, double max, double value);
   ~scroll(void);

public:
    double    min(void) const;
    double    max(void) const;
    double    value(void) const;
    long long valued(void) const;
    double    step(void) const;
    double    page(void) const;
    bool      scrollable(void) const;

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
    bool onMouseUp(sys::Button button, Point point) override;
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
    bool onMouseDown(sys::Button button, int times, Point point) override;
    bool onMouseMove(Point point) override;
};

class horizontal_scroll : public scroll
{
public:
    using scroll::scroll;

public:
    void onPaint(graphics graphics, Rect rect) override;
    bool onMouseDown(sys::Button button, int times, Point point) override;
    bool onMouseMove(Point point) override;
};

class scroll_area : public widget
{
    class implement;

public:
    scroll_area(void);
   ~scroll_area(void);

public:
          class vertical_scroll&   vertical_scroll(void);
    const class vertical_scroll&   vertical_scroll(void) const;
          class horizontal_scroll& horizontal_scroll(void);
    const class horizontal_scroll& horizontal_scroll(void) const;

public:
    void onSize(void) override;
    bool onSetCursor(void) override;
    bool onMouseDown(sys::Button button, int times, Point point) override;
    bool onMouseUp(sys::Button button, Point point) override;
    bool onMouseMove(Point point) override;
    bool onMouseWheel(int delta, Point point) override;

private:
    class implement* impl_;
};

#endif // scroll_h_20151229
