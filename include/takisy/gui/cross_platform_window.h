#ifndef cross_platform_window_h_20160318
#define cross_platform_window_h_20160318

#include <string>
#include <takisy/core/macro.h>
#include <takisy/gui/basic/define.h>

#ifdef OS_WIN
#include <Windows.h>
#define __SytemHandleType__ HWND
#else
#define __SytemHandleType__ void*
#endif

class cross_platform_window
{
    class implement;

public:
    typedef __SytemHandleType__ Handle;

public:
    cross_platform_window(void);
    cross_platform_window(bool create_window);
    cross_platform_window(Handle handle);
    cross_platform_window(const cross_platform_window& cpw);
   ~cross_platform_window(void);

    cross_platform_window& operator=(const cross_platform_window& cpw);

public:
    static Handle create(void);

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

    void repaint(void);
    void repaint(const Rect& rect);
    void capture(bool capture);
    void close(void);

public:
    static long exec(void);
    static void quit(void);
    static void quit(long quitcode);

private:
    class implement* impl_;
};

#endif // cross_platform_window_h_20160318
