#include <map>
#include <takisy/core/sys.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/basic/cursor.h>
#include <takisy/gui/basic/graphics.h>
#include <takisy/gui/widget/widget.h>
#include <takisy/gui/cross_platform_window.h>
#include "basic/impl/graphics.hpp"

typedef widget* LPWIDGET;

namespace takisy
{
    const char*
        class_name__ = "takisy::gui::cross_platform_window";

    LPWIDGET
        captured_widget__;

    std::map<cross_platform_window::Handle, LPWIDGET>
        all_windows__;
}

class cross_platform_window::implement
{
    friend class cross_platform_window;

    static class cross_platform_window_initializer
    {
    public:
        cross_platform_window_initializer(void)
        {
        #if defined(__WINNT__) || defined(__CYGWIN__)
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
                .lpszClassName = takisy::class_name__,
                .hIconSm       = LoadIcon(nullptr, IDI_APPLICATION),
            };

            RegisterClassEx(&cls);
        #endif
        }

        ~cross_platform_window_initializer(void)
        {
        #if defined(__WINNT__) || defined(__CYGWIN__)
            UnregisterClass(takisy::class_name__, GetModuleHandle(nullptr));
        #endif
        }
    } __cross_platform_window_initializer__;

public:
    implement(Handle handle)
        : handle_(handle)
    {}

private:
    static void onWidgetPaint(LPWIDGET, graphics&, const Rect&);
#if defined(__WINNT__) || defined(__CYGWIN__)
    static LRESULT CALLBACK widgetProc(HWND, LPWIDGET, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);
#else
    // other system callback
#endif

    static Point pointFromLPARAM(LPARAM lparam)
    {
        return Point {
            .x = static_cast<short>(LOWORD(lparam)),
            .y = static_cast<short>(HIWORD(lparam))
        };
    }

    static Size sizeFromLPARAM(LPARAM lparam)
    {
        return Size(LOWORD(lparam), HIWORD(lparam));
    }

    static Point cursor_point(void)
    {
    #if defined(__WINNT__) || defined(__CYGWIN__)
        POINT point;

        if (GetCursorPos(&point))
            return point;
        else
            return Point();
    #else
        return Point();
    #endif
    }

    static LPWIDGET hittest_nocapture(LPWIDGET widget, Point& point)
    {
        if (!widget || !widget->inside(point.x, point.y))
            return nullptr;
        else
            point -= widget->xy();

        typedef std::vector<LPWIDGET> children_type;
        typedef children_type::reverse_iterator iterator_type;
        children_type children = widget->children();

        for (iterator_type i = children.rbegin(); i != children.rend(); ++i)
        {
            LPWIDGET child = *i;
            if (!child->visible())
                continue;

            LPWIDGET ht_widget = hittest_nocapture(child, point);
            if (ht_widget)
                return ht_widget;
        }

        return widget;
    }

    static LPWIDGET hittest(LPWIDGET widget, Point& point)
    {
        if (takisy::captured_widget__)
        {
            LPWIDGET bar = takisy::captured_widget__->father();
            while (bar)
            {
                point -= bar->xy();
                bar    = bar->father();
            }

            if (!takisy::captured_widget__->inside(point.x, point.y))
            {
                point -= takisy::captured_widget__->xy();
                return takisy::captured_widget__;
            }
            else
                widget = takisy::captured_widget__;
        }

        return hittest_nocapture(widget, point);
    }

private:
    Handle handle_;
};

cross_platform_window::implement::cross_platform_window_initializer
        cross_platform_window::implement::__cross_platform_window_initializer__;

void cross_platform_window::implement::onWidgetPaint
    (LPWIDGET widget, graphics& graphics, const Rect& rect)
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
        onWidgetPaint(child, child_graphics, paint_rect);
    }

    widget->onEndPaint(graphics, rect);
}

#if defined(__WINNT__) || defined(__CYGWIN__)

class windows_device_context : public device_context
{
public:
    windows_device_context(HDC hdc)
        : hdc_(hdc)
    {}

public:
    void refresh(const rect& rect, const canvas_bgra8& canvas) override
    {
        canvas_bgra8 c(canvas);

        for (canvas_bgra8::pixel_format& pixel : c.pixels())
            pixel.premultiply();

        BITMAPINFO bitmap_info = {
            .bmiHeader = BITMAPINFOHEADER {
                .biSize          = sizeof(BITMAPINFOHEADER),
                .biWidth         = +static_cast<LONG>(c.width()),
                .biHeight        = -static_cast<LONG>(c.height()),
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
                          0, 0, 0, c.height(),
                          c.pixels().data(), &bitmap_info, DIB_RGB_COLORS);
    }

private:
    HDC hdc_;
};

LRESULT CALLBACK cross_platform_window::implement::widgetProc
    (HWND hwnd, LPWIDGET widget, UINT msg, WPARAM wparam, LPARAM lparam)
{
    struct window_information
    {
        std::pair<bool, Point> move;
        LPWIDGET focus, enter, cursor;
        struct Click {
            unsigned int times;
            LPWIDGET widget;
            Point point;
            sys::MouseButton button;
            DWORD pretime;
        } click;

    public:
        window_information(void)
            : move(std::make_pair(false, Point(0, 0)))
            , focus(nullptr), enter(nullptr), cursor(nullptr)
            , click({0, nullptr, Point(0, 0), sys::mbNone, 0})
        {}
    };

    static std::map<HWND, window_information> wndinfos;
    window_information& wndinfo = wndinfos[hwnd];
    Point ht_point;

    #define onEvent(lpwidget, method, params...)                               \
        ({  LPWIDGET w = lpwidget; Point p = ht_point;                         \
            while (w) {                                                        \
                if (w->method(params)) break;                                  \
                else if (w->attribute<bool>("intercept."#method)) w = nullptr; \
                else { ht_point += w->xy(); w = w->father(); }                 \
            } ht_point = p; w; })

    #define onEventStop(stopwidget, lpwidget, method, params...)               \
        ({  LPWIDGET w = lpwidget; Point p = ht_point;                         \
            while (w) {                                                        \
                if (w == stopwidget || w->method(params)) break;               \
                else if (w->attribute<bool>("intercept."#method)) w = nullptr; \
                else { ht_point += w->xy(); w = w->father(); }                 \
            } ht_point = p; w; })

    switch (msg)
    {
    case WM_PAINT:
        if (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_LAYERED)
        {
            Rect cr = widget->client_rect();

            HDC     hdc = GetDC(hwnd);
            HDC     cdc = CreateCompatibleDC(hdc);
            HBITMAP bmp = CreateCompatibleBitmap(hdc, cr.width(), cr.height());
            windows_device_context wdc(cdc);
            SelectObject(cdc, bmp);

            graphics graphics;
            graphics.impl_->initialize(&wdc, cr);
            onWidgetPaint(widget, graphics, cr);
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
            windows_device_context wdc(ps.hdc);

            graphics graphics;
            graphics.impl_->initialize(&wdc, ps.rcPaint);
            graphics.clear(color::white());
            onWidgetPaint(widget, graphics, ps.rcPaint);
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
            sys::MouseButton button = sys::mbNone;

            switch (msg)
            {
            case WM_LBUTTONDOWN: button = sys::mbLButton; break;
            case WM_MBUTTONDOWN: button = sys::mbMButton; break;
            case WM_RBUTTONDOWN: button = sys::mbRButton; break;
            }

            if (button == sys::mbLButton)
            {
                LPWIDGET fw = onEventStop(wndinfo.focus, ht_widget, onFocus, 1);
                if (fw && wndinfo.focus != fw)
                {
                    if (wndinfo.focus)
                        wndinfo.focus->onFocus(false);
                    wndinfo.focus = fw;
                }
            }

            window_information::Click& ci = wndinfo.click;
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
                && button == sys::mbLButton)
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

            if (!onEventStop(wndinfo.cursor, ht_widget, onSetCursor))
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
            sys::MouseButton button = sys::mbNone;

            ReleaseCapture();
            wndinfo.move.first = false;

            switch (msg)
            {
            case WM_LBUTTONUP: button = sys::mbLButton; break;
            case WM_MBUTTONUP: button = sys::mbMButton; break;
            case WM_RBUTTONUP: button = sys::mbRButton; break;
            }

            if (button == wndinfo.click.button)
                onEvent(ht_widget, onClick,
                        button, wndinfo.click.times, ht_point);
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
        onEvent(wndinfo.focus, onKeyDown, (sys::VirtualKey)(wparam));
        break;
    case WM_CHAR:
        onEvent(wndinfo.focus, onKeyPress, wparam);
        break;
    case WM_KEYUP:
        onEvent(wndinfo.focus, onKeyUp, (sys::VirtualKey)(wparam));
        break;
    default:
        break;
    }

    #undef onEventStop
    #undef onEvent

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK cross_platform_window::implement::windowProc
    (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    static std::map<HWND, unsigned char> prechar;
    LPWIDGET client = nullptr;

    if (takisy::all_windows__.find(hwnd) != takisy::all_windows__.end())
        client = takisy::all_windows__[hwnd];

    switch (msg)
    {
    case WM_SIZING:
        if (client)
        {
            enum { None = 0, L = 0x1000, T = 0x100, R = 0x10, B = 0x1 };
            static unsigned int border[] =
                 { None, L, R, T, T | L, T | R, B, B | L, B | R };

            RECT& rect = *reinterpret_cast<RECT*>(lparam);
            cross_platform_window window(hwnd);
            Size min = client->minimal_size(), ws = window.size();
            Size max = client->maximal_size(), cs = window.client_size();
            Size rsz = Rect(rect).size();

            min.width  += ws.width  - cs.width;
            min.height += ws.height - cs.height;
            max.width  += ws.width  - cs.width;
            max.height += ws.height - cs.height;

            if (rsz.width < min.width) {
                if (border[wparam] & L) rect.left = rect.right - min.width;
                else
                if (border[wparam] & R) rect.right = rect.left + min.width;
            } else if (rsz.width > max.width) {
                if (border[wparam] & L) rect.left = rect.right - max.width;
                else
                if (border[wparam] & R) rect.right = rect.left + max.width;
            }

            if (rsz.height < min.height) {
                if (border[wparam] & T) rect.top = rect.bottom - min.height;
                else
                if (border[wparam] & B) rect.bottom = rect.top + min.height;
            } else if (rsz.height > max.height) {
                if (border[wparam] & T) rect.top = rect.bottom - max.height;
                else
                if (border[wparam] & B) rect.bottom = rect.top + max.height;
            }
        }
        break;
    case WM_MOVE:
        if (client)
            client->xy(pointFromLPARAM(lparam));
        break;
    case WM_SIZE:
        if (client)
            client->size(sizeFromLPARAM(lparam));
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
        if (prechar[hwnd] > 0x7f) {
            wparam |= prechar[hwnd] << 8;
            prechar[hwnd] = 0; }
        else if (wparam > 0x7f) {
            prechar[hwnd] = wparam;
            return 0; }
        break;
    case WM_NCDESTROY:
        takisy::all_windows__.erase(hwnd);
        if (takisy::all_windows__.empty()) quit();
        return 0;
    case WM_ERASEBKGND:
        if (client)
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

cross_platform_window::cross_platform_window(bool create_window)
    : cross_platform_window(create_window ? create() : nullptr)
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
    return CreateWindowEx(0, takisy::class_name__,
                          "Default Native Window",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          GetDesktopWindow(), nullptr,
                          GetModuleHandle(nullptr), nullptr);
}

cross_platform_window::Handle cross_platform_window::handle(void) const
{
    return impl_->handle_;
}

std::wstring cross_platform_window::caption(void) const
{
    if (!impl_->handle_)
        return std::wstring();

#if defined(__WINNT__) || defined(__CYGWIN__)
    static wchar_t caption[1024];
    int length = GetWindowTextW(impl_->handle_, caption, 1024);

    return std::wstring(caption, length);
#else
    return std::wstring();
#endif
}

std::string cross_platform_window::caption(const std::string& codec) const
{
    return stralgo::encode(caption(), codec);
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
        return Rect();

#if defined(__WINNT__) || defined(__CYGWIN__)
    RECT rect;

    if (GetWindowRect(impl_->handle_, &rect))
        return rect;
    else
        return Rect();
#else
    return Rect();
#endif
}

Point cross_platform_window::client_offset(void) const
{
    if (!impl_->handle_)
        return Point();

#if defined(__WINNT__) || defined(__CYGWIN__)
    POINT offset = {0, 0};

    if (ClientToScreen(impl_->handle_, &offset))
        return Point(offset) - xy();
    else
        return Point();
#else
    return Point();
#endif
}

Size cross_platform_window::client_size(void) const
{
    if (!impl_->handle_)
        return Size();

#if defined(__WINNT__) || defined(__CYGWIN__)
    RECT rect;

    if (GetClientRect(impl_->handle_, &rect))
        return Rect(rect).size();
    else
        return Size();
#else
    return Size();
#endif
}

bool cross_platform_window::visible(void) const
{
    if (!impl_->handle_)
        return false;

#if defined(__WINNT__) || defined(__CYGWIN__)
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

#if defined(__WINNT__) || defined(__CYGWIN__)
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
#if defined(__WINNT__) || defined(__CYGWIN__)
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

#if defined(__WINNT__) || defined(__CYGWIN__)
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

void cross_platform_window::repaint(void)
{
    if (!impl_->handle_)
        return;

#if defined(__WINNT__) || defined(__CYGWIN__)
    InvalidateRect(impl_->handle_, nullptr, TRUE);
#endif
}

void cross_platform_window::repaint(const Rect& rect)
{
    if (rect.empty() || !impl_->handle_)
        return;

#if defined(__WINNT__) || defined(__CYGWIN__)
    RECT r;

    r.left   = rect.left;
    r.top    = rect.top;
    r.right  = rect.right;
    r.bottom = rect.bottom;

    InvalidateRect(impl_->handle_, &r, TRUE);
#endif
}

void cross_platform_window::capture(bool capture)
{
    if (!impl_->handle_)
        return;

#if defined(__WINNT__) || defined(__CYGWIN__)
    if (capture)
        SetCapture(impl_->handle_);
    else
        ReleaseCapture();
#endif
}

void cross_platform_window::close(void)
{
    if (!impl_->handle_)
        return;

#if defined(__WINNT__) || defined(__CYGWIN__)
    DestroyWindow(impl_->handle_);
#endif
}

long cross_platform_window::exec(void)
{
#if defined(__WINNT__) || defined(__CYGWIN__)
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
#if defined(__WINNT__) || defined(__CYGWIN__)
    PostQuitMessage(quitcode);
#endif
}
