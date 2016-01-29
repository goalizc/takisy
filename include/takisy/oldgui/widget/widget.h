#ifndef widget_h_20130712
#define widget_h_20130712

#include <vector>
#include <takisy/oldgui/basic/typedef.h>
#include <takisy/oldgui/basic/define.h>
#include <takisy/oldgui/basic/graphics.h>

class Widget
{
    class Implement;

public:
    Widget(void);
    Widget(int x, int y, unsigned int width, unsigned int height,
           Widget* parent = nullptr, bool visible = true);
    virtual ~Widget(void);

    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;

public:
    void create(int x, int y, unsigned int width, unsigned int height,
                Widget* parent = nullptr, bool visible = true);

public:
    void    add(Widget* widget);
    bool    exists(Widget* widget) const;
    void    remove(Widget* widget);
    Widget* parent(void) const;
    void    parent(Widget* parent);
    Widget* forefather(void) const;
    std::vector<Widget*> children(void) const;
    Widget* hit_test(int x, int y);
    Widget* hit_test(const Point& point);

public:
    void        attribute(const char* attribute, const char* value);
    const char* attribute(const char* attribute) const;

public:
    unsigned int id(void) const;

    Rect         rect(void) const;
    Point        xy(void) const;
    int          x(void) const;
    int          y(void) const;
    Rect         global_rect(void) const;
    Point        global_xy(void) const;
    int          global_x(void) const;
    int          global_y(void) const;
    Size         size(void) const;
    unsigned int width(void) const;
    unsigned int height(void) const;
    bool         visible(void) const;

public:
    void x(int x);
    void y(int y);
    void xy(int x, int y);
    void xy(const Point& xy);
    void width(unsigned int width);
    void height(unsigned int height);
    void size(unsigned int width, unsigned int height);
    void size(const Size& size);
    void rect(int x, int y, unsigned int width, unsigned int height);
    void rect(const Point& xy, const Size& size);
    void rect(const Rect& rect);
    void visible(bool visble);
    void show(void);
    void hide(void);

public:
    void repaint(void);
    void repaint(int x, int y, unsigned int width, unsigned int height);
    void repaint(Rect rect);

    void set_capture(void);
    void release_capture(void);

public:
    virtual bool inside(int x, int y);

public:
    virtual void onCreate(void);
    virtual void onDestroy(void);
    virtual void onFocus(bool focus);
    virtual void onShown(void);
    virtual void onHidden(void);
    virtual void onMove(Point& point);
    virtual void onSize(Size& size);
    virtual void onPaint(Graphics graphics);
    virtual bool onKeyDown(VirtualKey vk, KeyState ks);
    virtual bool onKeyPress(unsigned int ch, KeyState ks);
    virtual bool onKeyUp(VirtualKey vk, KeyState ks);
    virtual bool onMouseDown(MouseButton mb, KeyState ks, Point point);
    virtual bool onClick(MouseButton mb, KeyState ks, int times, Point point);
    virtual bool onMouseUp(MouseButton mb, KeyState ks, Point point);
    virtual bool onMouseMove(KeyState ks, Point point);
    virtual bool onMouseEnter(KeyState ks, Point point);
    virtual bool onMouseHover(KeyState ks, Point point);
    virtual bool onMouseLeave(KeyState ks, Point point);
    virtual bool onMouseWheel(KeyState ks, int delta, Point point);
    virtual void onSetCursor(void);

private:
    class Implement* impl_;
};

#endif //widget_h_20130712
