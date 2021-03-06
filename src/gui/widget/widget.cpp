#include <map>
#include <set>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <takisy/core/algo.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/widget.h>
#include "../basic/impl/color_scheme.hpp"

namespace takisy {
namespace gui
{
    struct args { widget* widget; void* userdata; };

    extern UINT
        user_msgid__;
    extern widget*
        capture_widget__;
    extern std::map<window::Handle, widget*>
        windows_as__, focus__;
    extern std::set<window::Handle>
        windows_widget__;

    window::Handle handleFromLPWIDGET(const widget* widget)
    {
        for (auto& pair : windows_as__)
            if (pair.second == widget)
                return pair.first;

        return nullptr;
    }
}}

class widget::implement
{
    friend class widget;

    static constexpr unsigned int limit = 1 << 16;

public:
    implement(void)
        : father_(nullptr)
        , enabled_(true), visible_(false)
        , rect_(0, 0, 0, 0), lower_(0, 0), upper_(limit, limit)
        , id_(id()), color_scheme_(nullptr)
    {}

    ~implement(void)
    {
        if (color_scheme_)
            delete color_scheme_;
    }

public:
    std::vector<widget*>::iterator find_child(widget* widget)
    {
        return std::find(children_.begin(), children_.end(), widget);
    }

private:
    static inline unsigned int id(void)
    {
        static unsigned int id = 0;

        return ++id;
    }

private:
    widget* father_;
    std::vector<widget*> children_;
    bool enabled_, visible_;
    Rect rect_;
    Size lower_, upper_;
    const unsigned int id_;
    std::map<std::string, std::string> properties_;
    class color_scheme* color_scheme_;
};

widget::widget(void)
    : impl_(new implement)
{}

widget::~widget(void)
{
    for (widget* child : children())
        child->father(nullptr);

    focus(false);
    father(nullptr);
    as_window(nullptr);
    capture(false);

    delete impl_;
}

widget::widget(const widget& widget)
{
    operator=(widget);
}

widget& widget::operator=(const widget&)
{
    throw std::logic_error("class widget is noncopyable class.");
}

unsigned int widget::id(void) const
{
    return impl_->id_;
}

widget* widget::father(void) const
{
    return impl_->father_;
}

widget* widget::forefather(void)
{
    widget* forefather = this;

    while (forefather->father())
        forefather = forefather->father();

    return forefather;
}

const widget* widget::forefather(void) const
{
    return const_cast<widget*>(this)->forefather();
}

std::vector<widget*> widget::children(void) const
{
    return impl_->children_;
}

bool widget::exists_property(const std::string& name) const
{
    return impl_->properties_.find(name) != impl_->properties_.end();
}

std::string widget::property(const std::string& name) const
{
    return exists_property(name) ? impl_->properties_[name] : "";
}

int widget::x(void) const
{
    return impl_->rect_.left;
}

int widget::y(void) const
{
    return impl_->rect_.top;
}

Point widget::xy(void) const
{
    return impl_->rect_.left_top();
}

unsigned int widget::width(void) const
{
    return impl_->rect_.width();
}

unsigned int widget::height(void) const
{
    return impl_->rect_.height();
}

Size widget::size(void) const
{
    return impl_->rect_.size();
}

Rect widget::rect(void) const
{
    return impl_->rect_;
}

bool widget::enabled(void) const
{
    return !disabled();
}

bool widget::disabled(void) const
{
    const widget* widget = this;

    while (widget && widget->impl_->enabled_)
        widget = widget->father();

    return widget;
}

bool widget::visible(void) const
{
    return impl_->visible_;
}

bool widget::focused(void) const
{
    using takisy::gui::handleFromLPWIDGET;
    Window::Handle handle = handleFromLPWIDGET(forefather());

    return handle ? takisy::gui::focus__[handle] == this : false;
}

Rect widget::client_rect(void) const
{
    return Rect(0, 0, width(), height());
}

int widget::window_x(void) const
{
    return window_xy().x;
}

int widget::window_y(void) const
{
    return window_xy().y;
}

Point widget::window_xy(void) const
{
    return screen_xy() - forefather()->xy();
}

Rect widget::window_rect(void) const
{
    return Rect(window_xy(), size());
}

int widget::screen_x(void) const
{
    return screen_xy().x;
}

int widget::screen_y(void) const
{
    return screen_xy().y;
}

Point widget::screen_xy(void) const
{
    const widget* widget = this;
    Point point = xy();

    while ((widget = widget->father()))
        point += widget->xy();

    return point;
}

Rect widget::screen_rect(void) const
{
    return Rect(screen_xy(), size());
}

unsigned int widget::lower_width(void) const
{
    return lower_size().width;
}

unsigned int widget::lower_height(void) const
{
    return lower_size().height;
}

Size widget::lower_size(void) const
{
    return impl_->lower_;
}

unsigned int widget::upper_width(void) const
{
    return upper_size().width;
}

unsigned int widget::upper_height(void) const
{
    return upper_size().height;
}

Size widget::upper_size(void) const
{
    return impl_->upper_;
}

bool widget::father(widget* father)
{
    if (this->father() == father)
        return true;

    if (!father || forefather() != father->forefather())
        focus(false);

    if (impl_->father_)
        impl_->father_->remove(this);

    if (father)
        return father->add(this);

    return true;
}

bool widget::add(widget* widget)
{
    return add(widget, impl_->children_.size());
}

bool widget::add(widget* widget, unsigned int index)
{
    if (!widget || widget == this || is_senior(widget) || widget->is_window())
        return false;
    if (!onAdding(widget))
        return false;

    if (widget->father())
        widget->father()->remove(widget);
    impl_->children_.insert(impl_->children_.begin() + index, widget);
    widget->impl_->father_ = this;

    onAdd(widget);
    repaint();

    return true;
}

bool widget::remove(widget* widget)
{
    typedef std::vector<class widget*>::iterator widget_iterator;
    widget_iterator child = impl_->find_child(widget);
    if (child == impl_->children_.end())
        return false;
    if (!onRemoving(widget))
        return false;

    impl_->children_.erase(child);
    widget->impl_->father_ = nullptr;

    onRemove(widget);
    repaint();

    return true;
}

void widget::property(const std::string& name, const std::string& value)
{
    impl_->properties_[name] = value;
}

void widget::x(int x)
{
    xy(x, y());
}

void widget::y(int y)
{
    xy(x(), y);
}

void widget::xy(int x, int y)
{
    xy(Point(x, y));
}

void widget::xy(Point _xy)
{
    if (!onMoving(_xy))
        return;
    if (impl_->father_ && !impl_->father_->onChildMoving(this, _xy))
        return;

    if (_xy == xy())
        return;

    using takisy::gui::handleFromLPWIDGET;
    Window::Handle handle = handleFromLPWIDGET(this);

    if (!handle)
    {
        repaint();
        impl_->rect_ = impl_->rect_.move(_xy);
        repaint();
    }
    else
    {
        Window window(handle);

        impl_->rect_ = impl_->rect_.move(_xy);
        window.xy(_xy - window.client_offset());
    }

    onMove();
    if (impl_->father_)
        impl_->father_->onChildMove(this);
}

void widget::width(unsigned int width)
{
    size(width, height());
}

void widget::height(unsigned int height)
{
    size(width(), height);
}

void widget::size(unsigned int width, unsigned int height)
{
    size(Size(width, height));
}

void widget::size(Size _size)
{
    if (!onSizing(_size))
        return;
    if (impl_->father_ && !impl_->father_->onChildSizing(this, _size))
        return;

    _size.width  = algo::clamp(_size.width,
            impl_->lower_.width, impl_->upper_.width);
    _size.height = algo::clamp(_size.height,
            impl_->lower_.height, impl_->upper_.height);

    if (_size == size())
        return;

    repaint();
    impl_->rect_.size(_size);
    repaint();

    onSize();
    if (impl_->father_)
        impl_->father_->onChildSize(this);

    window().client_size(_size);
}

void widget::rect(int x, int y, unsigned int width, unsigned int height)
{
    rect(Rect(x, y, x + width, y + height));
}

void widget::rect(const Point& xy, const Size& size)
{
    rect(Rect(xy, size));
}

void widget::rect(const Rect& rect)
{
    Rect r = rect.normalize();

    xy(r.left_top());
    size(r.size());
}

void widget::enable(void)
{
    if (impl_->enabled_)
        return;

    if (!onEnabling())
        return;
    if (impl_->father_ && !impl_->father_->onChildEnabling(this))
        return;

    impl_->enabled_ = true;

    onEnable();
    if (father())
        father()->onChildEnable(this);

    repaint();
}

void widget::disable(void)
{
    if (!impl_->enabled_)
        return;

    if (!onDisabling())
        return;
    if (impl_->father_ && !impl_->father_->onChildDisabling(this))
        return;

    impl_->enabled_ = false;

    onDisable();
    if (father())
        father()->onChildDisable(this);

    repaint();
}

void widget::visible(bool visible)
{
    if (impl_->visible_ == visible)
        return;

    if (visible)
    {
        if (!onShowing())
            return;
        if (impl_->father_ && !impl_->father_->onChildShowing(this))
            return;

        impl_->visible_ = true;

        onShown();
        if (father())
            father()->onChildShown(this);
    }
    else
    {
        if (!onHiding())
            return;
        if (impl_->father_ && !impl_->father_->onChildHiding(this))
            return;

        impl_->visible_ = false;

        onHidden();
        if (father())
            father()->onChildHidden(this);
    }

    window().visible(visible);
    if (father())
        father()->repaint(rect());
    else
        repaint();
}

void widget::show(void)
{
    visible(true);
}

void widget::hide(void)
{
    visible(false);
}

void widget::focus(bool focused)
{
    using takisy::gui::handleFromLPWIDGET;
    Window::Handle handle = handleFromLPWIDGET(forefather());
    if (!handle)
        return;

    if (focused)
    {
        if (takisy::gui::focus__[handle] != this)
        {
            widget* previous = takisy::gui::focus__[handle];
            takisy::gui::focus__[handle] = this;
            if (previous)
                previous->onFocus(false);
            onFocus(true);
        }
    }
    else if (takisy::gui::focus__[handle] == this)
    {
        takisy::gui::focus__[handle] = nullptr;
        onFocus(false);
    }
}

void widget::capture(bool capture)
{
    Window window = forefather()->window();

    if (capture) {
        if (window.handle()) {
            takisy::gui::capture_widget__ = this;
            window.capture(true);
        }
    } else if (takisy::gui::capture_widget__ == this) {
        takisy::gui::capture_widget__ = nullptr;
        if (window.handle())
            window.capture(false);
    }
}

void widget::lower_width(unsigned int lowerwidth)
{
    if (lowerwidth > impl_->upper_.width)
        lowerwidth = impl_->upper_.width;
    impl_->lower_.width = lowerwidth;
    width(width());
}

void widget::lower_height(unsigned int lowerheight)
{
    if (lowerheight > impl_->upper_.height)
        lowerheight = impl_->upper_.height;
    impl_->lower_.height = lowerheight;
    height(height());
}

void widget::lower_size(unsigned int lowerwidth, unsigned int lowerheight)
{
    lower_width(lowerwidth);
    lower_height(lowerheight);
}

void widget::lower_size(const Size& lowersize)
{
    lower_size(lowersize.width, lowersize.height);
}

void widget::upper_width(unsigned int upperwidth)
{
    if (upperwidth < impl_->lower_.width)
        upperwidth = impl_->lower_.width;
    if (upperwidth > implement::limit)
        upperwidth = implement::limit;
    impl_->upper_.width = upperwidth;
    width(width());
}

void widget::upper_height(unsigned int upperheight)
{
    if (upperheight < impl_->lower_.height)
        upperheight = impl_->lower_.height;
    if (upperheight > implement::limit)
        upperheight = implement::limit;
    impl_->upper_.height = upperheight;
    height(height());
}

void widget::upper_size(unsigned int upperwidth, unsigned int upperheight)
{
    upper_width(upperwidth);
    upper_height(upperheight);
}

void widget::upper_size(const Size& uppersize)
{
    upper_size(uppersize.width, uppersize.height);
}

void widget::absolute_width(unsigned int width)
{
    lower_width(width);
    upper_width(width);
}

void widget::absolute_height(unsigned int height)
{
    lower_height(height);
    upper_height(height);
}

void widget::absolute_size(unsigned int width, unsigned int height)
{
    lower_size(width, height);
    upper_size(width, height);
}

void widget::absolute_size(const Size& size)
{
    lower_size(size);
    upper_size(size);
}

class color_scheme widget::color_scheme(void)
{
    class color_scheme* color_scheme = impl_->color_scheme_;
    const widget* widget = father();

    while (!color_scheme && widget)
    {
        color_scheme = widget->impl_->color_scheme_;
        widget = widget->father();
    }

    class color_scheme retcs =
        color_scheme ? *color_scheme : color_scheme::default_color_scheme();
    retcs.impl_->host(this);

    return retcs;
}

class color_scheme* widget::p_color_scheme(void)
{
    return impl_->color_scheme_;
}

const class color_scheme* widget::p_color_scheme(void) const
{
    return impl_->color_scheme_;
}

class color_scheme& widget::color_scheme(const class color_scheme* colorscheme)
{
    if (!impl_->color_scheme_)
    {
        if (colorscheme)
            impl_->color_scheme_ = new class color_scheme(*colorscheme);
        else
            impl_->color_scheme_ = new class color_scheme(color_scheme());

        impl_->color_scheme_->impl_->host(this);
        repaint();
    }
    else if (colorscheme)
    {
       *impl_->color_scheme_ = *colorscheme;
        impl_->color_scheme_->impl_->host(this);
        repaint();
    }

    return *impl_->color_scheme_;
}

void widget::repaint(void)
{
    repaint(client_rect());
}

void widget::repaint(int x, int y, unsigned int width, unsigned int height)
{
    repaint(Rect(x, y, x + width, y + height));
}

void widget::repaint(const Point& xy, const Size& size)
{
    repaint(Rect(xy, size));
}

void widget::repaint(const Rect& rect)
{
    if (!visible() || rect.empty())
        return;

    Rect    paint_rect = rect;
    widget* widget = this;

    while (widget->father())
    {
        if (!widget->visible())
            return;

        paint_rect = paint_rect.offset(widget->xy());
        widget     = widget->father();
        paint_rect = paint_rect.intersect(widget->client_rect());

        if (paint_rect.empty())
            return;
    }

    widget->window().repaint(paint_rect);
}

long widget::emitmsg(int msgid, void* userdata)
{
    Window::Handle handle = window().handle();
    if (handle)
        return SendMessage(handle, takisy::gui::user_msgid__, msgid,
                           (LPARAM)new takisy::gui::args { this, userdata });

    return 0;
}

void widget::async_emitmsg(int msgid, void* userdata)
{
    Window::Handle handle = window().handle();
    if (handle)
        PostMessage(handle, takisy::gui::user_msgid__, msgid,
                    (LPARAM)new takisy::gui::args { this, userdata });
}

bool widget::is_child(const widget* widget) const
{
    return impl_->find_child(const_cast<class widget*>(widget))
        != impl_->children_.end();
}

bool widget::is_senior(const widget* widget) const
{
    class widget* w = father();

    while (w && w != widget)
        w = w->father();

    return w;
}

bool widget::is_junior(const widget* widget) const
{
    return widget && widget->is_senior(this);
}

bool widget::as_window(void)
{
    return as_window(Window::wsWidgetWindow);
}

bool widget::as_window(unsigned long wndstyle)
{
    if (father())
        return false;
    if (is_window())
        return true;

    auto handle = Window::create(wndstyle);
    if (!handle)
        return false;
    else
        takisy::gui::windows_widget__.insert(handle);

    return as_window(handle);
}

bool widget::as_window(Window window)
{
    return as_window(window, true);
}

bool widget::as_window(Window window, bool reset)
{
    using namespace takisy::gui;

    if (father())
        return false;

    Window::Handle
        as_handle = handleFromLPWIDGET(this), handle = window.handle();

    if (as_handle)
    {
        windows_as__.erase(as_handle);

        if (windows_widget__.find(as_handle) != windows_widget__.end())
            Window(as_handle).close();
        else
            Window(as_handle).repaint();
    }

    if (handle)
    {
        if (reset)
        {
            window.xy(xy() - window.client_offset());
            window.client_size(size());
            window.visible(visible());
        }
        else
        {
            xy(window.xy() + window.client_offset());
            size(window.client_size());
            visible(window.visible());
        }

        window.repaint();
        windows_as__[handle] = this;
    }

    return true;
}

bool widget::as_window(Window::Handle handle)
{
    return as_window(Window(handle));
}

bool widget::as_window(Window::Handle handle, bool reset)
{
    return as_window(Window(handle), reset);
}

bool widget::is_window(void) const
{
    return !!takisy::gui::handleFromLPWIDGET(this);
}

widget::Window widget::window(void) const
{
    return Window(takisy::gui::handleFromLPWIDGET(this));
}

Size widget::optimal(OptimalPolicy policy) const
{
    return size();
}

bool widget::inside(int x, int y) const
{
    return impl_->rect_.inside(x, y);
}

bool widget::onAdding(widget*)                    { return true;  }
void widget::onAdd(widget*)                       {               }
bool widget::onRemoving(widget*)                  { return true;  }
void widget::onRemove(widget*)                    {               }

bool widget::onMoving(Point&)                     { return true;  }
void widget::onMove(void)                         {               }
bool widget::onSizing(Size&)                      { return true;  }
void widget::onSize(void)                         {               }
bool widget::onEnabling(void)                     { return true;  }
void widget::onEnable(void)                       {               }
bool widget::onDisabling(void)                    { return true;  }
void widget::onDisable(void)                      {               }
bool widget::onShowing(void)                      { return true;  }
void widget::onShown(void)                        {               }
bool widget::onHiding(void)                       { return true;  }
void widget::onHidden(void)                       {               }

bool widget::onChildMoving(widget*, Point&)       { return true;  }
void widget::onChildMove(widget*)                 {               }
bool widget::onChildSizing(widget*, Size&)        { return true;  }
void widget::onChildSize(widget*)                 {               }
bool widget::onChildEnabling(widget*)             { return true;  }
void widget::onChildEnable(widget*)               {               }
bool widget::onChildDisabling(widget*)            { return true;  }
void widget::onChildDisable(widget*)              {               }
bool widget::onChildShowing(widget*)              { return true;  }
void widget::onChildShown(widget*)                {               }
bool widget::onChildHiding(widget*)               { return true;  }
void widget::onChildHidden(widget*)               {               }

void widget::onPaint(graphics, Rect)              {               }
void widget::onEndPaint(graphics, Rect)           {               }
bool widget::onFocus(bool)                        { return false; }
bool widget::onSetCursor(Point)                   { return false; }
bool widget::onKeyDown(sys::VirtualKey)           { return false; }
bool widget::onKeyPress(unsigned int)             { return false; }
bool widget::onKeyUp(sys::VirtualKey)             { return false; }
bool widget::onMouseDown(sys::Button, int, Point) { return false; }
bool widget::onMouseUp(sys::Button, int, Point)   { return false; }
bool widget::onMouseMove(Point)                   { return false; }
bool widget::onMouseEnter(void)                   { return false; }
bool widget::onMouseLeave(void)                   { return false; }
bool widget::onMouseWheel(int, Point)             { return false; }
long widget::onMessage(int, void*)                { return 0;     }
