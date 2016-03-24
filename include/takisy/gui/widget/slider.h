#ifndef slider_h_20160314
#define slider_h_20160314

#include <takisy/core/handler.h>
#include <takisy/gui/widget/widget.h>

class slider : public widget
{
protected:
    class implement;

    DECLARE_HANDLER(onSlide);

public:
    slider(void);
    slider(double min, double max);
    slider(double min, double max, double value);
   ~slider(void);

public:
    double min(void) const;
    double max(void) const;
    double value(void) const;

public:
    void range(double min, double max);
    void min(double min);
    void max(double max);
    void value(double value);

public:
    void home(void);
    void end(void);

public:
    bool onMouseUp(sys::MouseButton button, Point point) override;

protected:
    class implement* impl_;
};

class vertical_slider : public slider
{
public:
    using slider::slider;

public:
    void onPaint(graphics graphics, Rect rect) override;
    bool onMouseDown(sys::MouseButton button, int times, Point point) override;
    bool onMouseMove(Point point) override;
};

class horizontal_slider : public slider
{
public:
    using slider::slider;

public:
    void onPaint(graphics graphics, Rect rect) override;
    bool onMouseDown(sys::MouseButton button, int times, Point point) override;
    bool onMouseMove(Point point) override;
};

#endif // slider_h_20160314
