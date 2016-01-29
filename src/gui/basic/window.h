#ifndef window_h_20170717
#define window_h_20170717

#include <map>
#include <Windows.h>
#include <takisy/gui/widget/widget.h>

class graphics;

class Window
{
    typedef class widget* LPWIDGET;
    class Initializer;

private:
    Window(HWND hwnd);
public:
   ~Window(void);

private:
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

public:
    HWND hwnd(void) const;
    HDC  hdc(void) const;

public:
    void caption(const char* caption);
    void xy(int x, int y);
    void size(unsigned width, unsigned height);
    void visible(bool visible);
    void topmost(bool topmost);
    void showInTaskbar(bool visible);
    void setCompositionWindow(int x, int y);

    void repaint(const widget::Rect& rect);
    void destroy(void);

public:
    void sendMessage(UINT msg, WPARAM wparam, LPARAM lparam);
    void postMessage(UINT msg, WPARAM wparam, LPARAM lparam);

public:
    static widget::Point getCursorPoint(void);
    static bool          setCurosrPoint(const widget::Point& point);

public:
    static bool     create(LPWIDGET widget, bool layered_window);
    static Window*  find(LPWIDGET widget);
    static void     capture(LPWIDGET widget);
    static LPWIDGET hittest(LPWIDGET widget, widget::Point& point);

private:
    static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
    static LRESULT CALLBACK WidgetProc(HWND, LPWIDGET, UINT, WPARAM, LPARAM);

    static void OnWidgetPaint(graphics&, LPWIDGET, const widget::Rect&);

    #define onEvent(lpwidget, method, params...)                              \
        ({ LPWIDGET w = lpwidget; widget::Point p = ht_point;                 \
           while (w) {                                                        \
               if (w->method(params)) break;                                  \
               else if (w->attribute<bool>("intercept."#method)) w = nullptr; \
               else { ht_point += w->xy(); w = w->father(); } }               \
           ht_point = p; w; })

    #define onEvent2(stopwidget, lpwidget, method, params...)                 \
        ({ LPWIDGET w = lpwidget; widget::Point p = ht_point;                 \
           while (w) {                                                        \
               if (w == stopwidget || w->method(params)) break;               \
               else if (w->attribute<bool>("intercept."#method)) w = nullptr; \
               else { ht_point += w->xy(); w = w->father(); } }               \
           ht_point = p; w; })

public:
    #define doIfAsWindow(w, m, ps...) \
        ({ if (!w->pure()) { Window* W = Window::find(w); if (W) W->m(ps); } })

private:
    HWND hwnd_;
    HDC  hdc_;

    static const char* class_name__;
    static class Initializer initializer__;
    static LPWIDGET captured_widget__;
    static std::map<const LPWIDGET, Window*> all_windows__;
};

#endif // window_h_20170717
