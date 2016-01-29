#ifndef widget_h_20150715
#define widget_h_20150715

#include <string>
#include <vector>
#include <takisy/core/sys.h>
#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/basic/size.h>
#include <takisy/cgl/basic/rect.h>
#include <takisy/gui/basic/graphics.h>

class widget
{
    class implement;

public:
    typedef point Point;
    typedef sizeu Size;
    typedef rect  Rect;

public:
    widget(void);
    widget(widget* father);
    virtual ~widget(void);

private:
    widget(const widget&);
    void operator=(const widget&);

public:
    unsigned int id(void) const;
    widget*      father(void) const;
    widget*      forefather(void);
    const widget* forefather(void) const;
    std::vector<widget*> children(void) const;
    template <typename ReturnType>
    ReturnType   attribute(const std::string& name) const;

    int          x(void) const;
    int          y(void) const;
    Point        xy(void) const;
    unsigned int width(void) const;
    unsigned int height(void) const;
    Size         size(void) const;
    Rect         rect(void) const;
    bool         visible(void) const;
    Rect         client_rect(void) const;
    int          window_x(void) const;
    int          window_y(void) const;
    Point        window_xy(void) const;
    Rect         window_rect(void) const;
    int          screen_x(void) const;
    int          screen_y(void) const;
    Point        screen_xy(void) const;
    Rect         screen_rect(void) const;

public:
    unsigned int minimal_width(void) const;
    unsigned int minimal_height(void) const;
    Size         minimal_size(void) const;
    unsigned int maximal_width(void) const;
    unsigned int maximal_height(void) const;
    Size         maximal_size(void) const;

public:
    bool pure(void);

public:
    bool father(widget* father);
    bool add(widget* widget);
    void remove(widget* widget);
    template <typename ValueType>
    void attribute(const std::string& name, const ValueType& value);

    void x(int x);
    void y(int y);
    void xy(int x, int y);
    void xy(Point xy);
    void width(unsigned int width);
    void height(unsigned int height);
    void size(unsigned int width, unsigned int height);
    void size(Size size);
    void rect(int x, int y, unsigned int width, unsigned int height);
    void rect(const Point& xy, const Size& size);
    void rect(const Rect& rect);
    void visible(bool visible);
    void show(void);
    void hide(void);

public:
    void minimal_width(unsigned int minimal_width);
    void minimal_height(unsigned int minimal_height);
    void minimal_size(unsigned int minimal_width, unsigned int minimal_height);
    void minimal_size(const Size& minimal_size);
    void maximal_width(unsigned int maximal_width);
    void maximal_height(unsigned int maximal_height);
    void maximal_size(unsigned int maximal_width, unsigned int maximal_height);
    void maximal_size(const Size& maximal_size);
    void absolute_width(unsigned int width);
    void absolute_height(unsigned int height);
    void absolute_size(unsigned int width, unsigned int height);
    void absolute_size(const Size& size);

public:
    void pure(bool pure);
    void sendcmd(const char* cmdid);
    void repaint(void);
    void repaint(const Rect& rect);
    void capture(bool capture);

public:
    bool exists_attribute(const std::string& name) const;
    bool is_child(widget* widget) const;
    bool is_senior(widget* widget) const;
    bool is_junior(widget* widget) const;
    virtual bool inside(int x, int y) const;

public:
    bool as_window(void);
    bool as_window(bool enable_alpha_channel);
    bool as_topmost_window(void);
    bool as_topmost_window(bool enable_alpha_channel);

public:
    virtual void onShown(void);
    virtual void onHidden(void);
    virtual void onMove(Point point);
    virtual void onSize(Size size);
    virtual void onPaint(graphics graphics, Rect rect);
    virtual void onEndPaint(graphics graphics, Rect rect);
    virtual bool onFocus(bool focus);
    virtual bool onSetCursor(void);
    virtual bool onKeyDown(sys::VirtualKey vkey);
    virtual bool onKeyPress(unsigned int chr);
    virtual bool onKeyUp(sys::VirtualKey vkey);
    virtual bool onMouseDown(sys::MouseButton button, int times, Point point);
    virtual bool onClick(sys::MouseButton button, int times, Point point);
    virtual bool onCommand(widget* widget, const char* cmdid);
    virtual bool onMouseUp(sys::MouseButton button, Point point);
    virtual bool onMouseMove(Point point);
    virtual bool onMouseEnter(void);
    virtual bool onMouseLeave(void);
    virtual bool onMouseWheel(int delta, Point point);

private:
    void* attribute(const std::string& name) const;
    void  attribute(const std::string& name,
                    const void* value, unsigned int length);

public:
    class implement* impl_;
};

template <typename ReturnType>
ReturnType widget::attribute(const std::string& name) const
{
    void* value = attribute(name);

    if (value)
        return *reinterpret_cast<ReturnType*>(value);

    return ReturnType();
}

template <>
char* widget::attribute<char*>(const std::string& name) const;
template <>
const char* widget::attribute<const char*>(const std::string& name) const;
template <>
std::string widget::attribute<std::string>(const std::string& name) const;

template <typename ValueType>
inline void widget::attribute(const std::string& name, const ValueType& value)
{
    attribute(name, &value, sizeof(value));
}

template <>
void widget::attribute<char*>(const std::string& name, char* const& value);
template <>
void widget::attribute<const char*>(
        const std::string& name, const char* const& value);
template <>
void widget::attribute<std::string>(
        const std::string& name, const std::string& value);

#endif // widget_h_20150715
