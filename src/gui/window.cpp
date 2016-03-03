#include <stdexcept>
#include <takisy/core/sys.h>
#include <takisy/gui/widget/layout.h>
#include <takisy/gui/widget/button.h>
#include <takisy/gui/widget/label.h>
#include <takisy/gui/window.h>
#include "basic/window.h"

struct SysButton : public widget
{
    color color;

public:
    void onPaint(graphics graphics, Rect rect) override
    {
        graphics.fill_circle(rect.center(), rect.width() >> 2, color);
        graphics.draw_circle(rect.center(), rect.width() >> 2, color::white());
    }
};

struct TitleBar : public horizontal_layout
{
    typedef label Caption;

    Caption caption;
    SysButton min, max, close;

public:
    TitleBar(void)
    {
        caption.alignment(aCenter);
        caption.show();

        min.color = color::yellow();
        min.attribute("cmdid", "minimal");
        min.show();

        max.color = color::green();
        max.attribute("cmdid", "maximal");
        max.show();

        close.color = color::red();
        close.attribute("cmdid", "close");
        close.show();

        absolute_height(20);
        alignment(aLeft);
        add(&caption);
        add(&min);
        add(&max);
        add(&close);
        show();
    }

public:
    void onSize(Size size) override
    {
        min.absolute_size(size.height, size.height);
        max.absolute_size(size.height, size.height);
        close.absolute_size(size.height, size.height);
    }

    void onPaint(graphics graphics, Rect rect) override
    {
        graphics.clear(sys::default_interface_color());
    }
};

class Frame : public vertical_layout
{
public:
    Frame(window::Layout layout)
        : vertical_layout()
    {
        switch (layout)
        {
        case window::lNone:       client_ = new widget;            break;
        case window::lHorizontal: client_ = new horizontal_layout; break;
        case window::lVertical:   client_ = new vertical_layout;   break;
        }
        client_->show();

        add(&title_bar_);
        add(client_);
        show();

        as_window(false);
        doIfAsWindow(this, showInTaskbar, true);
    }

    ~Frame(void)
    {
        hide();
        delete client_;
    }

public:
    label& caption(void)
    {
        return title_bar_.caption;
    }

    widget& client(void)
    {
        return *client_;
    }

public:
    void onPaint(graphics graphics, Rect rect) override
    {
        graphics.clear(color::white());
    }

    void onEndPaint(graphics graphics, Rect rect) override
    {
        graphics.draw_rectangle(client_rect().expand(-1), color::gray());
    }

    bool onCommand(widget* widget, const char* cmdid) override
    {
        if (strcmp(cmdid, "close") == 0)
            doIfAsWindow(this, destroy);

        return true;
    }

private:
    TitleBar title_bar_;
    widget*  client_;
};

class window::implement
{
    friend class window;

public:
    implement(window::Layout layout)
        : frame_(layout)
    {}

private:
    Frame frame_;
};

window::window(void)
    : window(lNone)
{}

window::window(const char* caption)
    : window(lNone, caption)
{}

window::window(const char* caption, unsigned int width, unsigned int height)
    : window(lNone, caption, width, height)
{}

window::window(const char* caption,
               int x, int y, unsigned int width, unsigned int height)
    : window(lNone, caption, x, y, width, height)
{}

window::window(const char* caption, const Size& size)
    : window(lNone, caption, size)
{}

window::window(const char* caption, const Point& xy, const Size& size)
    : window(lNone, caption, xy, size)
{}

window::window(const char* caption,
               const Point& left_top, const Point& right_bottom)
    : window(lNone, caption, left_top, right_bottom)
{}

window::window(const char* caption, const Rect& rect)
    : window(lNone, caption, rect)
{}

window::window(Layout layout)
    : window(layout, "unname")
{}

window::window(Layout layout, const char* caption)
    : window(layout, caption, sys::screen_width() / 3, sys::screen_height() / 3)
{}

window::window(Layout layout, const char* caption,
               unsigned int width, unsigned int height)
    : window(layout, caption, 0, 0, width, height)
{}

window::window(Layout layout, const char* caption,
               int x, int y, unsigned int width, unsigned int height)
    : window(layout, caption, Rect(x, y, x + width, y + height))
{}

window::window(Layout layout, const char* caption, const Size& size)
    : window(layout, caption, Rect(0, 0, size.width, size.height))
{}

window::window(Layout layout, const char* caption,
               const Point& xy, const Size& size)
    : window(layout, caption, Rect(xy, size))
{}

window::window(Layout layout, const char* caption,
               const Point& left_top, const Point& right_bottom)
    : window(layout, caption, Rect(left_top, right_bottom))
{}

window::window(Layout layout, const char* caption, const Rect& rect)
    : impl_(new implement(layout))
{
    impl_->frame_.caption().text(caption);
    impl_->frame_.rect(rect);
}

window::~window(void)
{
    delete impl_;
}

window::window(const window& window)
{
    operator=(window);
}

void window::operator=(const window&)
{
    throw std::logic_error("class window is noncopyable class.");
}

const char* window::caption(void) const
{
    return "unkown";
}

int window::x(void) const
{
    return impl_->frame_.x();
}

int window::y(void) const
{
    return impl_->frame_.y();
}

window::Point window::xy(void) const
{
    return impl_->frame_.xy();
}

unsigned int window::width(void) const
{
    return impl_->frame_.width();
}

unsigned int window::height(void) const
{
    return impl_->frame_.height();
}

window::Size window::size(void) const
{
    return impl_->frame_.size();
}

window::Rect window::rect(void) const
{
    return impl_->frame_.rect();
}

bool window::visible(void) const
{
    return impl_->frame_.visible();
}

unsigned int window::minimal_width(void) const
{
    return impl_->frame_.minimal_width();
}

unsigned int window::minimal_height(void) const
{
    return impl_->frame_.minimal_height();
}

window::Size window::minimal_size(void) const
{
    return impl_->frame_.minimal_size();
}

unsigned int window::maximal_width(void) const
{
    return impl_->frame_.maximal_width();
}

unsigned int window::maximal_height(void) const
{
    return impl_->frame_.maximal_height();
}

window::Size window::maximal_size(void) const
{
    return impl_->frame_.maximal_size();
}

bool window::add(widget* widget)
{
    return impl_->frame_.client().add(widget);
}

void window::remove(widget* widget)
{
    impl_->frame_.client().remove(widget);
}

void window::caption(const char* caption)
{
    impl_->frame_.caption().text(caption);
}

void window::x(int x)
{
    impl_->frame_.x(x);
}

void window::y(int y)
{
    impl_->frame_.y(y);
}

void window::xy(int x, int y)
{
    impl_->frame_.xy(x, y);
}

void window::xy(Point xy)
{
    impl_->frame_.xy(xy);
}

void window::width(unsigned int width)
{
    impl_->frame_.width(width);
}

void window::height(unsigned int height)
{
    impl_->frame_.height(height);
}

void window::size(unsigned int width, unsigned int height)
{
    impl_->frame_.size(width, height);
}

void window::size(Size size)
{
    impl_->frame_.size(size);
}

void window::rect(int x, int y, unsigned int width, unsigned int height)
{
    impl_->frame_.rect(x, y, width, height);
}

void window::rect(const Point& xy, const Size& size)
{
    impl_->frame_.rect(xy, size);
}

void window::rect(const Rect& rect)
{
    impl_->frame_.rect(rect);
}

void window::visible(bool visible)
{
    impl_->frame_.visible(visible);
}

void window::show(void)
{
    impl_->frame_.show();
}

void window::show_normal(void)
{

}

void window::show_minimal(void)
{

}

void window::show_maximal(void)
{

}

void window::hide(void)
{
    impl_->frame_.hide();
}

void window::minimal_width(unsigned int minimal_width)
{
    impl_->frame_.minimal_width(minimal_width);
}

void window::minimal_height(unsigned int minimal_height)
{
    impl_->frame_.minimal_height(minimal_height);
}

void window::minimal_size(unsigned int minimal_width,
                          unsigned int minimal_height)
{
    impl_->frame_.minimal_size(minimal_width, minimal_height);
}

void window::minimal_size(const Size& minimal_size)
{
    impl_->frame_.minimal_size(minimal_size);
}

void window::maximal_width(unsigned int maximal_width)
{
    impl_->frame_.maximal_width(maximal_width);
}

void window::maximal_height(unsigned int maximal_height)
{
    impl_->frame_.maximal_height(maximal_height);
}

void window::maximal_size(unsigned int maximal_width,
                          unsigned int maximal_height)
{
    impl_->frame_.maximal_size(maximal_width, maximal_height);
}

void window::maximal_size(const Size& maximal_size)
{
    impl_->frame_.maximal_size(maximal_size);
}

void window::absolute_width(unsigned int width)
{
    impl_->frame_.absolute_width(width);
}

void window::absolute_height(unsigned int height)
{
    impl_->frame_.absolute_height(height);
}

void window::absolute_size(unsigned int width, unsigned int height)
{
    impl_->frame_.absolute_size(width, height);
}

void window::absolute_size(const Size& size)
{
    impl_->frame_.absolute_size(size);
}

