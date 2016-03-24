#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <takisy/core/dynamic_linking_loader.h>
#include <takisy/oldgui/basic/exception.h>
#include <takisy/oldgui/window.h>
#include "impl/graphics.hpp"

#define assert(condition) \
    if (!(condition))       \
        throw takisy::oldgui::Exception::system_exception(GetLastError())

namespace global
{
    std::list<OldWindow*>   windowWithWidgets;
    std::map<HWND, Widget*> capturedWidget;
    std::map<HWND, Widget*> focusedWidget;
}

OldWindow::EventListener::~EventListener(void) {}

class OldWindow::Implement
{
    friend class OldWindow;

    class Container : public Widget
    {
    public:
        inline void set_background_color(Color color)
        {
            background_color_ = color;
            repaint();
        }

    public:
        inline void onPaint(Graphics graphics) override
        {
            if (background_color_.a)
                graphics.clear(background_color_);
        }

    private:
        Color background_color_;
    };

    template <typename... Params>
    class EventProcessor
    {
    public:
        template <bool (Widget::*Method)(Params..., Point)>
        bool do_event(Widget* widget, Params... params, Point hit_point) const
        {
            while (widget)
                if ((widget->*Method)(params..., hit_point)) return true;
                else hit_point += widget->xy(), widget = widget->parent();
            return false;
        }

        template <bool (Widget::*Method)(Params...)>
        bool do_event_keyboard(Widget* widget, Params... params) const
        {
            while (widget)
                if ((widget->*Method)(params...)) return true;
                else widget = widget->parent();
            return false;
        }
    };

    template <typename... Params>
    static inline EventProcessor<Params...> get_event_processor(Params...)
    {
        return EventProcessor<Params...>();
    }

    #define DO_EVENT(widget, method, hit_point, params...) \
        get_event_processor(params).do_event<method>(widget, params, hit_point)
    #define DO_EVENT_KEYBOARD(widget, method, params...) \
        get_event_processor(params).do_event_keyboard<method>(widget, params)

public:
    Implement(void)
        : hwnd_(nullptr), created_(false), alpha_window_(false)
    {
        register_class();
    }

    ~Implement(void)
    {
        deregister_class();
    }

public:
    void register_event(UINT msg, std::shared_ptr<EventListener> listener)
    {
        event_listeners_[msg].push_back(listener);
    }

    void deregister_event(UINT msg)
    {
        event_listeners_.erase(msg);
    }

private:
    static const char   sClassName[];
    static unsigned int sRegisterTimes;

public:
    static void register_class(void)
    {
        if (sRegisterTimes++ == 0)
        {
            WNDCLASSEX cls = {
                .cbSize        = sizeof(WNDCLASSEX),
                .style         = CS_HREDRAW | CS_VREDRAW,
                .lpfnWndProc   = WindowProc,
                .cbClsExtra    = 0,
                .cbWndExtra    = 0,
                .hInstance     = GetModuleHandle(nullptr),
                .hIcon         = LoadIcon(nullptr, IDI_APPLICATION),
                .hCursor       = LoadCursor(nullptr, IDC_ARROW),
                .hbrBackground = HBRUSH(COLOR_WINDOW),
                .lpszMenuName  = nullptr,
                .lpszClassName = sClassName,
                .hIconSm       = LoadIcon(nullptr, IDI_APPLICATION),
            };
            assert(RegisterClassEx(&cls));
        }
    }

    static void deregister_class(void)
    {
        if (--sRegisterTimes == 0)
            assert(UnregisterClass(sClassName, GetModuleHandle(nullptr)));
    }

    static LRESULT CALLBACK WindowProc(HWND   hwnd,
                                       UINT   msg,
                                       WPARAM wparam,
                                       LPARAM lparam)
    {
        static std::map<HWND, OldWindow*> windows;
        static std::map<HWND, unsigned char> last_char;

        switch (msg)
        {
        case WM_CREATE:
            windows[hwnd] = reinterpret_cast<OldWindow*>(
                    reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams);
            break;

        case WM_ACTIVATE:
            {
                auto window = std::find(global::windowWithWidgets.begin(),
                                        global::windowWithWidgets.end(),
                                        windows[hwnd]);
                if (window != global::windowWithWidgets.end()
                    && window != global::windowWithWidgets.begin())
                    std::swap(global::windowWithWidgets.front(), *window);
            }
            break;

        case WM_SIZE:
            windows[hwnd]->impl_->container_.size(windows[hwnd]->clientSize());
            break;

        case WM_CHAR:
            if (last_char[hwnd] > 0x7f) {
                wparam |= last_char[hwnd] << 8;
                last_char[hwnd] = 0;
            } else {
                last_char[hwnd] = wparam;
                if (wparam > 0x7f)
                    return 0;
            }
            break;

        case WM_DESTROY:
            windows[hwnd]->detach();
            global::windowWithWidgets.remove(windows[hwnd]);
            windows.erase(hwnd);
            if (windows.empty())
                OldWindow::quit();
            break;

        default:
            break;
        }

        if (windows.find(hwnd) != windows.end())
        {
            OldWindow* window = windows[hwnd];

            auto& listeners = window->impl_->event_listeners_;
            if (listeners.find(msg) != listeners.end())
                for (auto listener : listeners[msg])
                    listener->onEvent(EventParams {
                        .window = *window,
                        .msg    = msg,
                        .wparam = wparam,
                        .lparam = lparam
                    });

            if (!window->impl_->container_.children().empty())
            {
                std::vector<Widget*> container;
                container.push_back(&window->impl_->container_);
                onWidgetEvent(container, *window, msg, wparam, lparam);
            }
        }

        switch (msg)
        {
        case WM_ERASEBKGND: return 0;
        default: return DefWindowProc(hwnd, msg, wparam, lparam);
        }
    }

    static void onWidgetEvent(const std::vector<Widget*>& widgets,
                              OldWindow& window,
                              UINT       msg,
                              WPARAM     wparam,
                              LPARAM     lparam)
    {
        class util
        {
        public:
            static inline KeyState key_state(void)
            {
                return KeyState {
                    .button_left   = !!(0x0100 & GetKeyState(vkLButton)),
                    .button_middle = !!(0x0100 & GetKeyState(vkMButton)),
                    .button_right  = !!(0x0100 & GetKeyState(vkRButton)),
                    .button_x1     = !!(0x0100 & GetKeyState(vkXButton1)),
                    .button_x2     = !!(0x0100 & GetKeyState(vkXButton2)),
                    .shift         = !!(0x0100 & GetKeyState(vkShift)),
                    .shift_left    = !!(0x0100 & GetKeyState(vkLShift)),
                    .shift_right   = !!(0x0100 & GetKeyState(vkRShift)),
                    .ctrl          = !!(0x0100 & GetKeyState(vkControl)),
                    .ctrl_left     = !!(0x0100 & GetKeyState(vkLControl)),
                    .ctrl_right    = !!(0x0100 & GetKeyState(vkRControl)),
                    .alt           = !!(0x0100 & GetKeyState(vkMenu)),
                    .alt_left      = !!(0x0100 & GetKeyState(vkLMenu)),
                    .alt_right     = !!(0x0100 & GetKeyState(vkRMenu)),
                };
            }

            static inline Point point(LPARAM lparam)
            {
                return Point {
                    .x = static_cast<short>(lparam),
                    .y = static_cast<short>(lparam >> 16),
                };
            }

            static inline Point screen2client(HWND hwnd, const Point& point)
            {
                POINT bar = {point.x, point.y};
                ScreenToClient(hwnd, &bar);
                return Point(bar.x, bar.y);
            }

            static Widget* hit_test(HWND hwnd,
                                    const std::vector<Widget*>& widgets,
                                    Point& hit_point)
            {
                if (hwnd && global::capturedWidget.find(hwnd)
                         != global::capturedWidget.end()
                    && global::capturedWidget[hwnd])
                {
                    Widget* widget = global::capturedWidget[hwnd];
                    do { hit_point -= widget->xy(); widget = widget->parent(); }
                    while (widget);
                    return global::capturedWidget[hwnd];
                }

                typedef decltype(widgets.rbegin()) iterator_type;
                iterator_type iterator = widgets.rbegin();
                for ( ; iterator != widgets.rend(); ++iterator)
                {
                    Widget* widget = *iterator;
                    if (!widget->visible())
                        continue;
                    Widget* hitted_widget = widget->hit_test(hit_point);
                    if (!hitted_widget)
                        continue;
                    hit_point -= hitted_widget->global_xy();
                    return hitted_widget;
                }

                return nullptr;
            }

            static void onWidgetPaint(const std::vector<Widget*>& widgets,
                                      Rect paint_rect,
                                      Graphics& graphics)
            {
                for (Widget* child : widgets)
                {
                    if (!child->visible())
                        continue;
                    Rect crct = child->rect();
                    Point offset = crct.left_top();
                    Rect cpr = paint_rect.intersect(crct).offset(-offset);
                    if (cpr.empty())
                        continue;
                    Graphics child_graphics = graphics;
                    child_graphics.impl_->paintArea(offset, cpr);
                    child->onPaint(child_graphics);
                    onWidgetPaint(child->children(), cpr, child_graphics);
                }
            }
        };

        struct ClickInfo
        {
            DWORD last_time;
            int times;
            Point point;
            MouseButton button;

        public:
            ClickInfo(void)
                : last_time(0), times(0), point(-1, -1), button(mbNoneButton)
            {}
        };

        static std::map<Widget*, ClickInfo> click_info;
        HWND hwnd = window.hwnd();

        switch (msg)
        {
        case WM_PAINT:
            if (window.impl_->alpha_window_)
            {
                RECT client_rect;
                GetClientRect(hwnd, &client_rect);
                Rect rect = client_rect;

                HDC hdc = GetDC(hwnd);
                HDC cdc = CreateCompatibleDC(hdc);
                HBITMAP bitmap =
                    CreateCompatibleBitmap(hdc, rect.width(), rect.height());
                SelectObject(cdc, bitmap);

                Graphics graphics(cdc, rect);
                util::onWidgetPaint(widgets, rect, graphics);
                graphics.endpaint(true);

                SIZE size = {rect.width(), rect.height()};
                POINT position = {0, 0};
                BLENDFUNCTION bf = {
                    .BlendOp = AC_SRC_OVER,
                    .BlendFlags = 0,
                    .SourceConstantAlpha = 255,
                    .AlphaFormat = AC_SRC_ALPHA,
                };
                UpdateLayeredWindow(hwnd, hdc, nullptr, &size, cdc,
                                    &position, RGB(0, 0, 0), &bf, ULW_ALPHA);

                DeleteObject(bitmap);
                DeleteObject(cdc);
                ReleaseDC(hwnd, hdc);
            }
            else
            {
                PAINTSTRUCT ps;
                BeginPaint(hwnd, &ps);
                {
                    Rect paint_rect = ps.rcPaint;
                    Graphics graphics(ps);
                    graphics.impl_->canvas_->clear(Color::white());
                    util::onWidgetPaint(widgets, paint_rect, graphics);
                }
                EndPaint(hwnd, &ps);
            }
            break;

        case WM_MOUSEMOVE:
            do
            {
                Point hit_point    = util::point(lparam);
                Widget* widget     = util::hit_test(hwnd, widgets, hit_point);
                static Widget* prewdt = nullptr; // last widget
                KeyState key_state = util::key_state();
                TRACKMOUSEEVENT track_mouse_event = {
                    .cbSize      = sizeof(TRACKMOUSEEVENT),
                    .dwFlags     = TME_HOVER,
                    .hwndTrack   = hwnd,
                    .dwHoverTime = HOVER_DEFAULT,
                };

                TrackMouseEvent(&track_mouse_event);

                DO_EVENT(widget, Widget::onMouseMove, hit_point, key_state);
                if (widget) widget->onSetCursor();
                if (prewdt == widget) break;
                DO_EVENT(prewdt, Widget::onMouseLeave, hit_point, key_state);
                DO_EVENT(widget, Widget::onMouseEnter, hit_point, key_state);
                prewdt = widget;
            } while (false);
            break;

        case WM_MOUSEHOVER:
            {
                Point    hit_point = util::point(lparam);
                Widget*     widget = util::hit_test(hwnd, widgets, hit_point);
                KeyState key_state = util::key_state();

                DO_EVENT(widget, Widget::onMouseHover, hit_point, key_state);
            }
            break;

        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
            {
                Point hit_point = util::point(lparam);
                Widget*  widget = util::hit_test(hwnd, widgets, hit_point);

                if (global::focusedWidget[hwnd])
                    global::focusedWidget[hwnd]->onFocus(false);
                global::focusedWidget[hwnd] = widget;

                if (widget)
                {
                    widget->onFocus(true);

                    MouseButton button = mbNoneButton;

                    switch (msg)
                    {
                    case WM_LBUTTONDOWN: button = mbLeftButton;   break;
                    case WM_MBUTTONDOWN: button = mbMiddleButton; break;
                    case WM_RBUTTONDOWN: button = mbRightButton;  break;
                    }

                    click_info[widget].times =
                               button        == click_info[widget].button
                            && hit_point     == click_info[widget].point
                            && GetTickCount() - click_info[widget].last_time
                                    < GetDoubleClickTime()
                                ? click_info[widget].times + 1 : 1;
                    click_info[widget].button    = button;
                    click_info[widget].point     = hit_point;
                    click_info[widget].last_time = GetTickCount();

                    DO_EVENT(widget, Widget::onMouseDown, hit_point,
                             button, util::key_state());
                }
            }
            break;

        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
            {
                Point hit_point = util::point(lparam);
                Widget*  widget = util::hit_test(hwnd, widgets, hit_point);

                if (widget)
                {
                    MouseButton button = mbNoneButton;
                    KeyState key_state = util::key_state();

                    switch (msg)
                    {
                    case WM_LBUTTONUP: button = mbLeftButton;   break;
                    case WM_MBUTTONUP: button = mbMiddleButton; break;
                    case WM_RBUTTONUP: button = mbRightButton;  break;
                    }

                    if (button == click_info[widget].button)
                        DO_EVENT(widget, Widget::onClick, hit_point,
                                 button, key_state, click_info[widget].times);

                    DO_EVENT(widget, Widget::onMouseUp, hit_point,
                             button, key_state);
                }
            }
            break;

        case WM_MOUSEWHEEL:
            {
                Point hit_point = util::point(lparam);
                      hit_point = util::screen2client(hwnd, hit_point);
                Widget*  widget = util::hit_test(hwnd, widgets, hit_point);
                int       delta = GET_WHEEL_DELTA_WPARAM(wparam);

                DO_EVENT(widget, Widget::onMouseWheel, hit_point,
                         util::key_state(), delta);
            }
            break;

        case WM_KEYDOWN:
            DO_EVENT_KEYBOARD(global::focusedWidget[hwnd], Widget::onKeyDown,
                              static_cast<VirtualKey>(wparam),
                              util::key_state());
            break;

        case WM_CHAR:
            DO_EVENT_KEYBOARD(global::focusedWidget[hwnd], Widget::onKeyPress,
                              static_cast<unsigned int>(wparam),
                              util::key_state());
            break;

        case WM_KEYUP:
            DO_EVENT_KEYBOARD(global::focusedWidget[hwnd], Widget::onKeyUp,
                              static_cast<VirtualKey>(wparam),
                              util::key_state());
            break;

        default:
            break;
        }
    }

    #undef DO_EVENT
    #undef DO_EVENT_KEYBOARD

private:
    typedef std::shared_ptr<EventListener> EventListenerPtr;

private:
    HDC  hdc_;
    HWND hwnd_;
    bool created_;
    bool alpha_window_;
    Container container_;
    std::map<UINT, std::vector<EventListenerPtr>> event_listeners_;
};

const char   OldWindow::Implement::sClassName[]   = "takisy::oldgui::OldWindow";
unsigned int OldWindow::Implement::sRegisterTimes = 0;

CREATESTRUCT OldWindow::defaultCreateStruct(void)
{
    return CREATESTRUCT {
        .lpCreateParams = nullptr,
        .hInstance      = GetModuleHandle(nullptr),
        .hMenu          = nullptr,
        .hwndParent     = GetDesktopWindow(),
        .cy             = 400,
        .cx             = 300,
        .y              = 0,
        .x              = 0,
        .style          = WS_OVERLAPPEDWINDOW,
        .lpszName       = "takisy.oldgui",
        .lpszClass      = OldWindow::Implement::sClassName,
        .dwExStyle      = WS_EX_APPWINDOW,
    };
}

OldWindow::OldWindow(void)
    : impl_(new Implement)
{}

OldWindow::OldWindow(HWND hwnd)
    : impl_(new Implement)
{
    attach(hwnd);
}

OldWindow::OldWindow(const OldWindow& window)
    : impl_(new Implement)
{
    operator=(window);
}

OldWindow::OldWindow(const std::string& caption)
    : OldWindow(caption, false)
{}

OldWindow::OldWindow(const std::string& caption, bool alpha_window)
    : OldWindow(caption, 400, 300, alpha_window)
{}

OldWindow::OldWindow(const std::string& caption, int width, int height)
    : OldWindow(caption, width, height, false)
{}

OldWindow::OldWindow(const std::string& caption,
               int width, int height, bool alpha_window)
    : OldWindow(caption, 0, 0, width, height, alpha_window)
{}

OldWindow::OldWindow(const std::string& caption, int x, int y, int width, int height)
    : OldWindow(caption, x, y, width, height, false)
{}

OldWindow::OldWindow(const std::string& caption,
               int x, int y, int width, int height, bool alpha_window)
    : impl_(new Implement)
{
    CREATESTRUCT create_struct = defaultCreateStruct();

    create_struct.lpszName = caption.c_str();
    create_struct.x        = x;
    create_struct.y        = y;
    create_struct.cx       = width;
    create_struct.cy       = height;

    create(create_struct, alpha_window);
}

OldWindow::OldWindow(const CREATESTRUCT& create_struct)
    : OldWindow(create_struct, false)
{}

OldWindow::OldWindow(const CREATESTRUCT& create_struct, bool alpha_window)
    : impl_(new Implement)
{
    create(create_struct, alpha_window);
}

OldWindow::~OldWindow(void)
{
    destroy();
    delete impl_;
}

OldWindow& OldWindow::operator=(const OldWindow& window)
{
    if (this != &window)
        attach(window.hwnd());

    return *this;
}

void OldWindow::create(const CREATESTRUCT& create_struct)
{
    return create(create_struct, false);
}

void OldWindow::create(const CREATESTRUCT& create_struct, bool alpha_window)
{
    if (hwnd())
        throw takisy::oldgui::Exception("HWND is not null.");

    CREATESTRUCT cs_copy = create_struct;
    if ((impl_->alpha_window_ = alpha_window))
    {
        cs_copy.style     = WS_POPUP | (cs_copy.style & WS_VISIBLE);
        cs_copy.dwExStyle = cs_copy.dwExStyle | WS_EX_LAYERED;
    }

    impl_->hwnd_ = CreateWindowEx(cs_copy.dwExStyle,
                                  cs_copy.lpszClass,
                                  cs_copy.lpszName,
                                  cs_copy.style,
                                  cs_copy.x,
                                  cs_copy.y,
                                  cs_copy.cx,
                                  cs_copy.cy,
                                  cs_copy.hwndParent,
                                  cs_copy.hMenu,
                                  cs_copy.hInstance, this);

    assert(hwnd());
    assert((impl_->hdc_ = GetDC(hwnd())));
    impl_->created_ = true;
}

void OldWindow::destroy(void)
{
    if (hwnd() && impl_->created_)
    {
        if (living())
        {
            if (hdc())
                assert(ReleaseDC(hwnd(), hdc()));
            assert(DestroyWindow(hwnd()));
        }

        detach();
        impl_->created_ = false;
    }
}

void OldWindow::listen(UINT msg, std::shared_ptr<EventListener> listener)
{
    impl_->register_event(msg, listener);
}

void OldWindow::forget(UINT msg)
{
    impl_->deregister_event(msg);
}

void OldWindow::attach(HWND _hwnd)
{
    if (hwnd())
        throw takisy::oldgui::Exception("HWND is not null.");
    impl_->hwnd_ = _hwnd;
    assert((impl_->hdc_ = GetDC(hwnd())));
}

void OldWindow::detach(void)
{
    impl_->hdc_  = nullptr;
    impl_->hwnd_ = nullptr;
}

void OldWindow::addWidget(Widget* widget)
{
    if (impl_->container_.children().empty())
        global::windowWithWidgets.push_back(this);

    impl_->container_.add(widget);
}

bool OldWindow::existsWidget(Widget* widget)
{
    return widget == &impl_->container_ || impl_->container_.exists(widget);
}

void OldWindow::removeWidget(Widget* widget)
{
    impl_->container_.remove(widget);

    if (impl_->container_.children().empty())
        global::windowWithWidgets.erase(
                std::find(global::windowWithWidgets.begin(),
                          global::windowWithWidgets.end(), this));
}

std::string OldWindow::caption(void) const
{
    char caption[256] = {0};

    assert(0 <= GetWindowText(hwnd(), caption, sizeof(caption)));

    return caption;
}

OldWindow OldWindow::parent(void) const
{
    return OldWindow(GetParent(hwnd()));
}

OldWindow OldWindow::forefather(void) const
{
    OldWindow&& parent_window = parent();
    if (!parent_window.hwnd())
        return OldWindow(hwnd());
    while (parent_window.parent().hwnd())
        parent_window = parent_window.parent();
    return parent_window;
}

DWORD OldWindow::style(void) const
{
    return GetWindowLong(hwnd(), GWL_STYLE);
}

DWORD OldWindow::exstyle(void) const
{
    return GetWindowLong(hwnd(), GWL_EXSTYLE);
}

bool OldWindow::sizeBox(void) const
{
    return style() & WS_SIZEBOX;
}

bool OldWindow::minimizeBox(void) const
{
    return style() & WS_MINIMIZEBOX;
}

bool OldWindow::maximizeBox(void) const
{
    return style() & WS_MAXIMIZEBOX;
}

bool OldWindow::living(void) const
{
    return IsWindow(hwnd());
}

bool OldWindow::visible(void) const
{
    return IsWindowVisible(hwnd());
}

bool OldWindow::minimized(void) const
{
    return IsIconic(hwnd());
}

bool OldWindow::maximized(void) const
{
    return IsZoomed(hwnd());
}

HWND OldWindow::hwnd(void) const
{
    return impl_->hwnd_;
}

HDC OldWindow::hdc(void) const
{
    return impl_->hdc_;
}

Rect OldWindow::rect(void) const
{
    if (hwnd())
    {
        RECT rect;
        assert(GetWindowRect(hwnd(), &rect));
        return Rect(rect.left, rect.top, rect.right, rect.bottom);
    }
    else
        return Rect(0, 0, 0, 0);
}

Point OldWindow::xy(void) const
{
    return rect().left_top();
}

int OldWindow::x(void) const
{
    return xy().x;
}

int OldWindow::y(void) const
{
    return xy().y;
}

Size OldWindow::size(void) const
{
    return rect().size();
}

unsigned int OldWindow::width(void) const
{
    return size().width;
}

unsigned int OldWindow::height(void) const
{
    return size().height;
}

Size OldWindow::clientSize(void) const
{
    if (hwnd())
    {
        RECT rect;
        assert(GetClientRect(hwnd(), &rect));
        return Size(rect.right - rect.left, rect.bottom - rect.top);
    }
    else
        return Size(0, 0);
}

unsigned int OldWindow::clientWidth(void) const
{
    return clientSize().width;
}

unsigned int OldWindow::clientHeight(void) const
{
    return clientSize().height;
}

void OldWindow::caption(const std::string& caption)
{
    assert(SetWindowText(hwnd(), caption.c_str()));
}

void OldWindow::parent(HWND _hwnd)
{
    assert(SetParent(hwnd(), _hwnd));
}

void OldWindow::parent(const OldWindow& parent)
{
    assert(SetParent(hwnd(), parent.hwnd()));
}

void OldWindow::style(DWORD style)
{
    SetLastError(0);
    SetWindowLong(hwnd(), GWL_STYLE, style);
    assert(GetLastError() == 0);
}

void OldWindow::exstyle(DWORD exstyle)
{
    SetLastError(0);
    SetWindowLong(hwnd(), GWL_EXSTYLE, exstyle);
    assert(GetLastError() == 0);
}

void OldWindow::sizeBox(bool effective)
{
    effective ? style(style() | WS_SIZEBOX) : style(style() & ~WS_SIZEBOX);
}

void OldWindow::minimizeBox(bool effective)
{
    effective ? style(style() |  WS_MINIMIZEBOX)
              : style(style() & ~WS_MINIMIZEBOX);
}

void OldWindow::maximizeBox(bool effective)
{
    effective ? style(style() |  WS_MAXIMIZEBOX)
              : style(style() & ~WS_MAXIMIZEBOX);
}

void OldWindow::repaint(void)
{
    repaint(0, 0, clientWidth(), clientHeight());
}

void OldWindow::repaint(const Rect& rect)
{
    repaint(rect.left_top(), rect.size());
}

void OldWindow::repaint(const Point& xy, const Size& size)
{
    repaint(xy.x, xy.y, size.width, size.height);
}

void OldWindow::repaint(int x, int y, unsigned int width, unsigned int height)
{
    RECT rect = {
        .left   = x,
        .top    = y,
        .right  = static_cast<LONG>(x + width),
        .bottom = static_cast<LONG>(y + height)
    };

    assert(InvalidateRect(hwnd(), &rect, FALSE));
}

void OldWindow::show(void)
{
    ShowWindow(hwnd(), SW_SHOW);
}

void OldWindow::showMinimized(void)
{
    ShowWindow(hwnd(), SW_MINIMIZE);
}

void OldWindow::showMaximized(void)
{
    ShowWindow(hwnd(), SW_MAXIMIZE);
}

void OldWindow::showNormal(void)
{
    ShowWindow(hwnd(), SW_SHOWNORMAL);
}

void OldWindow::hide(void)
{
    ShowWindow(hwnd(), SW_HIDE);
}

void OldWindow::x(int x)
{
    xy(x, y());
}

void OldWindow::y(int y)
{
    xy(x(), y);
}

void OldWindow::xy(int x, int y)
{
    assert(SetWindowPos(hwnd(), nullptr, x, y, 0, 0, SWP_NOSIZE));
}

void OldWindow::xy(const Point& _xy)
{
    xy(_xy.x, _xy.y);
}

void OldWindow::width(unsigned int width)
{
    size(width, height());
}

void OldWindow::height(unsigned int height)
{
    size(width(), height);
}

void OldWindow::size(unsigned int width, unsigned int height)
{
    assert(SetWindowPos(hwnd(), nullptr, 0, 0, width, height, SWP_NOMOVE));
}

void OldWindow::size(const Size& _size)
{
    size(_size.width, _size.height);
}

void OldWindow::clientWidth(unsigned int width)
{
    return clientSize(width, clientHeight());
}

void OldWindow::clientHeight(unsigned int height)
{
    return clientSize(clientWidth(), height);
}

void OldWindow::clientSize(unsigned int width, unsigned int height)
{
    Size window_size = size(), client_size = clientSize();

    size(width  + window_size.width  - client_size.width,
         height + window_size.height - client_size.height);
}

void OldWindow::clientSize(const Size& size)
{
    clientSize(size.width, size.height);
}

void OldWindow::rect(int x, int y, unsigned int width, unsigned int height)
{
    assert(SetWindowPos(hwnd(), nullptr, x, y, width, height, SWP_NOZORDER));
}

void OldWindow::rect(const Point& xy, const Size& size)
{
    rect(xy.x, xy.y, size.width, size.height);
}

void OldWindow::rect(const Rect& _rect)
{
    rect(_rect.left_top(), _rect.size());
}

void OldWindow::background_color(Color color)
{
    impl_->container_.set_background_color(color);
}

void OldWindow::opacity(double factor)
{
    exstyle(exstyle() | WS_EX_LAYERED);

    typedef BOOL WINAPI (*funcSWA_t)(HWND, COLORREF, BYTE, DWORD);
    static funcSWA_t SetLayeredWindowAttributes =
            dll::dlsymbol<funcSWA_t> ("user32", "SetLayeredWindowAttributes");
    if (SetLayeredWindowAttributes)
    {
        unsigned char alpha = factor < 0 ? 0 : factor > 1 ? 255 : factor * 255;
        enum { lwaColorKey = 0x1, lwaAlpha = 0x2 };

        assert(SetLayeredWindowAttributes(hwnd(), 0, alpha, lwaAlpha));
    }
}

void OldWindow::topmost(void)
{
    SetWindowPos(hwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

HWND OldWindow::setCapture(void)
{
    return SetCapture(hwnd());
}

bool OldWindow::releaseCapture(void)
{
    return ReleaseCapture();
}

LRESULT OldWindow::sendMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
    return SendMessage(hwnd(), msg, wparam, lparam);
}

LRESULT OldWindow::postMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
    return PostMessage(hwnd(), msg, wparam, lparam);
}

int OldWindow::msgbox(const char* msg)
{
    return msgbox(msg, caption().c_str());
}

int OldWindow::msgbox(const char* msg, const char* caption)
{
    return msgbox(msg, caption, MB_OK);
}

int OldWindow::msgbox(const char* msg, const char* caption, UINT type)
{
    return MessageBox(hwnd(), msg, caption, type);
}

int OldWindow::run(void)
{
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

void OldWindow::quit(void)
{
    PostQuitMessage(0);
}
