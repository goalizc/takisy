#include <map>
#include <list>
#include <string>
#include <algorithm>
#include <takisy/oldgui/basic/exception.h>
#include <takisy/oldgui/window.h>
#include <takisy/oldgui/widget/widget.h>

namespace global
{
    extern std::list<Window*>   windowWithWidgets;
    extern std::map<HWND, Widget*> capturedWidget;
}

class Widget::Implement
{
    friend class Widget;

public:
    Implement(void)
        : visible_(true), id_(id()), parent_(nullptr)
    {}

private:
    static inline unsigned int id(void)
    {
        static unsigned int id_generator = 0;

        return id_generator++;
    }

private:
    Rect rect_;
    bool visible_;
    const unsigned int id_;
    Widget* parent_;
    std::vector<Widget*> children_;
    std::map<std::string, std::string> attributes_;
};

Widget::Widget(void)
    : impl_(new Implement)
{
    create(0, 0, 200, 150);
}

Widget::Widget(int x, int y, unsigned int width, unsigned int height,
               Widget* parent, bool visible)
    : impl_(new Implement)
{
    create(x, y, width, height, parent, visible);
}

Widget::~Widget(void)
{
    onDestroy();

    for (Widget* child : impl_->children_)
        remove(child);

    if (impl_->parent_)
        impl_->parent_->remove(this);

    delete impl_;
}

void Widget::create(int x, int y, unsigned int width, unsigned int height,
                    Widget* _parent, bool _visible)
{
    rect(x, y, width, height);

    parent(_parent);
    visible(_visible);

    onCreate();
}

void Widget::add(Widget* widget)
{
    if (!widget || widget == this)
        return;

    auto finded_widget = std::find(impl_->children_.begin(),
                                   impl_->children_.end(), widget);
    if (finded_widget == impl_->children_.end())
    {
        if (widget->impl_->parent_)
            widget->impl_->parent_->remove(widget);

        impl_->children_.push_back(widget);
        widget->impl_->parent_ = this;
        repaint(widget->rect());
    }
}

bool Widget::exists(Widget* widget) const
{
    for (Widget* child : impl_->children_)
        if (child == widget || child->exists(widget))
            return true;

    return false;
}

void Widget::remove(Widget* widget)
{
    auto finded_widget = std::find(impl_->children_.begin(),
                                   impl_->children_.end(), widget);
    if (finded_widget != impl_->children_.end())
    {
        if (widget->impl_->parent_ != this)
            throw "The widget's parent is not me.!!!";

        impl_->children_.erase(finded_widget);
        widget->impl_->parent_ = nullptr;
        repaint(widget->rect());
    }
}

Widget* Widget::parent(void) const
{
    return impl_->parent_;
}

void Widget::parent(Widget* parent)
{
    if (impl_->parent_)
        impl_->parent_->remove(this);

    if (parent)
        parent->add(this);
}

Widget* Widget::forefather(void) const
{
    if (!impl_->parent_)
        return nullptr;

    Widget* forefather = impl_->parent_;
    while (forefather->impl_->parent_)
        forefather = forefather->impl_->parent_;

    return forefather;
}

std::vector<Widget*> Widget::children(void) const
{
    return impl_->children_;
}

Widget* Widget::hit_test(int x, int y)
{
    if (!inside(x, y))
        return nullptr;

    std::vector<Widget*>::reverse_iterator iterator = impl_->children_.rbegin();
    for ( ; iterator != impl_->children_.rend(); ++iterator)
    {
        Widget* child = *iterator;
        if (!child->visible())
            continue;
        Widget* hitted_widget = child->hit_test(x - child->x(), y - child->y());
        if (hitted_widget)
            return hitted_widget;
    }

    return this;
}

Widget* Widget::hit_test(const Point& point)
{
    return hit_test(point.x, point.y);
}

void Widget::attribute(const char* attribute, const char* value)
{
    impl_->attributes_[attribute] = value;
}

const char* Widget::attribute(const char* attribute) const
{
    if (impl_->attributes_.find(attribute) == impl_->attributes_.end())
        return nullptr;

    return impl_->attributes_[attribute].c_str();
}

unsigned int Widget::id(void) const
{
    return impl_->id_;
}

Rect Widget::rect(void) const
{
    return impl_->rect_;
}

Point Widget::xy(void) const
{
    return rect().left_top();
}

int Widget::x(void) const
{
    return xy().x;
}

int Widget::y(void) const
{
    return xy().y;
}

Rect Widget::global_rect(void) const
{
    return Rect(global_xy(), size());
}

Point Widget::global_xy(void) const
{
    const Widget* widget = this;
    Point xy;

    do
    {
        xy    += widget->xy();
        widget = widget->parent();
    } while (widget);

    return xy;
}

int Widget::global_x(void) const
{
    return global_xy().x;
}

int Widget::global_y(void) const
{
    return global_xy().y;
}

Size Widget::size(void) const
{
    return rect().size();
}

unsigned int Widget::width(void) const
{
    return size().width;
}

unsigned int Widget::height(void) const
{
    return size().height;
}

bool Widget::visible(void) const
{
    return impl_->visible_;
}

void Widget::x(int x)
{
    xy(x, y());
}

void Widget::y(int y)
{
    xy(x(), y);
}

void Widget::xy(int x, int y)
{
    rect(x, y, width(), height());
}

void Widget::xy(const Point& _xy)
{
    xy(_xy.x, _xy.y);
}

void Widget::width(unsigned int width)
{
    size(width, height());
}

void Widget::height(unsigned int height)
{
    size(width(), height);
}

void Widget::size(unsigned int width, unsigned int height)
{
    rect(x(), y(), width, height);
}

void Widget::size(const Size& _size)
{
    size(_size.width, _size.height);
}

void Widget::rect(int x, int y, unsigned int width, unsigned int height)
{
    rect(Point(x, y), Size(width, height));
}

void Widget::rect(const Point& xy, const Size& size)
{
    Rect old_rect = impl_->rect_;
    bool need_repaint = false;

    if (xy != impl_->rect_.left_top())
    {
        Point xy2 = xy;

        onMove(xy2);

        if (xy2 != impl_->rect_.left_top())
        {
            need_repaint = true;
            impl_->rect_ = impl_->rect_.move(xy2);
        }
    }

    if (size != impl_->rect_.size())
    {
        Size new_size = size;

        onSize(new_size);

        if (new_size != impl_->rect_.size())
        {
            need_repaint = true;
            impl_->rect_.size(new_size);
        }
    }

    if (need_repaint)
        repaint(impl_->rect_.unit(old_rect).offset(-impl_->rect_.left_top()));
}

void Widget::rect(const Rect& _rect)
{
    rect(_rect.left_top(), _rect.size());
}

void Widget::visible(bool visible)
{
    if (impl_->visible_ != visible)
    {
        if ((impl_->visible_ = visible))
            onShown();
        else
            onHidden();

        repaint();
    }
}

void Widget::show(void)
{
    visible(true);
}

void Widget::hide(void)
{
    visible(false);
}

void Widget::repaint(void)
{
    repaint(0, 0, width(), height());
}

void Widget::repaint(int x, int y, unsigned int width, unsigned int height)
{
    repaint(Rect(x, y, x + width, y + height));
}

void Widget::repaint(Rect rect)
{
    if (rect.empty())
        return;

    for (Window* window : global::windowWithWidgets)
        if (window->existsWidget(this))
            window->repaint(rect.offset(global_xy()));
}

void Widget::set_capture(void)
{
    Window* window = global::windowWithWidgets.front();
    global::capturedWidget[window->hwnd()] = this;
    window->setCapture();
}

void Widget::release_capture(void)
{
    Window* window = global::windowWithWidgets.front();
    window->releaseCapture();
    global::capturedWidget[window->hwnd()] = nullptr;
}

bool Widget::inside(int x, int y)
{
    return static_cast<unsigned int>(x) < width()
        && static_cast<unsigned int>(y) < height();
}

void Widget::onCreate(void)    {}
void Widget::onDestroy(void)   {}
void Widget::onFocus(bool)     {}
void Widget::onShown(void)     {}
void Widget::onHidden(void)    {}
void Widget::onMove(Point&)    {}
void Widget::onSize(Size&)     {}
void Widget::onPaint(Graphics) {}
bool Widget::onKeyDown    (VirtualKey,   KeyState)        { return false; }
bool Widget::onKeyPress   (unsigned int, KeyState)        { return false; }
bool Widget::onKeyUp      (VirtualKey,   KeyState)        { return false; }
bool Widget::onMouseDown  (MouseButton,  KeyState, Point) { return false; }
bool Widget::onClick      (MouseButton,  KeyState, int, Point)
                                                          { return false; }
bool Widget::onMouseUp    (MouseButton,  KeyState, Point) { return false; }
bool Widget::onMouseMove  (KeyState,               Point) { return false; }
bool Widget::onMouseEnter (KeyState,               Point) { return false; }
bool Widget::onMouseHover (KeyState,               Point) { return false; }
bool Widget::onMouseLeave (KeyState,               Point) { return false; }
bool Widget::onMouseWheel (KeyState, int,          Point) { return false; }
void Widget::onSetCursor(void) { SetCursor(LoadCursor(nullptr, IDC_ARROW)); }
