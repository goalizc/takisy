#ifndef cross_platform_window_h_20160318
#define cross_platform_window_h_20160318

#include <string>
#include <takisy/core/osdet.h>
#include <takisy/gui/basic/define.h>

#ifdef __os_win__
#include <Windows.h>
#define __SystemWindowHandle__ HWND
#else
#define __SystemWindowHandle__ void*
#endif

class cross_platform_window
{
    class implement;

public:
    enum WindowStyle
    {
        // basic window style
        wsAlphaChannel = 0x0001,
        wsTaskbar      = 0x0002,
        wsCaption      = 0x0004,
        wsSysMenu      = 0x0008,
        wsMinBox       = 0x0010,
        wsMinimize     = 0x0020,
        wsMaxBox       = 0x0040,
        wsMaximize     = 0x0080,
        wsResizable    = 0x0100,
        wsPopup        = 0x0200,
        wsVisible      = 0x0400,

        // extention window style
        wsWidgetWindow = wsAlphaChannel | wsPopup,
        wsCommonWindow = wsTaskbar | wsCaption
                       | wsSysMenu | wsMinBox | wsMaxBox | wsResizable,
    };

public:
    typedef __SystemWindowHandle__ Handle;

public:
    cross_platform_window(void);
    cross_platform_window(unsigned long wndstyle);
    cross_platform_window(Handle handle);
    cross_platform_window(const cross_platform_window& cpw);
   ~cross_platform_window(void);

    cross_platform_window& operator=(const cross_platform_window& cpw);

public:
    static Handle create(void);
    static Handle create(unsigned long wndstyle);

public:
    Handle        handle(void) const;
    std::wstring  caption(void) const;
    int           x(void) const;
    int           y(void) const;
    Point         xy(void) const;
    unsigned int  width(void) const;
    unsigned int  height(void) const;
    Size          size(void) const;
    Rect          rect(void) const;
    Point         client_offset(void) const;
    Size          client_size(void) const;
    bool          visible(void) const;

public:
    void handle(Handle handle);
    void caption(const std::string& caption);
    void caption(const std::string& caption, const std::string& codec);
    void caption(const std::wstring& caption);
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
    void client_width(unsigned int width);
    void client_height(unsigned int height);
    void client_size(unsigned int width, unsigned int height);
    void client_size(const Size& size);
    void visible(bool visible);
    void show(void);
    void hide(void);

public:
    void move_center(void);

public:
    void capture(bool capture);
    void topmost(bool topmost);
    void repaint(void);
    void repaint(const Rect& rect);
    void close(void);

public:
    static long exec(void);
    static void quit(void);
    static void quit(long quitcode);

private:
    class implement* impl_;
};

#endif // cross_platform_window_h_20160318
