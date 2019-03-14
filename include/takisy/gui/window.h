#ifndef window_h_20160318
#define window_h_20160318

#include <string>
#include <takisy/core/sys.h>
#include <takisy/core/handler.h>
#include <takisy/gui/basic/define.h>
#include <Windows.h>

class window
{
    class implement;

public:
    enum WindowStyle
    {
        // basic window style
        wsAlphaChannel = 0x00000001,
        wsTaskbar      = 0x00000002,
        wsCaption      = 0x00000004,
        wsSysMenu      = 0x00000008,
        wsMinBox       = 0x00000010,
        wsMinimize     = 0x00000020,
        wsMaxBox       = 0x00000040,
        wsMaximize     = 0x00000080,
        wsResizable    = 0x00000100,
        wsPopup        = 0x00000200,
        wsVisible      = 0x00000400,
        wsNoActivate   = 0x00000800,
        wsEvanescent   = 0x00001000,
        wsTopmost      = 0x00002000,
        wsDropShadow   = 0x00004000,

        // extention window style
        wsWidgetWindow = wsAlphaChannel | wsPopup,
        wsCommonWindow = wsTaskbar | wsCaption
                       | wsSysMenu | wsMinBox | wsMaxBox | wsResizable,
    };

public:
    struct event
    {
        DECLARE_HANDLER(onActivate, int /* active */);
        DECLARE_HANDLER(onMove);
        DECLARE_HANDLER(onClose, bool& /* agree */);
        DECLARE_HANDLER(onDestroy);
    };

    typedef HWND Handle;

public:
    window(void);
    window(unsigned long wndstyle);
    window(Handle handle);
    window(const window& wnd);
   ~window(void);

    window& operator=(const window& wnd);

public:
    static Handle create(void);
    static Handle create(unsigned long wndstyle);
    static long   exec(void);
    static void   quit(void);
    static void   quit(long quitcode);
    static bool   key_pressed(sys::VirtualKey vkey);
    static bool   key_pressed(const std::vector<sys::VirtualKey>& vkeys);

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
    struct event& event(void) const;

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
    void show_normal(void);
    void show_minimize(void);
    void show_maximize(void);
    void hide(void);

public:
    void move_center(void);
    void capture(bool capture);
    void repaint(void);
    void repaint(const Rect& rect);
    void close(void);

private:
    class implement* impl_;
};

#endif // window_h_20160318
