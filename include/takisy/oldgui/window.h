#ifndef window_h_20130709
#define window_h_20130709

#include <Windows.h>
#include <memory>
#include <string>
#include <takisy/oldgui/basic/define.h>
#include <takisy/oldgui/widget/widget.h>

class Window
{
private:
    class Implement;

public:
    struct EventParams
    {
        Window& window;
        UINT    msg;
        WPARAM  wparam;
        LPARAM  lparam;
    };

    class EventListener
    {
    public:
        virtual ~EventListener(void);
        virtual void onEvent(const EventParams& ep) = 0;
    };

public:
    static CREATESTRUCT defaultCreateStruct(void);

public:
    Window(void);
    Window(HWND hwnd);
    Window(const Window& wnd);
    explicit
    Window(const std::string& caption);
    Window(const std::string& caption, bool alpha_window);
    Window(const std::string& caption, int width, int height);
    Window(const std::string& caption, int width, int height,
           bool alpha_window);
    Window(const std::string& caption, int x, int y, int width, int height);
    Window(const std::string& caption, int x, int y, int width, int height,
           bool alpha_window);
    Window(const CREATESTRUCT& craete_struct);
    Window(const CREATESTRUCT& craete_struct, bool alpha_window);
   ~Window(void);

    Window& operator=(const Window& wnd);

public:
    void create(const CREATESTRUCT& craete_struct);
    void create(const CREATESTRUCT& craete_struct, bool alpha_window);
    void destroy(void);

    template <typename Lambda>
    void listen(UINT msg, Lambda event_listener);
    void listen(UINT msg, std::shared_ptr<EventListener> event_listener);
    void forget(UINT msg);

    void attach(HWND hwnd);
    void detach(void);

public:
    void addWidget(Widget* widget);
    bool existsWidget(Widget* widget);
    void removeWidget(Widget* widget);

public:
    std::string caption(void) const;

    Window parent(void) const;
    Window forefather(void) const;

    DWORD style(void) const;
    DWORD exstyle(void) const;

    bool sizeBox(void) const;
    bool minimizeBox(void) const;
    bool maximizeBox(void) const;

    bool living(void) const;
    bool visible(void) const;
    bool minimized(void) const;
    bool maximized(void) const;

    HWND hwnd(void) const;
    HDC  hdc(void) const;

    Rect rect(void) const;
    Point xy(void) const;
    int x(void) const;
    int y(void) const;
    Size size(void) const;
    unsigned int width(void) const;
    unsigned int height(void) const;
    Size clientSize(void) const;
    unsigned int clientWidth(void) const;
    unsigned int clientHeight(void) const;

public:
    void caption(const std::string& caption);

    void parent(HWND hwnd);
    void parent(const Window& parent);

    void style(DWORD style);
    void exstyle(DWORD exstyle);

    void sizeBox(bool effective);
    void minimizeBox(bool effective);
    void maximizeBox(bool effective);

    void repaint(void);
    void repaint(const Rect& rect);
    void repaint(const Point& xy, const Size& size);
    void repaint(int x, int y, unsigned int width, unsigned int height);

    void show(void);
    void showMinimized(void);
    void showMaximized(void);
    void showNormal(void);
    void hide(void);

    void x(int x);
    void y(int y);
    void xy(int x, int y);
    void xy(const Point& xy);
    void width(unsigned int width);
    void height(unsigned int height);
    void size(unsigned int width, unsigned int height);
    void size(const Size& size);
    void clientWidth(unsigned int width);
    void clientHeight(unsigned int height);
    void clientSize(unsigned int width, unsigned int height);
    void clientSize(const Size& size);
    void rect(int x, int y, unsigned int width, unsigned int height);
    void rect(const Point& xy, const Size& size);
    void rect(const Rect& rect);
    void background_color(Color color);

    void opacity(double factor);
    void topmost(void);

public:
    HWND setCapture(void);
    bool releaseCapture(void);

    LRESULT sendMessage(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT postMessage(UINT msg, WPARAM wparam, LPARAM lparam);

    int msgbox(const char* msg);
    int msgbox(const char* msg, const char* caption);
    int msgbox(const char* msg, const char* caption, UINT type);

public:
    static int  run(void);
    static void quit(void);

private:
    class Implement* impl_;
};

template <typename Lambda>
void Window::listen(UINT msg, Lambda lambda)
{
    class LambdaListener : public Window::EventListener
    {
    public: inline LambdaListener(Lambda lambda) : lambda_(lambda) {}
    public: inline void onEvent(const Window::EventParams& ep) override
        { lambda_(ep); }
    private: Lambda lambda_;
    };

    listen(msg, std::shared_ptr<EventListener>(new LambdaListener(lambda)));
}

#endif //window_h_20130709
