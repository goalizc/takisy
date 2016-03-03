#include <map>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <takisy/core/algorithm.h>
#include <takisy/core/stretchy_buffer.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/widget.h>
#include <takisy/gui/widget/layout.h>
#include "../basic/window.h"
#include "impl/layout.hpp"

#define readapt(widget)                                            \
    ({ class layout* layout = dynamic_cast<class layout*>(widget); \
       if (layout) layout->impl_->readapt(); })

class widget::implement
{
    friend class Window;
    friend class widget;

public:
    implement(void)
        : father_(nullptr), pure_(false), visible_(false), rect_(0, 0, 0, 0)
        , minimal_(0, 0), maximal_(~0u, ~0u), id_(id())
    {}

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
    bool pure_, visible_;
    widget::Rect rect_;
    widget::Size minimal_, maximal_;
    const unsigned int id_;
    std::map<std::string, stretchy_buffer<unsigned char>> attributes_;
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
    doIfAsWindow(this, destroy);
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

int widget::x(void) const
{
    return impl_->rect_.left;
}

int widget::y(void) const
{
    return impl_->rect_.top;
}

widget::Point widget::xy(void) const
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

widget::Size widget::size(void) const
{
    return impl_->rect_.size();
}

widget::Rect widget::rect(void) const
{
    return impl_->rect_;
}

bool widget::visible(void) const
{
    return impl_->visible_;
}

widget::Rect widget::client_rect(void) const
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

widget::Point widget::window_xy(void) const
{
    return screen_xy() - forefather()->xy();
}

widget::Rect widget::window_rect(void) const
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

widget::Point widget::screen_xy(void) const
{
    const widget* widget = this;
    Point point = xy();

    while ((widget = widget->father()))
        point += widget->xy();

    return point;
}

widget::Rect widget::screen_rect(void) const
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

widget::Size widget::minimal_size(void) const
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

widget::Size widget::maximal_size(void) const
{
    return impl_->maximal_;
}

widget::Size widget::optimal_size(void) const
{
    return size();
}

bool widget::pure(void)
{
    return impl_->pure_;
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
    if (!widget || widget == this || is_senior(widget) || Window::find(widget))
        return false;

    if (widget->father())
        widget->father()->remove(widget);

    impl_->children_.push_back(widget);
    widget->impl_->father_ = this;

    if (widget->visible())
        readapt(this);

    repaint();

    return true;
}

void widget::remove(widget* widget)
{
    typedef std::vector<class widget*>::const_iterator widget_iterator;
    widget_iterator child = impl_->find_child(widget);
    if (child == impl_->children_.end())
        return;

    impl_->children_.erase(child);
    widget->impl_->father_ = nullptr;

    if (widget->visible())
        readapt(this);

    repaint();
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
    if (_xy == xy())
        return;

    impl_->rect_ = impl_->rect_.move(_xy);
    doIfAsWindow(this, xy, _xy.x, _xy.y);
    onMove(_xy);

    if (visible())
        readapt(father());

    repaint();
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
    _size.width  = algorithm::clamp(_size.width,
            impl_->minimal_.width, impl_->maximal_.width);
    _size.height = algorithm::clamp(_size.height,
            impl_->minimal_.height, impl_->maximal_.height);

    if (_size == size())
        return;

    impl_->rect_.size(_size);
    doIfAsWindow(this, size, _size.width, _size.height);
    onSize(_size);

    if (visible())
        readapt(father());
    readapt(this);

    repaint();
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

    impl_->visible_ = visible;
    doIfAsWindow(this, visible, visible);

    if (visible)
        onShown();
    else
        onHidden();

    readapt(father());
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

void widget::minimal_width(unsigned int minimal_width)
{
    impl_->minimal_.width = minimal_width;
    width(width());
}

void widget::minimal_height(unsigned int minimal_height)
{
    impl_->minimal_.height = minimal_height;
    height(height());
}

void widget::minimal_size(unsigned int minwidth, unsigned int minheight)
{
    impl_->minimal_.width  = minwidth;
    impl_->minimal_.height = minheight;
    size(width(), height());
}

void widget::minimal_size(const Size& minsize)
{
    minimal_size(minsize.width, minsize.height);
}

void widget::maximal_width(unsigned int maximal_width)
{
    impl_->maximal_.width = maximal_width;
    width(width());
}

void widget::maximal_height(unsigned int maximal_height)
{
    impl_->maximal_.height = maximal_height;
    height(height());
}

void widget::maximal_size(unsigned int maxwidth, unsigned int maxheight)
{
    impl_->maximal_.width  = maxwidth;
    impl_->maximal_.height = maxheight;
    size(width(), height());
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

void widget::pure(bool pure)
{
    impl_->pure_ = pure;
}

void widget::sendcmd(const char* cmdid)
{
    class widget* widget = this;

    while (widget)
        if (widget->onCommand(this, cmdid)
            || widget->attribute<bool>("intercept.onCommand"))
            break;
        else
            widget = widget->father();
}

void widget::repaint(void)
{
    repaint(client_rect());
}

void widget::repaint(const Rect& rect)
{
    if (!visible() || rect.empty())
        return;

    widget::Rect paint_rect = rect;
    widget* widget = this;

    while (widget->father())
    {
        paint_rect = paint_rect.offset(widget->xy());
        widget     = widget->father();
        paint_rect = paint_rect.intersect(widget->client_rect());

        if (!widget->visible() || paint_rect.empty())
            return;
    }

    doIfAsWindow(forefather(), repaint, paint_rect);
}

void widget::capture(bool capture)
{
    Window::capture(capture ? this : nullptr);
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
    bool ret = Window::create(this, enable_alpha_channel);

    if (ret)
        doIfAsWindow(this, showInTaskbar, false);

    return ret;
}

bool widget::as_topmost_window(void)
{
    return as_topmost_window(true);
}

bool widget::as_topmost_window(bool enable_alpha_channel)
{
    bool ret = as_window(enable_alpha_channel);

    if (ret)
        doIfAsWindow(this, topmost, true);

    return ret;
}

void widget::onShown(void) {}
void widget::onHidden(void) {}
void widget::onMove(Point) {}
void widget::onSize(Size) {}
void widget::onPaint(graphics, Rect) {}
void widget::onEndPaint(graphics, Rect) {}
bool widget::onFocus(bool) { return false; }
bool widget::onSetCursor(void) { return false; }
bool widget::onKeyDown(sys::VirtualKey) { return false; }
bool widget::onKeyPress(unsigned int) { return false; }
bool widget::onKeyUp(sys::VirtualKey) { return false; }
bool widget::onMouseDown(sys::MouseButton, int, Point) { return false; }
bool widget::onClick(sys::MouseButton, int, Point) { return false; }
bool widget::onCommand(widget*, const char*) { return false; }
bool widget::onMouseUp(sys::MouseButton, Point) { return false; }
bool widget::onMouseMove(Point) { return false; }
bool widget::onMouseEnter(void) { return false; }
bool widget::onMouseLeave(void) { return false; }
bool widget::onMouseWheel(int, Point) { return false; }

void* widget::attribute(const std::string& name) const
{
    if (exists_attribute(name))
        return impl_->attributes_[name].data();

    return nullptr;
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
    void* value = attribute(name);

    if (value)
        return reinterpret_cast<char*>(value);

    return nullptr;
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
