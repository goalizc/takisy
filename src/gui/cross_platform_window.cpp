#include <map>
#include <set>
#include <takisy/core/sys.h>
#include <takisy/core/macro.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/basic/cursor.h>
#include <takisy/gui/basic/graphics.h>
#include <takisy/gui/widget/widget.h>
#include <takisy/gui/cross_platform_window.h>
#include "basic/impl/color_scheme.hpp"
#include "basic/impl/graphics.hpp"

typedef widget* LPWIDGET;

namespace takisy {
namespace gui
{
    LPWIDGET
        captured_widget__;
    std::map<cross_platform_window::Handle, LPWIDGET>
        windows_as__, focus__;
    std::set<cross_platform_window::Handle>
        windows__, windows_widget__;
}}

class cross_platform_window::implement
{
    friend class cross_platform_window;

    static constexpr
           const char* class_name__ = "takisy::gui::cross_platform_window";

    static class initializer
    {
    public:
        initializer(void)
        {
        #ifdef __os_win__
            WNDCLASSEX cls = {
                .cbSize        = sizeof(WNDCLASSEX),
                .style         = CS_HREDRAW | CS_VREDRAW,
                .lpfnWndProc   = windowProc,
                .cbClsExtra    = 0,
                .cbWndExtra    = 0,
                .hInstance     = GetModuleHandle(nullptr),
                .hIcon         = LoadIcon(nullptr, IDI_APPLICATION),
                .hCursor       = nullptr,
                .hbrBackground = HBRUSH(COLOR_WINDOW),
                .lpszMenuName  = nullptr,
                .lpszClassName = class_name__,
                .hIconSm       = LoadIcon(nullptr, IDI_APPLICATION),
            };

            RegisterClassEx(&cls);
        #endif
        }

        ~initializer(void)
        {
        #ifdef __os_win__
            UnregisterClass(class_name__, GetModuleHandle(nullptr));
        #endif
        }
    } __initializer__;

public:
    implement(Handle handle)
        : handle_(handle)
    {}

private:
#ifdef __os_win__
    static LRESULT CALLBACK widgetProc(HWND, LPWIDGET, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);
#else
    // other system callback
#endif

    static Point cursor_point(void)
    {
    #ifdef __os_win__
        POINT point;

        if (GetCursorPos(&point))
            return point;
        else
            return Point();
    #else
        return Point();
    #endif
    }

    static void paint(LPWIDGET widget, graphics& graphics, const Rect& rect)
    {
        widget->onPaint(graphics, rect);

        for (LPWIDGET child : widget->children())
        {
            if (!child->visible())
                continue;

            Rect paint_rect = rect.intersect(child->rect());
            if (paint_rect.empty())
                continue;

            paint_rect = paint_rect.offset(-child->xy());
            class graphics child_graphics = graphics;
            child_graphics.impl_->paint_area(child->xy(), paint_rect);
            paint(child, child_graphics, paint_rect);
        }

        widget->onEndPaint(graphics, rect);
    }

    static LPWIDGET hittest_nocapture(LPWIDGET widget, Point& point)
    {
        if (!widget || widget->disabled() || !widget->inside(point.x, point.y))
            return nullptr;
        else
            point -= widget->xy();

        typedef std::vector<LPWIDGET> children_type;
        typedef children_type::reverse_iterator iterator_type;
        children_type children = widget->children();

        for (iterator_type i = children.rbegin(); i != children.rend(); ++i)
        {
            if (!(*i)->visible())
                continue;

            LPWIDGET ht_widget = hittest_nocapture(*i, point);
            if (ht_widget)
                return ht_widget;
        }

        return widget;
    }

    static LPWIDGET hittest(LPWIDGET widget, Point& point)
    {
        if (takisy::gui::captured_widget__)
        {
            widget = takisy::gui::captured_widget__;

            while (widget)
            {
                point -= widget->xy();
                widget = widget->father();
            }

            return takisy::gui::captured_widget__;
        }

        return hittest_nocapture(widget, point);
    }

private:
    Handle handle_;
};

cross_platform_window::implement::initializer
        cross_platform_window::implement::__initializer__;

#ifdef __os_win__

class windows_device_context : public device_context
{
public:
    windows_device_context(HDC hdc)
        : hdc_(hdc)
    {}

public:
    void refresh(const rect& rect, canvas_bgra8& canvas) override
    {
        for (canvas_bgra8::pixel_format& pixel : canvas.pixels())
            pixel.premultiply();

        BITMAPINFO bitmap_info = {
            .bmiHeader = BITMAPINFOHEADER {
                .biSize          = sizeof(BITMAPINFOHEADER),
                .biWidth         = +static_cast<LONG>(canvas.width()),
                .biHeight        = -static_cast<LONG>(canvas.height()),
                .biPlanes        = 1,
                .biBitCount      = 32,
                .biCompression   = BI_RGB,
                .biSizeImage     = 0,
                .biXPelsPerMeter = 0,
                .biYPelsPerMeter = 0,
                .biClrUsed       = 0,
                .biClrImportant  = 0, },
            .bmiColors = {0},
        };

        SetDIBitsToDevice(hdc_,
                          rect.left, rect.top, rect.width(), rect.height(),
                          0, 0, 0, canvas.height(),
                          canvas.pixels().data(), &bitmap_info, DIB_RGB_COLORS);
    }

private:
    HDC hdc_;
};

LRESULT CALLBACK cross_platform_window::implement::widgetProc
    (HWND hwnd, LPWIDGET widget, UINT msg, WPARAM wparam, LPARAM lparam)
{
    using namespace takisy::gui;

    struct window_information
    {
        std::pair<bool, Point> move;
        LPWIDGET enter;
        struct click {
            unsigned int times;
            LPWIDGET widget;
            Point point;
            sys::Button button;
            DWORD pretime;
        } click;

    public:
        window_information(void)
            : move(std::make_pair(false, Point(0, 0))), enter(nullptr)
            , click({0, nullptr, Point(0, 0), sys::btnNone, 0})
        {}
    };

    static std::map<HWND, window_information> wndinfos;
    window_information& wndinfo = wndinfos[hwnd];
    Point ht_point;

    #define onEventStop(stopwidget, lpwidget, method, params...)        \
        ({  LPWIDGET w = lpwidget; Point p = ht_point;                  \
            while (w && w != stopwidget) {                              \
                if (w->method(params)) break;                           \
                else if (w->property<bool>("intercept."#method)) break; \
                else { ht_point += w->xy(); w = w->father(); }          \
            } ht_point = p; w; })
    #define onEvent(lpwidget, method, params...)                        \
            onEventStop(nullptr, lpwidget, method, params)

    switch (msg)
    {
    case WM_PAINT:
        if (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_LAYERED)
        {
            Rect cr = widget->client_rect();

            HDC     hdc = GetDC(hwnd);
            HDC     cdc = CreateCompatibleDC(hdc);
            HBITMAP bmp = CreateCompatibleBitmap(hdc, cr.width(), cr.height());
            device_context::pointer wdc(new windows_device_context(cdc));
            SelectObject(cdc, bmp);

            graphics graphics;
            graphics.impl_->initialize(wdc, cr);
            paint(widget, graphics, cr);
            graphics.impl_->paint();

            SIZE          size  = { cr.width(), cr.height() };
            POINT         point = { 0, 0 };
            BLENDFUNCTION bf    = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
            UpdateLayeredWindow(hwnd, hdc, nullptr, &size,
                                      cdc, &point, 0, &bf, ULW_ALPHA);

            DeleteObject(bmp);
            DeleteObject(cdc);
            ReleaseDC(hwnd, hdc);
        }
        else
        {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            device_context::pointer wdc(new windows_device_context(ps.hdc));
            static constexpr const char* bgc = "background";

            graphics graphics;
            graphics.impl_->initialize(wdc, ps.rcPaint);
            graphics.clear(widget->color_scheme().impl_->other(bgc, 255));
            paint(widget, graphics, ps.rcPaint);
            graphics.impl_->paint();

            EndPaint(hwnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        {
            Point    ht_point  = cursor_point();
            LPWIDGET ht_widget = hittest(widget, ht_point);
            sys::Button button = sys::btnNone;

            switch (msg)
            {
            case WM_LBUTTONDOWN: button = sys::btnLeft;   break;
            case WM_MBUTTONDOWN: button = sys::btnMiddle; break;
            case WM_RBUTTONDOWN: button = sys::btnRight;  break;
            }

            if (button == sys::btnLeft)
            {
                LPWIDGET prefw = focus__[hwnd]; focus__[hwnd] = nullptr;
                LPWIDGET    fw = onEventStop(prefw, ht_widget, onFocus, true);

                if (fw && prefw != fw) {
                    focus__[hwnd] = fw;
                    if (prefw) prefw->onFocus(false);
                } else
                    focus__[hwnd] = prefw;
            }

            struct window_information::click& ci = wndinfo.click;
            ci.times   = ci.widget == ht_widget
                      && ci.button == button
                      && ci.point  == ht_point
                      && ci.pretime > GetTickCount() - GetDoubleClickTime()
                             ? ci.times + 1 : 1;
            ci.widget  = ht_widget;
            ci.point   = ht_point;
            ci.button  = button;
            ci.pretime = GetTickCount();

            if (!onEvent(ht_widget, onMouseDown, button, ci.times, ht_point)
                && button == sys::btnLeft
                && !IsZoomed(hwnd))
            {
                wndinfo.move = std::make_pair(true, cursor_point());
                SetCapture(hwnd);
            }
        }
        break;
    case WM_MOUSEMOVE:
        if (wndinfo.move.first)
        {
            ReleaseCapture();
            Point offset = cursor_point() - wndinfo.move.second;
            widget->xy(widget->xy() + offset);
            SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
            wndinfo.move.first = false;
        }
        else
        {
            Point    ht_point  = cursor_point();
            LPWIDGET ht_widget = hittest(widget, ht_point);

            if (!onEvent(ht_widget, onSetCursor))
                cursor::set(cursor::ctArrow);

            LPWIDGET ew = onEventStop(wndinfo.enter, ht_widget, onMouseEnter);
            if (wndinfo.enter != ew)
            {
                if (wndinfo.enter)
                    wndinfo.enter->onMouseLeave();
                wndinfo.enter = ew;
            }

            onEvent(ht_widget, onMouseMove, ht_point);
        }
        break;
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
        {
            Point    ht_point  = cursor_point();
            LPWIDGET ht_widget = hittest(widget, ht_point);
            sys::Button button = sys::btnNone;

            ReleaseCapture();
            wndinfo.move.first = false;

            switch (msg)
            {
            case WM_LBUTTONUP: button = sys::btnLeft;   break;
            case WM_MBUTTONUP: button = sys::btnMiddle; break;
            case WM_RBUTTONUP: button = sys::btnRight;  break;
            }

            onEvent(ht_widget, onMouseUp, button, ht_point);
        }
        break;
    case WM_MOUSEWHEEL:
        {
            Point    ht_point  = cursor_point();
            LPWIDGET ht_widget = hittest(widget, ht_point);
            int      delta     = GET_WHEEL_DELTA_WPARAM(wparam);

            onEvent(ht_widget, onMouseWheel, delta, ht_point);
        }
        break;
    case WM_KEYDOWN:
        if (focus__[hwnd] && focus__[hwnd]->enabled())
            onEvent(focus__[hwnd], onKeyDown, (sys::VirtualKey)(wparam));
        break;
    case WM_CHAR:
        if (focus__[hwnd] && focus__[hwnd]->enabled())
            onEvent(focus__[hwnd], onKeyPress, wparam);
        break;
    case WM_KEYUP:
        if (focus__[hwnd] && focus__[hwnd]->enabled())
            onEvent(focus__[hwnd], onKeyUp, (sys::VirtualKey)(wparam));
        break;
    default:
        break;
    }

    #undef onEvent
    #undef onEventStop

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK cross_platform_window::implement::windowProc
    (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    using namespace takisy::gui;
    static std::map<HWND, unsigned char> prechar;
    LPWIDGET client = nullptr;

    try
        { client = windows_as__.at(hwnd); }
    catch (const std::out_of_range&)
        {}

    switch (msg)
    {
    case WM_ERASEBKGND:
        if (client)
            return 0;
    case WM_SIZING:
        if (client)
        {
            enum { None = 0, L = 0x1000, T = 0x100, R = 0x10, B = 0x1 };
            static unsigned int border[] =
                 { None, L, R, T, T | L, T | R, B, B | L, B | R };
            cross_platform_window window(hwnd);
            Size  min  = client->lower_size(), ws = window.size();
            Size  max  = client->upper_size(), cs = window.client_size();
            RECT& rect = *reinterpret_cast<RECT*>(lparam);
            Size  size = Rect(rect).size();

            min.width  += ws.width  - cs.width;
            min.height += ws.height - cs.height;
            max.width  += ws.width  - cs.width;
            max.height += ws.height - cs.height;

            if (size.width < min.width) {
                if (border[wparam] & L)
                    rect.left = rect.right - min.width;
                else
                if (border[wparam] & R)
                    rect.right = rect.left + min.width;
            } else
            if (size.width > max.width) {
                if (border[wparam] & L)
                    rect.left = rect.right - max.width;
                else
                if (border[wparam] & R)
                    rect.right = rect.left + max.width;
            }

            if (size.height < min.height) {
                if (border[wparam] & T)
                    rect.top = rect.bottom - min.height;
                else
                if (border[wparam] & B)
                    rect.bottom = rect.top + min.height;
            } else
            if (size.height > max.height) {
                if (border[wparam] & T)
                    rect.top = rect.bottom - max.height;
                else
                if (border[wparam] & B)
                    rect.bottom = rect.top + max.height;
            }
        }
        break;
    case WM_MOVE:
        if (client)
            client->xy(Point {
                .x = static_cast<short>(LOWORD(lparam)),
                .y = static_cast<short>(HIWORD(lparam))
            });
        break;
    case WM_SIZE:
        if (client)
            client->size(Size {
                .width  = LOWORD(lparam),
                .height = HIWORD(lparam)
            });
        break;
    case WM_SHOWWINDOW:
        if (client)
            client->visible(wparam);
        break;
    case WM_MOUSEMOVE:
        if (!client)
            cursor::set(cursor::ctArrow);
        break;
    case WM_CHAR:
        if (prechar[hwnd] > 0x7f)
        {
            wparam |= prechar[hwnd] << 8;
            prechar[hwnd] = 0;
        }
        else if (wparam > 0x7f)
        {
            prechar[hwnd] = wparam;
            return 0;
        }
        break;
    case WM_NCDESTROY:
        windows__.erase(hwnd);
        windows_as__.erase(hwnd);
        windows_widget__.erase(hwnd);
        if (windows__.empty())
            quit();
        return 0;
    default:
        break;
    };

    if (client)
        return widgetProc(hwnd, client, msg, wparam, lparam);
    else
        return DefWindowProc(hwnd, msg, wparam, lparam);
}

#else
    // other system callback
#endif

cross_platform_window::cross_platform_window(void)
    : cross_platform_window(nullptr)
{}

cross_platform_window::cross_platform_window(unsigned long wndstyle)
    : cross_platform_window(create(wndstyle))
{}

cross_platform_window::cross_platform_window(Handle handle)
    : impl_(new implement(handle))
{}

cross_platform_window::cross_platform_window(const cross_platform_window& cpw)
    : cross_platform_window(cpw.impl_->handle_)
{}

cross_platform_window::~cross_platform_window(void)
{
    delete impl_;
}

cross_platform_window&
    cross_platform_window::operator=(const cross_platform_window& cpw)
{
    if (this != &cpw)
        impl_->handle_ = cpw.impl_->handle_;

    return *this;
}

cross_platform_window::Handle cross_platform_window::create(void)
{
    return create(wsCommonWindow);
}

cross_platform_window::Handle
    cross_platform_window::create(unsigned long wndstyle)
{
#ifdef __os_win__
    DWORD exstyle = 0, style = 0;
    if (wndstyle & wsAlphaChannel)  exstyle |= WS_EX_LAYERED;
    if (wndstyle & wsTaskbar); else exstyle |= WS_EX_TOOLWINDOW;
    if (wndstyle & wsCaption)         style |= WS_CAPTION;
    if (wndstyle & wsSysMenu)         style |= WS_SYSMENU;
    if (wndstyle & wsMinBox)          style |= WS_MINIMIZEBOX;
    if (wndstyle & wsMinimize)        style |= WS_MINIMIZE;
    if (wndstyle & wsMaxBox)          style |= WS_MAXIMIZEBOX;
    if (wndstyle & wsMaximize)        style |= WS_MAXIMIZE;
    if (wndstyle & wsResizable)       style |= WS_SIZEBOX;
    if (wndstyle & wsPopup)           style |= WS_POPUP;
    if (wndstyle & wsVisible)         style |= WS_VISIBLE;

    Handle handle = CreateWindowEx(
                    exstyle, implement::class_name__, "Cross Platform Window",
                    style, CW_USEDEFAULT, CW_USEDEFAULT,
                           CW_USEDEFAULT, CW_USEDEFAULT,
                    nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
    if (handle)
        takisy::gui::windows__.insert(handle);

    return handle;
#else
    return nullptr;
#endif
}

cross_platform_window::Handle cross_platform_window::handle(void) const
{
    return impl_->handle_;
}

std::wstring cross_platform_window::caption(void) const
{
    if (!impl_->handle_)
        return std::wstring();

#ifdef __os_win__
    static wchar_t caption[1024];
    int length = GetWindowTextW(impl_->handle_, caption, 1024);

    return std::wstring(caption, length);
#else
    return std::wstring();
#endif
}

int cross_platform_window::x(void) const
{
    return rect().left;
}

int cross_platform_window::y(void) const
{
    return rect().top;
}

Point cross_platform_window::xy(void) const
{
    return rect().left_top();
}

unsigned int cross_platform_window::width(void) const
{
    return rect().width();
}

unsigned int cross_platform_window::height(void) const
{
    return rect().height();
}

Size cross_platform_window::size(void) const
{
    return rect().size();
}

Rect cross_platform_window::rect(void) const
{
    if (!impl_->handle_)
        return {0, 0, 0, 0};

#ifdef __os_win__
    RECT rect;

    if (GetWindowRect(impl_->handle_, &rect))
        return rect;
    else
        return {0, 0, 0, 0};
#else
    return {0, 0, 0, 0};
#endif
}

Point cross_platform_window::client_offset(void) const
{
    if (!impl_->handle_)
        return {0, 0};

#ifdef __os_win__
    POINT offset = {0, 0};

    if (ClientToScreen(impl_->handle_, &offset))
        return Point(offset) - xy();
    else
        return {0, 0};
#else
    return {0, 0};
#endif
}

Size cross_platform_window::client_size(void) const
{
    if (!impl_->handle_)
        return {0, 0};

#ifdef __os_win__
    RECT rect;

    if (GetClientRect(impl_->handle_, &rect))
        return Rect(rect).size();
    else
        return {0, 0};
#else
    return {0, 0};
#endif
}

bool cross_platform_window::visible(void) const
{
    if (!impl_->handle_)
        return false;

#ifdef __os_win__
    return IsWindowVisible(impl_->handle_);
#else
    return false;
#endif
}

void cross_platform_window::handle(Handle handle)
{
    impl_->handle_ = handle;
}

void cross_platform_window::caption(const std::string& _caption)
{
    caption(_caption, sys::default_codec());
}

void cross_platform_window::caption(const std::string& _caption,
                                    const std::string& codec)
{
    caption(stralgo::decode(_caption, codec));
}

void cross_platform_window::caption(const std::wstring& caption)
{
    if (!impl_->handle_)
        return;

#ifdef __os_win__
    SetWindowTextW(impl_->handle_, caption.c_str());
#endif
}

void cross_platform_window::x(int x)
{
    Rect rct = rect();
    rect(rct.move(x, rct.top));
}

void cross_platform_window::y(int y)
{
    Rect rct = rect();
    rect(rct.move(rct.left, y));
}

void cross_platform_window::xy(int x, int y)
{
    rect(rect().move(x, y));
}

void cross_platform_window::xy(const Point& _xy)
{
    xy(_xy.x, _xy.y);
}

void cross_platform_window::width(unsigned int width)
{
    Rect rct = rect();

    rct.width(width);

    rect(rct);
}

void cross_platform_window::height(unsigned int height)
{
    Rect rct = rect();

    rct.height(height);

    rect(rct);
}

void cross_platform_window::size(unsigned int width, unsigned int height)
{
    Rect rct = rect();

    rct.width(width);
    rct.height(height);

    rect(rct);
}

void cross_platform_window::size(const Size& _size)
{
    size(_size.width, _size.height);
}

void cross_platform_window::rect(int x, int y,
                                 unsigned int width, unsigned int height)
{
#ifdef __os_win__
    SetWindowPos(handle(), nullptr, x, y, width, height, SWP_NOZORDER);
#endif
}

void cross_platform_window::rect(const Point& xy, const Size& size)
{
    rect(xy.x, xy.y, xy.x + size.width, xy.y + size.height);
}

void cross_platform_window::rect(const Rect& _rect)
{
    rect(_rect.left, _rect.top, _rect.width(), _rect.height());
}

void cross_platform_window::client_width(unsigned int width)
{
    Size size = client_size();

    size.width = width;

    client_size(size);
}

void cross_platform_window::client_height(unsigned int height)
{
    Size size = client_size();

    size.height = height;

    client_size(size);
}

void cross_platform_window::client_size(unsigned int width, unsigned int height)
{
    client_size(Size(width, height));
}

void cross_platform_window::client_size(const Size& clientsize)
{
    size(clientsize + size() - client_size());
}

void cross_platform_window::visible(bool visible)
{
    if (!impl_->handle_)
        return;

#ifdef __os_win__
    ShowWindow(impl_->handle_, visible ? SW_SHOW : SW_HIDE);
#endif
}

void cross_platform_window::show(void)
{
    visible(true);
}

void cross_platform_window::hide(void)
{
    visible(false);
}

void cross_platform_window::move_center(void)
{
    Size screen(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
    Size window = size();

    xy((screen.width - window.width) / 2, (screen.height - window.height) / 2);
}

void cross_platform_window::capture(bool capture)
{
    if (!impl_->handle_)
        return;

#ifdef __os_win__
    if (capture)
        SetCapture(impl_->handle_);
    else
        ReleaseCapture();
#endif
}

void cross_platform_window::topmost(bool topmost)
{
    if (!impl_->handle_)
        return;

#ifdef __os_win__
    if (topmost)
        SetWindowPos(impl_->handle_, HWND_TOPMOST,
                     0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    else
        SetWindowPos(impl_->handle_, HWND_NOTOPMOST,
                     0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#endif
}

void cross_platform_window::repaint(void)
{
    if (!impl_->handle_)
        return;

#ifdef __os_win__
    InvalidateRect(impl_->handle_, nullptr, TRUE);
#endif
}

void cross_platform_window::repaint(const Rect& rect)
{
    if (rect.empty() || !impl_->handle_)
        return;

#ifdef __os_win__
    RECT r;

    r.left   = rect.left;
    r.top    = rect.top;
    r.right  = rect.right;
    r.bottom = rect.bottom;

    InvalidateRect(impl_->handle_, &r, TRUE);
#endif
}

void cross_platform_window::close(void)
{
    if (!impl_->handle_)
        return;

#ifdef __os_win__
    DestroyWindow(impl_->handle_);
#endif
}

long cross_platform_window::exec(void)
{
#ifdef __os_win__
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
#else
    return 255;
#endif
}

void cross_platform_window::quit(void)
{
    quit(0);
}

void cross_platform_window::quit(long quitcode)
{
#ifdef __os_win__
    PostQuitMessage(quitcode);
#endif
}
