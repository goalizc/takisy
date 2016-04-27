#include <map>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <takisy/core/macro.h>
#include <takisy/core/algorithm.h>
#include <takisy/core/stretchy_buffer.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/widget.h>

#ifdef __os_win__
#include <Windows.h>
#endif

namespace takisy
{
    extern const char*
        class_name__;

    extern widget*
        captured_widget__;

    extern std::map<cross_platform_window::Handle, widget*>
        all_windows__;

    cross_platform_window::Handle handleFromLPWIDGET(const widget* widget)
    {
        for (auto& pair : all_windows__)
            if (pair.second == widget)
                return pair.first;

        return nullptr;
    }
}

class widget::implement
{
    friend class Window;
    friend class widget;

    static constexpr unsigned int limit = 1 << 20;

public:
    implement(void)
        : father_(nullptr), visible_(false), rect_(0, 0, 0, 0)
        , minimal_(0, 0), maximal_(limit, limit)
        , id_(id()), color_scheme_(nullptr)
    {}

    ~implement(void)
    {
        if (color_scheme_)
            delete color_scheme_;
    }

public:
    std::vector<widget*>::const_iterator find_child(widget* widget) const
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
    bool visible_;
    Rect rect_;
    Size minimal_, maximal_;
    const unsigned int id_;
    std::map<std::string, stretchy_buffer<unsigned char>> attributes_;
    class color_scheme* color_scheme_;
};

widget::widget(void)
    : impl_(new implement)
{}

widget::widget(widget* _father)
    : widget()
{
    father(_father);
}

widget::~widget(void)
{
    for (widget* child : children())
        child->father(nullptr);
    father(nullptr);
    as_window(nullptr);
    delete impl_;
}

widget::widget(const widget& widget)
{
    operator=(widget);
}

void widget::operator=(const widget&)
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

class color_scheme* widget::color_scheme(void)
{
    const widget* widget = this;

    while (widget)
        if (widget->impl_->color_scheme_)
            return widget->impl_->color_scheme_;
        else
            widget = widget->father();

    return &color_scheme::default_color_scheme();
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

bool widget::visible(void) const
{
    return impl_->visible_;
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

unsigned int widget::minimal_width(void) const
{
    return minimal_size().width;
}

unsigned int widget::minimal_height(void) const
{
    return minimal_size().height;
}

Size widget::minimal_size(void) const
{
    return impl_->minimal_;
}

unsigned int widget::maximal_width(void) const
{
    return maximal_size().width;
}

unsigned int widget::maximal_height(void) const
{
    return maximal_size().height;
}

Size widget::maximal_size(void) const
{
    return impl_->maximal_;
}

Size widget::optimal_size(void) const
{
    return size();
}

bool widget::father(widget* father)
{
    if (impl_->father_)
        impl_->father_->remove(this);

    if (father)
        return father->add(this);

    return true;
}

bool widget::add(widget* widget)
{
    if (!widget || widget == this || is_senior(widget) || widget->is_window())
        return false;
    if (!onAdding(widget))
        return false;

    if (widget->father())
        widget->father()->remove(widget);
    impl_->children_.push_back(widget);
    widget->impl_->father_ = this;

    onAdd(widget);
    repaint();

    return true;
}

bool widget::remove(widget* widget)
{
    typedef std::vector<class widget*>::const_iterator widget_iterator;
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

class color_scheme* widget::color_scheme(const class color_scheme* colorscheme)
{
    if (!impl_->color_scheme_)
    {
        if (!colorscheme)
            colorscheme = color_scheme();

        impl_->color_scheme_ = new class color_scheme(*colorscheme);
        repaint();
    }
    else
    if (!colorscheme)
    {
        *impl_->color_scheme_ = *colorscheme;
        repaint();
    }

    return impl_->color_scheme_;
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

    cross_platform_window::Handle handle = takisy::handleFromLPWIDGET(this);
    if (!handle)
    {
        repaint();
        impl_->rect_ = impl_->rect_.move(_xy);
        repaint();
    }
    else
        impl_->rect_ = impl_->rect_.move(_xy);

    onMove();
    if (impl_->father_)
        impl_->father_->onChildMove(this);

    if (handle)
    {
        cross_platform_window window(handle);
        window.xy(_xy - window.client_offset());
    }
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

    _size.width  = algorithm::clamp(_size.width,
            impl_->minimal_.width, impl_->maximal_.width);
    _size.height = algorithm::clamp(_size.height,
            impl_->minimal_.height, impl_->maximal_.height);
    if (_size.width  > implement::limit)
        _size.width  = 0;
    if (_size.height > implement::limit)
        _size.height = 0;

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

void widget::minimal_width(unsigned int minwidth)
{
    if (minwidth > impl_->maximal_.width)
        minwidth = impl_->maximal_.width;
    impl_->minimal_.width = minwidth;
    width(width());
}

void widget::minimal_height(unsigned int minheight)
{
    if (minheight > impl_->maximal_.height)
        minheight = impl_->maximal_.height;
    impl_->minimal_.height = minheight;
    height(height());
}

void widget::minimal_size(unsigned int minwidth, unsigned int minheight)
{
    minimal_width(minwidth);
    minimal_height(minheight);
}

void widget::minimal_size(const Size& minsize)
{
    minimal_size(minsize.width, minsize.height);
}

void widget::maximal_width(unsigned int maxwidth)
{
    if (maxwidth < impl_->minimal_.width)
        maxwidth = impl_->minimal_.width;
    if (maxwidth > implement::limit)
        maxwidth = implement::limit;
    impl_->maximal_.width = maxwidth;
    width(width());
}

void widget::maximal_height(unsigned int maxheight)
{
    if (maxheight < impl_->minimal_.height)
        maxheight = impl_->minimal_.height;
    if (maxheight > implement::limit)
        maxheight = implement::limit;
    impl_->maximal_.height = maxheight;
    height(height());
}

void widget::maximal_size(unsigned int maxwidth, unsigned int maxheight)
{
    maximal_width(maxwidth);
    maximal_height(maxheight);
}

void widget::maximal_size(const Size& _maximal_size)
{
    maximal_size(_maximal_size.width, _maximal_size.height);
}

void widget::absolute_width(unsigned int width)
{
    minimal_width(width);
    maximal_width(width);
}

void widget::absolute_height(unsigned int height)
{
    minimal_height(height);
    maximal_height(height);
}

void widget::absolute_size(unsigned int width, unsigned int height)
{
    minimal_size(width, height);
    maximal_size(width, height);
}

void widget::absolute_size(const Size& size)
{
    minimal_size(size);
    maximal_size(size);
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

void widget::capture(bool capture)
{
    if (capture)
        takisy::captured_widget__ = this;
    else
        takisy::captured_widget__ = nullptr;

    forefather()->window().capture(capture);
}

bool widget::exists_attribute(const std::string& name) const
{
    return impl_->attributes_.find(name) != impl_->attributes_.end();
}

bool widget::is_child(widget* widget) const
{
    return impl_->find_child(widget) != impl_->children_.end();
}

bool widget::is_senior(widget* widget) const
{
    class widget* w = father();

    while (w && w != widget)
        w = w->father();

    return w;
}

bool widget::is_junior(widget* widget) const
{
    for (const class widget* child : impl_->children_)
        if (child == widget || child->is_junior(widget))
            return true;

    return false;
}

bool widget::inside(int x, int y) const
{
    return impl_->rect_.inside(x, y);
}

bool widget::as_window(void)
{
    return as_window(true);
}

bool widget::as_window(bool enable_alpha_channel)
{
    if (father())
        return false;

    if (is_window())
        return true;

    cross_platform_window::Handle handle = nullptr;
#ifdef __os_win__
    DWORD style = WS_POPUP;
    if (visible())
        style |= WS_VISIBLE;

    DWORD exstyle = WS_EX_TOOLWINDOW;
    if (enable_alpha_channel)
        exstyle |= WS_EX_LAYERED;

    handle = CreateWindowEx(exstyle, takisy::class_name__,
                            "takisy::gui::cross_platform_window::winnt::widget",
                            style, x(), y(), width(), height(),
                            GetDesktopWindow(), nullptr,
                            GetModuleHandle(nullptr), nullptr);
#endif

    if (!handle)
        return false;
    else
        return as_window(handle);
}

bool widget::as_window(const cross_platform_window& cpw)
{
    return as_window(cpw.handle());
}

bool widget::as_window(cross_platform_window::Handle handle)
{
    if (father())
        return false;

    if (is_window())
        return true;

    if (handle)
    {
        if (takisy::all_windows__.find(handle) == takisy::all_windows__.end())
        {
            cross_platform_window window(handle);

            window.xy(xy() - window.client_offset());
            window.client_size(size());
            window.visible(visible());
            window.repaint();

            takisy::all_windows__[handle] = this;
        }
    }
    else
    {
        handle = takisy::handleFromLPWIDGET(this);
        if (handle)
        {
            takisy::all_windows__.erase(handle);

            cross_platform_window(handle).repaint();
        }
    }

    return true;
}

bool widget::is_window(void) const
{
    return !!takisy::handleFromLPWIDGET(this);
}

cross_platform_window widget::window(void) const
{
    return cross_platform_window(takisy::handleFromLPWIDGET(this));
}

bool widget::onAdding(widget*)                         { return true;  }
void widget::onAdd(widget*)                            {               }
bool widget::onRemoving(widget*)                       { return true;  }
void widget::onRemove(widget*)                         {               }

bool widget::onMoving(Point&)                          { return true;  }
void widget::onMove(void)                              {               }
bool widget::onSizing(Size&)                           { return true;  }
void widget::onSize(void)                              {               }
bool widget::onShowing(void)                           { return true;  }
void widget::onShown(void)                             {               }
bool widget::onHiding(void)                            { return true;  }
void widget::onHidden(void)                            {               }

bool widget::onChildMoving(widget*, Point&)            { return true;  }
void widget::onChildMove(widget*)                      {               }
bool widget::onChildSizing(widget*, Size&)             { return true;  }
void widget::onChildSize(widget*)                      {               }
bool widget::onChildShowing(widget*)                   { return true;  }
void widget::onChildShown(widget*)                     {               }
bool widget::onChildHiding(widget*)                    { return true;  }
void widget::onChildHidden(widget*)                    {               }

void widget::onPaint(graphics, Rect)                   {               }
void widget::onEndPaint(graphics, Rect)                {               }
bool widget::onFocus(bool)                             { return false; }
bool widget::onSetCursor(void)                         { return false; }
bool widget::onKeyDown(sys::VirtualKey)                { return false; }
bool widget::onKeyPress(unsigned int)                  { return false; }
bool widget::onKeyUp(sys::VirtualKey)                  { return false; }
bool widget::onMouseDown(sys::MouseButton, int, Point) { return false; }
bool widget::onClick(sys::MouseButton, int, Point)     { return false; }
bool widget::onMouseUp(sys::MouseButton, Point)        { return false; }
bool widget::onMouseMove(Point)                        { return false; }
bool widget::onMouseEnter(void)                        { return false; }
bool widget::onMouseLeave(void)                        { return false; }
bool widget::onMouseWheel(int, Point)                  { return false; }

void* widget::attribute(const std::string& name) const
{
    return exists_attribute(name) ? impl_->attributes_[name].data() : nullptr;
}

void widget::attribute(const std::string& name,
                       const void* value, unsigned int length)
{
    impl_->attributes_[name].resize(length);
    memcpy(impl_->attributes_[name].data(), value, length);
}

template <>
char* widget::attribute<char*>(const std::string& name) const
{
    void*  value = attribute(name);
    return value ? reinterpret_cast<char*>(value) : nullptr;
}

template <>
const char* widget::attribute<const char*>(const std::string& name) const
{
    return attribute<char*>(name);
}

template <>
std::string widget::attribute<std::string>(const std::string& name) const
{
    return attribute<char*>(name);
}

template <>
void widget::attribute<char*>(const std::string& name, char* const& value)
{
    attribute(name, value, strlen(value) + 1);
}

template <>
void widget::attribute<const char*>(
        const std::string& name, const char* const& value)
{
    attribute(name, value, strlen(value) + 1);
}

template <>
void widget::attribute<std::string>(
        const std::string& name, const std::string& value)
{
    attribute(name, value.data(), value.size());
}

#undef doIfAsWindow
