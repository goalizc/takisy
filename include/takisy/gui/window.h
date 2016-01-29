#ifndef window_h_20160113
#define window_h_20160113

#include <takisy/gui/widget/widget.h>

class window
{
    class implement;

public:
    enum Layout
    {
        lNone, lHorizontal, lVertical
    };

    typedef widget::Point Point;
    typedef widget::Size  Size;
    typedef widget::Rect  Rect;

public:
    window(void);
    explicit
    window(const char* caption);
    window(const char* caption, unsigned int width, unsigned int height);
    window(const char* caption,
           int x, int y, unsigned int width, unsigned int height);
    window(const char* caption, const Size& size);
    window(const char* caption, const Point& xy, const Size& size);
    window(const char* caption,
           const Point& left_top, const Point& right_bottom);
    window(const char* caption, const Rect& rect);
    explicit
    window(Layout layout);
    window(Layout layout, const char* caption);
    window(Layout layout, const char* caption,
           unsigned int width, unsigned int height);
    window(Layout layout, const char* caption,
           int x, int y, unsigned int width, unsigned int height);
    window(Layout layout, const char* caption, const Size& size);
    window(Layout layout, const char* caption,
           const Point& xy, const Size& size);
    window(Layout layout, const char* caption,
           const Point& left_top, const Point& right_bottom);
    window(Layout layout, const char* caption, const Rect& rect);
   ~window(void);

private:
    window(const window&);
    void operator=(const window&);

public:
    const char*  caption(void) const;
    int          x(void) const;
    int          y(void) const;
    Point        xy(void) const;
    unsigned int width(void) const;
    unsigned int height(void) const;
    Size         size(void) const;
    Rect         rect(void) const;
    bool         visible(void) const;

public:
    unsigned int minimal_width(void) const;
    unsigned int minimal_height(void) const;
    Size         minimal_size(void) const;
    unsigned int maximal_width(void) const;
    unsigned int maximal_height(void) const;
    Size         maximal_size(void) const;

public:
    bool add(widget* widget);
    void remove(widget* widget);

    void caption(const char* caption);
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
    void show_normal(void);
    void show_minimal(void);
    void show_maximal(void);
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

private:
    class implement* impl_;
};

#endif // window_h_20160113
