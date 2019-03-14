#include <map>
#include <set>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/basic/cursor.h>
#include <takisy/cgl/graphics.h>
#include <takisy/gui/widget/widget.h>
#include <takisy/gui/window.h>
#include "../cgl/impl/graphics.hpp"
#include "basic/impl/color_scheme.hpp"

#ifndef CS_DROPSHADOW
#define CS_DROPSHADOW 0x00020000
#endif // CS_DROPSHADOW

#ifndef ULW_ALPHA
#define ULW_ALPHA 0x2
#include <takisy/core/dynamic_linking_loader.h>
typedef BOOL WINAPI (*funcUpdateLayeredWindow)(
    HWND hwnd,
    HDC hdcDst,
    POINT *pptDst,
    SIZE *psize,
    HDC hdcSrc,
    POINT *pptSrc,
    COLORREF crKey,
    BLENDFUNCTION *pblend,
    DWORD dwFlags
);
static funcUpdateLayeredWindow UpdateLayeredWindow =
      (funcUpdateLayeredWindow)dll::dlsymbol("user32", "UpdateLayeredWindow");
#endif // ULW_ALPHA

typedef widget* LPWIDGET;

namespace takisy {
namespace gui
{
    enum OtherMessageID
    {
        omidUserMessage = WM_USER,
    };

    struct args { widget* widget; void* userdata; };

    UINT
        user_msgid__ = omidUserMessage;
    LPWIDGET
        capture_widget__;
    window::Handle
        activate_window__;
    std::map<window::Handle, LPWIDGET>
        windows_as__, focus__;
    std::map<window::Handle, struct window::event>
        windows_event__;
    std::map<window::Handle, unsigned long>
        windows_style__;
    std::set<window::Handle>
        windows__, windows_widget__, windows_evanescent__;

    void erase_handle(window::Handle handle)
    {
        focus__.erase(handle);
        windows__.erase(handle);
        windows_as__.erase(handle);
        windows_evanescent__.erase(handle);
        windows_event__.erase(handle);
        windows_style__.erase(handle);
        windows_widget__.erase(handle);
    }
}}

class window::implement
{
    friend class window;

    static class initializer
    {
        std::vector<std::string> wndclasses;

        bool register_class(const std::string& wndclass, unsigned int style)
        {
            WNDCLASSEX cls = {
                .cbSize        = sizeof(WNDCLASSEX),
                .style         = style,
                .lpfnWndProc   = windowProc,
                .cbClsExtra    = 0,
                .cbWndExtra    = 0,
                .hInstance     = GetModuleHandle(nullptr),
                .hIcon         = LoadIcon(nullptr, IDI_APPLICATION),
                .hCursor       = nullptr,
                .hbrBackground = HBRUSH(COLOR_WINDOW),
                .lpszMenuName  = nullptr,
                .lpszClassName = wndclass.c_str(),
                .hIconSm       = LoadIcon(nullptr, IDI_APPLICATION),
            };

            if (!RegisterClassEx(&cls))
                return false;
            else
                wndclasses.push_back(wndclass);

            return true;
        }

        void unregister_class(const std::string& wndclass)
        {
            UnregisterClass(wndclass.c_str(), GetModuleHandle(nullptr));
        }

    public:
        initializer(void)
        {
            register_class("takisy::gui::window.drop_shadow",
                           CS_HREDRAW | CS_VREDRAW | CS_DROPSHADOW);
            register_class("takisy::gui::window",
                           CS_HREDRAW | CS_VREDRAW);
        }

        ~initializer(void)
        {
            for (const std::string& wndclass : wndclasses)
                unregister_class(wndclass);
        }
    } __wnd_initializer__;

public:
    implement(Handle handle)
        : handle_(handle)
    {}

private:
    static LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK widgetProc(HWND, LPWIDGET, UINT, WPARAM, LPARAM);

    static Point cursor_point(void)
    {
        POINT point;

        if (GetCursorPos(&point))
            return point;
        else
            return Point();
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

        for (LPWIDGET child : widget->children())
        {
            if (!child->visible())
                continue;

            LPWIDGET ht_widget = hittest_nocapture(child, point);
            if (ht_widget)
                return ht_widget;
        }

        return widget;
    }

    static  LPWIDGET hittest(
            LPWIDGET widget, Point& point,
            LPWIDGET prior_widget = takisy::gui::capture_widget__,
            bool prior_children = true)
    {
        if (prior_widget) {
            widget = prior_widget->father();
            while (widget) {
                point -= widget->xy();
                widget = widget->father();
            }

            widget = prior_widget;
            if (!prior_children || !widget->inside(point.x, point.y)) {
                point -= widget->xy();
                return widget;
            }
        }

        return hittest_nocapture(widget, point);
    }

private:
    Handle handle_;
};

window::implement::initializer window::implement::__wnd_initializer__;

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
            .bmiColors = {{0}},
        };

        SetDIBitsToDevice(hdc_,
                          rect.left, rect.top, rect.width(), rect.height(),
                          0, 0, 0, canvas.height(),
                          canvas.pixels().data(), &bitmap_info, DIB_RGB_COLORS);
    }

private:
    HDC hdc_;
};

LRESULT CALLBACK window::implement::windowProc
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
            static constexpr const unsigned int border[] =
                 { None, L, R, T, T | L, T | R, B, B | L, B | R };
            window wnd(hwnd);
            Size  min  = client->lower_size(), ws = wnd.size();
            Size  max  = client->upper_size(), cs = wnd.client_size();
            RECT& rect = *reinterpret_cast<RECT*>(lparam);
            Size  size = Rect(rect).size();

            min.width  += ws.width  - cs.width;
            min.height += ws.height - cs.height;
            max.width  += ws.width  - cs.width;
            max.height += ws.height - cs.height;

            if (size.width < min.width) {
                if (border[wparam] & L) rect.left = rect.right - min.width;
                else
                if (border[wparam] & R) rect.right = rect.left + min.width;
            } else if (size.width > max.width) {
                if (border[wparam] & L) rect.left = rect.right - max.width;
                else
                if (border[wparam] & R) rect.right = rect.left + max.width;
            }

            if (size.height < min.height) {
                if (border[wparam] & T) rect.top = rect.bottom - min.height;
                else
                if (border[wparam] & B) rect.bottom = rect.top + min.height;
            } else if (size.height > max.height) {
                if (border[wparam] & T) rect.top = rect.bottom - max.height;
                else
                if (border[wparam] & B) rect.bottom = rect.top + max.height;
            }
        }
        break;
    case WM_MOVE:
        if (client)
            client->xy(Point {
                static_cast<short>(LOWORD(lparam)),
                static_cast<short>(HIWORD(lparam))
            });
        if (windows_event__.find(hwnd) != windows_event__.end())
            windows_event__[hwnd].onMoveHandle();
        break;
    case WM_SIZE:
        if (client)
            client->size(Size { LOWORD(lparam), HIWORD(lparam) });
        break;
    case WM_SHOWWINDOW:
        activate_window__ = hwnd;
        if (client)
            client->visible(wparam);
        break;
    case WM_MOUSEACTIVATE:
        activate_window__ = hwnd;
        if (windows_style__[hwnd] & wsNoActivate)
            return MA_NOACTIVATE;
        break;
    case WM_ACTIVATE:
        activate_window__ = hwnd;
        if (windows_event__.find(hwnd) != windows_event__.end())
            windows_event__[hwnd].onActivateHandle(wparam);
    case WM_LBUTTONDOWN:   case WM_MBUTTONDOWN:   case WM_RBUTTONDOWN:
    case WM_NCLBUTTONDOWN: case WM_NCMBUTTONDOWN: case WM_NCRBUTTONDOWN:
        if (windows_evanescent__.find(hwnd) == windows_evanescent__.end())
            for (window::Handle handle : windows_evanescent__)
                DestroyWindow(handle);
        break;
    case WM_MOUSEMOVE:
        if (!client)
            cursor::set(cursor::ctArrow);
        break;
    case WM_CHAR:
        if (prechar[hwnd] > 0x7f) {
            wparam |= prechar[hwnd] << 8;
            prechar[hwnd] = 0;
        } else if (wparam > 0x7f) {
            prechar[hwnd] = wparam;
            return 0;
        }
        break;
    case WM_CLOSE:
        {
            bool agree = true;
            if (windows_event__.find(hwnd) != windows_event__.end())
                windows_event__[hwnd].onCloseHandle(agree);
            if (!agree)
                return 0;
        }
        break;
    case WM_NCDESTROY:
        if (windows_event__.find(hwnd) != windows_event__.end())
            windows_event__[hwnd].onDestroyHandle();
        if (capture_widget__ && capture_widget__->forefather() == client)
            capture_widget__ = nullptr;
        if (activate_window__ == hwnd)
            activate_window__ = nullptr;
        erase_handle(hwnd);
        if (windows__.empty())
            quit();
        return 0;
    case takisy::gui::omidUserMessage:
        {
            struct user_message_handler {
                takisy::gui::args* args;
            public:
                user_message_handler(LPARAM lparam)
                    : args(reinterpret_cast<takisy::gui::args*>(lparam)) {}
               ~user_message_handler(void) { delete args; }
            public:
                long handle(int msgid) {
                    return args->widget->onMessage(msgid, args->userdata);
                }
            };

            return user_message_handler(lparam).handle(wparam);
        }
    default:
        break;
    };

    if (client)
        return widgetProc(hwnd, client, msg, wparam, lparam);
    else
        return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK window::implement::widgetProc
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

            graphics graphics;
            graphics.impl_->initialize(wdc, ps.rcPaint);
            graphics.clear(widget->color_scheme().background());
            paint(widget, graphics, ps.rcPaint);
            graphics.impl_->paint();

            EndPaint(hwnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        {
            Point       ht_point  = cursor_point();
            LPWIDGET    ht_widget = hittest(widget, ht_point);
            sys::Button button    = sys::btnNone;

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

            if (!onEvent(ht_widget, onSetCursor, ht_point))
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
            if (wndinfo.move.first)
            {
                ReleaseCapture();
                wndinfo.move.first = false;
            }

            Point       ht_point  = cursor_point();
            LPWIDGET    ht_widget = hittest(widget, ht_point);
            sys::Button button    = sys::btnNone;

            switch (msg)
            {
            case WM_LBUTTONUP: button = sys::btnLeft;   break;
            case WM_MBUTTONUP: button = sys::btnMiddle; break;
            case WM_RBUTTONUP: button = sys::btnRight;  break;
            }

            if (wndinfo.click.widget
                && (wndinfo.click.widget == ht_widget
                    || wndinfo.click.widget->is_senior(ht_widget)))
            {
                ht_point  = cursor_point();
                ht_widget = hittest(widget, ht_point,
                                    wndinfo.click.widget, false);
                onEvent(ht_widget, onMouseUp,
                        button, wndinfo.click.times, ht_point);
            }
            else
                onEvent(ht_widget, onMouseUp, button, 0, ht_point);
        }
        break;
    case WM_MOUSEWHEEL:
        {
            LPWIDGET focus = activate_window__
                   ? focus__[activate_window__] : focus__[hwnd];
            if (focus && focus->disabled())
                focus = nullptr;

            Point    ht_point  = cursor_point();
            LPWIDGET ht_widget = hittest(widget, ht_point, focus);
            int      delta     = GET_WHEEL_DELTA_WPARAM(wparam);

            onEvent(ht_widget, onMouseWheel, delta, ht_point);
        }
        break;
    case WM_KEYDOWN:
    case WM_CHAR:
    case WM_KEYUP:
        {
            LPWIDGET focus = activate_window__
                   ? focus__[activate_window__] : focus__[hwnd];
            if (!focus)
                focus = widget;

            if (focus && focus->enabled())
            {
                switch (msg)
                {
                case WM_KEYDOWN:
                    onEvent(focus, onKeyDown, (sys::VirtualKey)(wparam));
                    break;
                case WM_CHAR:
                    onEvent(focus, onKeyPress, wparam);
                    break;
                case WM_KEYUP:
                    onEvent(focus, onKeyUp, (sys::VirtualKey)(wparam));
                    break;
                }
            }
        }
        break;
    default:
        break;
    }

    #undef onEvent
    #undef onEventStop

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

window::window(void)
    : window(nullptr)
{}

window::window(unsigned long wndstyle)
    : window(create(wndstyle))
{}

window::window(Handle handle)
    : impl_(new implement(handle))
{}

window::window(const window& wnd)
    : window(wnd.impl_->handle_)
{}

window::~window(void)
{
    delete impl_;
}

window& window::operator=(const window& wnd)
{
    if (this != &wnd)
        impl_->handle_ = wnd.impl_->handle_;

    return *this;
}

window::Handle window::create(void)
{
    return create(wsCommonWindow);
}

window::Handle window::create(unsigned long wndstyle)
{
    const char* wndclass;
    DWORD exstyle = 0, style = 0;
    if (wndstyle & wsAlphaChannel)  exstyle |= WS_EX_LAYERED;
    if (wndstyle & wsTaskbar); else exstyle |= WS_EX_TOOLWINDOW;
    if (wndstyle & wsTopmost)       exstyle |= WS_EX_TOPMOST;
    if (wndstyle & wsCaption)         style |= WS_CAPTION;
    if (wndstyle & wsSysMenu)         style |= WS_SYSMENU;
    if (wndstyle & wsMinBox)          style |= WS_MINIMIZEBOX;
    if (wndstyle & wsMinimize)        style |= WS_MINIMIZE;
    if (wndstyle & wsMaxBox)          style |= WS_MAXIMIZEBOX;
    if (wndstyle & wsMaximize)        style |= WS_MAXIMIZE;
    if (wndstyle & wsResizable)       style |= WS_SIZEBOX;
    if (wndstyle & wsPopup)           style |= WS_POPUP;
    if (wndstyle & wsDropShadow)
        wndclass = "takisy::gui::window.drop_shadow";
    else
        wndclass = "takisy::gui::window";

    Handle handle = CreateWindowEx(
                    exstyle, wndclass, "Cross Platform Window",
                    style, CW_USEDEFAULT, CW_USEDEFAULT,
                           CW_USEDEFAULT, CW_USEDEFAULT,
                    nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
    if (handle)
    {
        takisy::gui::windows__.insert(handle);
        takisy::gui::windows_style__[handle] = wndstyle;

        if (wndstyle & wsVisible)
            window(handle).show();
        if (wndstyle & wsEvanescent)
            takisy::gui::windows_evanescent__.insert(handle);
    }

    return handle;
}

long window::exec(void)
{
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

void window::quit(void)
{
    quit(0);
}

void window::quit(long quitcode)
{
    PostQuitMessage(quitcode);
}

bool window::key_pressed(sys::VirtualKey vkey)
{
    return 0x0100 & GetKeyState(vkey);
}

bool window::key_pressed(const std::vector<sys::VirtualKey>& vkeys)
{
    for (sys::VirtualKey vkey : vkeys)
        if (!key_pressed(vkey))
            return false;

    return true;
}

window::Handle window::handle(void) const
{
    return impl_->handle_;
}

std::wstring window::caption(void) const
{
    if (!impl_->handle_)
        return std::wstring();

    static wchar_t caption[1024];
    int length = GetWindowTextW(impl_->handle_, caption, 1024);

    return std::wstring(caption, length);
}

int window::x(void) const
{
    return rect().left;
}

int window::y(void) const
{
    return rect().top;
}

Point window::xy(void) const
{
    return rect().left_top();
}

unsigned int window::width(void) const
{
    return rect().width();
}

unsigned int window::height(void) const
{
    return rect().height();
}

Size window::size(void) const
{
    return rect().size();
}

Rect window::rect(void) const
{
    if (!impl_->handle_)
        return {0, 0, 0, 0};

    RECT rect;

    if (GetWindowRect(impl_->handle_, &rect))
        return rect;
    else
        return {0, 0, 0, 0};
}

Point window::client_offset(void) const
{
    if (!impl_->handle_)
        return {0, 0};

    POINT offset = {0, 0};

    if (ClientToScreen(impl_->handle_, &offset))
        return Point(offset) - xy();
    else
        return {0, 0};
}

Size window::client_size(void) const
{
    if (!impl_->handle_)
        return {0, 0};

    RECT rect;

    if (GetClientRect(impl_->handle_, &rect))
        return Rect(rect).size();
    else
        return {0, 0};
}

bool window::visible(void) const
{
    if (!impl_->handle_)
        return false;

    return IsWindowVisible(impl_->handle_);
}

struct window::event& window::event(void) const
{
    return takisy::gui::windows_event__[handle()];
}

void window::handle(Handle handle)
{
    impl_->handle_ = handle;
}

void window::caption(const std::string& _caption)
{
    caption(_caption, sys::default_codec());
}

void window::caption(const std::string& _caption,
                                    const std::string& codec)
{
    caption(stralgo::decode(_caption, codec));
}

void window::caption(const std::wstring& caption)
{
    if (!impl_->handle_)
        return;

    SetWindowTextW(impl_->handle_, caption.c_str());
}

void window::x(int x)
{
    Rect rct = rect();
    rect(rct.move(x, rct.top));
}

void window::y(int y)
{
    Rect rct = rect();
    rect(rct.move(rct.left, y));
}

void window::xy(int x, int y)
{
    rect(rect().move(x, y));
}

void window::xy(const Point& _xy)
{
    xy(_xy.x, _xy.y);
}

void window::width(unsigned int width)
{
    Rect rct = rect();

    rct.width(width);

    rect(rct);
}

void window::height(unsigned int height)
{
    Rect rct = rect();

    rct.height(height);

    rect(rct);
}

void window::size(unsigned int width, unsigned int height)
{
    Rect rct = rect();

    rct.width(width);
    rct.height(height);

    rect(rct);
}

void window::size(const Size& _size)
{
    size(_size.width, _size.height);
}

void window::rect(int x, int y, unsigned int width, unsigned int height)
{
    MoveWindow(handle(), x, y, width, height, false);
}

void window::rect(const Point& xy, const Size& size)
{
    rect(xy.x, xy.y, xy.x + size.width, xy.y + size.height);
}

void window::rect(const Rect& _rect)
{
    rect(_rect.left, _rect.top, _rect.width(), _rect.height());
}

void window::client_width(unsigned int width)
{
    client_size(width, client_size().height);
}

void window::client_height(unsigned int height)
{
    client_size(client_size().width, height);
}

void window::client_size(unsigned int width, unsigned int height)
{
    client_size(Size(width, height));
}

void window::client_size(const Size& clientsize)
{
    size(clientsize + size() - client_size());
}

void window::visible(bool visible)
{
    (void)(visible ? show() : hide());
}

void window::show(void)
{
    if (!impl_->handle_)
        return;

    ShowWindow(impl_->handle_,
               takisy::gui::windows_style__[impl_->handle_] & wsNoActivate
                    ? SW_SHOWNA : SW_SHOW);
}

void window::show_normal(void)
{
    if (!impl_->handle_)
        return;

    ShowWindow(impl_->handle_,
               takisy::gui::windows_style__[impl_->handle_] & wsNoActivate
                    ? SW_SHOWNOACTIVATE : SW_SHOWNORMAL);
}

void window::show_minimize(void)
{
    if (!impl_->handle_)
        return;

    ShowWindow(impl_->handle_, SW_SHOWMINIMIZED);
}

void window::show_maximize(void)
{
    if (!impl_->handle_)
        return;

    ShowWindow(impl_->handle_, SW_SHOWMAXIMIZED);
}

void window::hide(void)
{
    if (!impl_->handle_)
        return;

    ShowWindow(impl_->handle_, SW_HIDE);
}

void window::move_center(void)
{
    int cxscreen = GetSystemMetrics(SM_CXSCREEN);
    int cyscreen = GetSystemMetrics(SM_CYSCREEN);
    int x = (cxscreen - width())  / 2;
    int y = (cyscreen - height()) / 2;

    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;

    xy(x, y);
}

void window::capture(bool capture)
{
    if (!impl_->handle_)
        return;

    if (capture)
        SetCapture(impl_->handle_);
    else
        ReleaseCapture();
}

void window::repaint(void)
{
    if (!impl_->handle_)
        return;

    InvalidateRect(impl_->handle_, nullptr, TRUE);
}

void window::repaint(const Rect& rect)
{
    if (rect.empty() || !impl_->handle_)
        return;

    RECT r;

    r.left   = rect.left;
    r.top    = rect.top;
    r.right  = rect.right;
    r.bottom = rect.bottom;

    InvalidateRect(impl_->handle_, &r, TRUE);
}

void window::close(void)
{
    if (!impl_->handle_)
        return;

    PostMessage(impl_->handle_, WM_CLOSE, 0, 0);
}
