#include <utility>
#include <Shobjidl.h>
#include <takisy/core/algorithm.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/basic/cursor.h>
#include <takisy/gui/basic/graphics.h>
#include "impl/graphics.hpp"
#include "device_context.h"
#include "window.h"

class WindowsDeviceContext : public device_context
{
public:
    WindowsDeviceContext(HDC hdc)
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

class Window::Initializer
{
public:
    Initializer(void)
    {
        WNDCLASSEX cls = {
            .cbSize        = sizeof(WNDCLASSEX),
            .style         = CS_HREDRAW | CS_VREDRAW,
            .lpfnWndProc   = WindowProc,
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
        CoInitialize(nullptr);
    }

    ~Initializer(void)
    {
        CoUninitialize();
        UnregisterClass(class_name__, GetModuleHandle(nullptr));
    }
};

const char*         Window::class_name__ = "takisy::gui::alpha_window";
Window::Initializer Window::initializer__;
Window::LPWIDGET    Window::captured_widget__ = nullptr;
std::map<const Window::LPWIDGET, Window*> Window::all_windows__;

Window::Window(HWND hwnd)
    : hwnd_(hwnd), hdc_(GetDC(hwnd))
{}

Window::~Window(void)
{
    ReleaseDC(hwnd_, hdc_);
    DestroyWindow(hwnd_);
}

HWND Window::hwnd(void) const
{
    return hwnd_;
}

HDC Window::hdc(void) const
{
    return hdc_;
}

void Window::caption(const char* caption)
{
    SetWindowText(hwnd_, caption);
}

void Window::xy(int x, int y)
{
    SetWindowPos(hwnd_, 0, x, y, 0, 0, SWP_NOSIZE);
}

void Window::size(unsigned width, unsigned height)
{
    SetWindowPos(hwnd_, 0, 0, 0, width, height, SWP_NOMOVE);
}

void Window::visible(bool visible)
{
    ShowWindow(hwnd_, visible ? SW_SHOW : SW_HIDE);
}

void Window::topmost(bool topmost)
{
    SetWindowPos(hwnd_, topmost ? HWND_TOPMOST : HWND_NOTOPMOST,
                 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void Window::showInTaskbar(bool visible)
{
    static const GUID CLSID_TaskbarList = { 0x56FDF344, 0xFD6D, 0x11D0,
        { 0x95, 0x8A, 0x00, 0x60, 0x97, 0xC9, 0xA0, 0x90, } };
    static const GUID IID_ITaskbarList  = { 0x56FDF342, 0xFD6D, 0x11D0,
        { 0x95, 0x8A, 0x00, 0x60, 0x97, 0xC9, 0xA0, 0x90, } };
    ITaskbarList* taskbar_list;
    HRESULT result = CoCreateInstance(CLSID_TaskbarList, nullptr,
                                      CLSCTX_INPROC_SERVER, IID_ITaskbarList,
                                      reinterpret_cast<void**>(&taskbar_list));
    if (!SUCCEEDED(result))
        return;

    taskbar_list->HrInit();

    if (visible)
        taskbar_list->AddTab(hwnd_);
    else
        taskbar_list->DeleteTab(hwnd_);

    taskbar_list->Release();
}

void Window::setCompositionWindow(int x, int y)
{
    COMPOSITIONFORM cf;

    cf.dwStyle = CFS_FORCE_POSITION;
    cf.ptCurrentPos.x = x;
    cf.ptCurrentPos.y = y;

    ImmSetCompositionWindow(ImmGetContext(hwnd_), &cf);
}

void Window::repaint(const widget::Rect& r)
{
    if (r.empty())
        return;

    RECT rect;
    rect.left   = r.left;
    rect.top    = r.top;
    rect.right  = r.right;
    rect.bottom = r.bottom;

    InvalidateRect(hwnd_, &rect, FALSE);
}

void Window::destroy(void)
{
    DestroyWindow(hwnd_);
}

void Window::sendMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
    SendMessage(hwnd_, msg, wparam, lparam);
}

void Window::postMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
    PostMessage(hwnd_, msg, wparam, lparam);
}

widget::Point Window::getCursorPoint(void)
{
    POINT point;

    if (!GetCursorPos(&point))
        return widget::Point(0, 0);

    return widget::Point(point.x, point.y);
}

bool Window::setCurosrPoint(const widget::Point& point)
{
    return SetCursorPos(point.x, point.y);
}

bool Window::create(LPWIDGET widget, bool layered_window)
{
    if (!widget || widget->father())
        return false;

    if (!find(widget))
    {
        DWORD exstyle = layered_window ? WS_EX_LAYERED : 0;
        DWORD style   = WS_POPUP | (widget->visible() ? WS_VISIBLE : 0);
        HWND  hwnd    = CreateWindowEx(exstyle, class_name__,
                                       stralgo::format("[id:%d]", widget->id()),
                                       style, widget->x(), widget->y(),
                                       widget->width(), widget->height(),
                                       GetDesktopWindow(), nullptr,
                                       GetModuleHandle(nullptr), widget);

        if (!hwnd)
            return false;
        else
            InvalidateRect(hwnd, nullptr, FALSE);

        all_windows__[widget] = new Window(hwnd);
    }

    return true;
}

Window* Window::find(LPWIDGET widget)
{
    return all_windows__.find(widget) != all_windows__.end()
                                       ? all_windows__[widget] : nullptr;
}

void Window::capture(LPWIDGET widget)
{
    if (widget == captured_widget__)
        return;

    if (!widget)
    {
        ReleaseCapture();
        captured_widget__ = nullptr;
    }
    else
    {
        Window* wnd = find(widget->forefather());
        if (wnd)
            SetCapture(wnd->hwnd());
        captured_widget__ = widget;
    }
}

Window::LPWIDGET Window::hittest(LPWIDGET widget, widget::Point& point)
{
    if (captured_widget__)
    {
        LPWIDGET bar = captured_widget__->father();
        while (bar)
        {
            point -= bar->xy();
            bar    = bar->father();
        }

        if (!captured_widget__->inside(point.x, point.y))
        {
            point -= captured_widget__->xy();
            return captured_widget__;
        }
        else
            widget = captured_widget__;
    }

    class david
    {
    public:
        static Window::LPWIDGET hittest(LPWIDGET widget, widget::Point& point)
        {
            if (!widget || !widget->inside(point.x, point.y))
                return nullptr;
            else
                point -= widget->xy();

            for (LPWIDGET child : widget->children())
            {
                if (!child->visible())
                    continue;

                LPWIDGET ht_widget = hittest(child, point);
                if (ht_widget)
                    return ht_widget;
            }

            return widget;
        }
    };

    return david::hittest(widget, point);
}

LRESULT CALLBACK Window::WindowProc
        (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    struct WindowData
    {
        LPWIDGET      widget  = nullptr;
        unsigned char prechar = 0;
    };
    static std::map<HWND, WindowData> wnddata;

    switch (msg)
    {
    case WM_CREATE:
        wnddata[hwnd].widget = reinterpret_cast<LPWIDGET>(
                               reinterpret_cast<CREATESTRUCT*>
                                                    (lparam)->lpCreateParams);
        break;
    case WM_SIZING:
        {
            enum { None = 0, L = 0x1000, T = 0x100, R = 0x10, B = 0x1 };
            static unsigned int border[] =
                 { None, L, R, T, T | L, T | R, B, B | L, B | R };
            widget::Size min_sz = wnddata[hwnd].widget->minimal_size();
            widget::Size max_sz = wnddata[hwnd].widget->maximal_size();
            RECT&        rect   = *reinterpret_cast<RECT*>(lparam);
            unsigned int width  = rect.right  - rect.left;
            unsigned int height = rect.bottom - rect.top;

            if (width < min_sz.width) {
                if (border[wparam] & L) rect.left = rect.right - min_sz.width;
                else
                if (border[wparam] & R) rect.right = rect.left + min_sz.width;
            } else if (width > max_sz.width) {
                if (border[wparam] & L) rect.left = rect.right - max_sz.width;
                else
                if (border[wparam] & R) rect.right = rect.left + max_sz.width;
            }

            if (height < min_sz.height) {
                if (border[wparam] & T) rect.top = rect.bottom - min_sz.height;
                else
                if (border[wparam] & B) rect.bottom = rect.top + min_sz.height;
            } else if (height > max_sz.height) {
                if (border[wparam] & T) rect.top = rect.bottom - max_sz.height;
                else
                if (border[wparam] & B) rect.bottom = rect.top + max_sz.height;
            }
        }
        break;
    case WM_MOVE:
    case WM_SIZE:
    case WM_SHOWWINDOW:
        {
            LPWIDGET widget = wnddata[hwnd].widget;

            widget->pure(true);

            switch (msg)
            {
            case WM_MOVE:
                widget->xy((short)LOWORD(lparam), (short)HIWORD(lparam));
                break;
            case WM_SIZE:
                widget->size((short)LOWORD(lparam), (short)HIWORD(lparam));
                break;
            case WM_SHOWWINDOW:
                widget->visible((bool)(BOOL)wparam);
                break;
            }

            widget->pure(false);
        }
        break;
    case WM_CHAR:
        if (wnddata[hwnd].prechar > 0x7f) {
            wparam |= wnddata[hwnd].prechar << 8;
            wnddata[hwnd].prechar = 0;
        } else if (wparam > 0x7f) {
            wnddata[hwnd].prechar = wparam;
            return 0;
        }
        break;
    case WM_NCDESTROY:
        {
            Window* window = all_windows__[wnddata[hwnd].widget];
            all_windows__.erase(wnddata[hwnd].widget);
            wnddata.erase(hwnd);
            delete window;
            if (wnddata.empty())
                PostQuitMessage(0);
        }
        return 0;
    case WM_ERASEBKGND:
        return 0;
    default:
        break;
    };

    return WidgetProc(hwnd, wnddata[hwnd].widget, msg, wparam, lparam);
}

LRESULT CALLBACK Window::WidgetProc
        (HWND hwnd, LPWIDGET widget, UINT msg, WPARAM wparam, LPARAM lparam)
{
    struct WindowInformation
    {
        graphics graphics;
        std::pair<bool, widget::Point> move;
        LPWIDGET focus, enter, cursor;
        struct Click {
            unsigned int times;
            LPWIDGET widget;
            widget::Point point;
            sys::MouseButton button;
            DWORD pretime;
        } click;

    public:
        WindowInformation(void)
            : move(std::make_pair(false, widget::Point(0, 0)))
            , focus(nullptr), enter(nullptr), cursor(nullptr)
            , click({0, nullptr, widget::Point(0, 0), sys::mbNone, 0})
        {}
    };

    static std::map<HWND, WindowInformation> sWindowInformations;
    WindowInformation& wndinfo = sWindowInformations[hwnd];
    widget::Point ht_point;

    switch (msg)
    {
    case WM_PAINT:
        if (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_LAYERED)
        {
            widget::Rect cr = widget->client_rect();

            HDC     hdc = all_windows__[widget]->hdc();
            HDC     cdc = CreateCompatibleDC(hdc);
            HBITMAP bmp = CreateCompatibleBitmap(hdc, cr.width(), cr.height());
            WindowsDeviceContext wdc(cdc);

            SelectObject(cdc, bmp);
            wndinfo.graphics.impl_->initialize(&wdc, cr);
            OnWidgetPaint(wndinfo.graphics, widget, cr);
            wndinfo.graphics.impl_->paint();

            SIZE          size  = { cr.width(), cr.height() };
            POINT         point = { 0, 0 };
            BLENDFUNCTION bf    = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
            UpdateLayeredWindow(hwnd, hdc, nullptr, &size,
                                      cdc, &point, 0, &bf, ULW_ALPHA);

            DeleteObject(bmp);
            DeleteObject(cdc);
        }
        else
        {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            WindowsDeviceContext wdc(ps.hdc);

            graphics graphics;
            graphics.impl_->initialize(&wdc, ps.rcPaint);
            OnWidgetPaint(graphics, widget, ps.rcPaint);
            graphics.impl_->paint();

            EndPaint(hwnd, &ps);
        }
        break;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
        {
            widget::Point    ht_point  = getCursorPoint();
            LPWIDGET         ht_widget = hittest(widget, ht_point);
            sys::MouseButton button    = sys::mbNone;

            switch (msg)
            {
            case WM_LBUTTONDOWN: button = sys::mbLButton; break;
            case WM_MBUTTONDOWN: button = sys::mbMButton; break;
            case WM_RBUTTONDOWN: button = sys::mbRButton; break;
            }

            if (button == sys::mbLButton)
            {
                LPWIDGET fw = onEvent2(wndinfo.focus, ht_widget, onFocus, true);
                if (fw && wndinfo.focus != fw)
                {
                    if (wndinfo.focus)
                        wndinfo.focus->onFocus(false);
                    wndinfo.focus = fw;
                }
            }

            WindowInformation::Click& ci = wndinfo.click;
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
                wndinfo.move = std::make_pair(true, getCursorPoint());
                widget->capture(true);
            }
        }
        break;
    case WM_MOUSEMOVE:
        if (wndinfo.move.first)
        {
            widget->capture(false);
            widget::Point offset = getCursorPoint() - wndinfo.move.second;
            widget->xy(widget->xy() + offset);
            SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
            wndinfo.move.first = false;
        }
        else
        {
            widget::Point ht_point  = getCursorPoint();
            LPWIDGET      ht_widget = hittest(widget, ht_point);

            if (!onEvent2(wndinfo.cursor, ht_widget, onSetCursor))
                cursor::set(cursor::ctArrow);

            LPWIDGET ew = onEvent2(wndinfo.enter, ht_widget, onMouseEnter);
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
            widget::Point    ht_point  = getCursorPoint();
            LPWIDGET         ht_widget = hittest(widget, ht_point);
            sys::MouseButton button    = sys::mbNone;

            switch (msg)
            {
            case WM_LBUTTONUP: button = sys::mbLButton; break;
            case WM_MBUTTONUP: button = sys::mbMButton; break;
            case WM_RBUTTONUP: button = sys::mbRButton; break;
            }

            if (button == wndinfo.click.button)
            {
                onEvent(ht_widget, onClick,
                        button, wndinfo.click.times, ht_point);
                if (wndinfo.click.times == 1
                    && ht_widget->exists_attribute("cmdid"))
                    onEvent(ht_widget->father(), onCommand, ht_widget,
                            ht_widget->attribute<const char*>("cmdid"));
            }

            onEvent(ht_widget, onMouseUp, button, ht_point);
        }
        break;
    case WM_MOUSEWHEEL:
        {
            widget::Point ht_point  = getCursorPoint();
            LPWIDGET      ht_widget = hittest(widget, ht_point);
            int           delta     = GET_WHEEL_DELTA_WPARAM(wparam);

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

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::OnWidgetPaint
        (graphics& graphics, LPWIDGET widget, const widget::Rect& rect)
{
    widget->onPaint(graphics, rect);

    for (LPWIDGET child : widget->children())
    {
        if (!child->visible())
            continue;

        widget::Rect paint_rect = rect.intersect(child->rect());
        if (paint_rect.empty())
            continue;

        paint_rect = paint_rect.offset(-child->xy());
        class graphics child_graphics = graphics;
        child_graphics.impl_->paint_area(child->xy(), paint_rect);
        OnWidgetPaint(child_graphics, child, paint_rect);
    }

    widget->onEndPaint(graphics, rect);
}
