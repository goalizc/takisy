#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/label.h>
#include <takisy/gui/widget/picture.h>
#include <takisy/gui/widget/menu.h>

static constexpr const unsigned int kSpacing     = 8;
static constexpr const unsigned int kHalfSpacing = kSpacing / 2;

class menu::item::implement
{
    friend class item;
    friend class menu;

public:
    implement(void)
        : submenu_(nullptr)
    {
        icon_.upper_size(16, 16);
        icon_.scalable(true);
        icon_.show();
        text_.word_wrap(false);
        text_.show();
    }

private:
    picture icon_;
    label text_;
    class menu* submenu_;
};

menu::item::item(void)
    : impl_(new implement)
{
    margin(kHalfSpacing);
    spacing(kSpacing);

    add(&impl_->icon_);
    add(&impl_->text_);
}

menu::item::~item(void)
{
    delete impl_;
}

class menu* menu::item::submenu(void)
{
    return impl_->submenu_;
}

const class menu* menu::item::submenu(void) const
{
    return impl_->submenu_;
}

std::wstring menu::item::text(void) const
{
    return impl_->text_.text();
}

Size menu::item::optimal(OptimalPolicy policy) const
{
    Size icon = impl_->icon_.size(), text = impl_->text_.optimal();

    return {
        icon.width  + text.width  + kSpacing * 2 + 64,
       (icon.height > text.height ? icon.height : text.height) + kSpacing
    };
}

class menu::item& menu::item::submenu(class menu* submenu)
{
    impl_->submenu_ = submenu;
    repaint();

    return *this;
}

class menu::item& menu::item::text(const std::string& txt)
{
    text(txt, sys::default_codec());

    return *this;
}

class menu::item& menu::item::text(const std::string& txt, const std::string& codec)
{
    text(stralgo::decode(txt, codec));

    return *this;
}

class menu::item& menu::item::text(const std::wstring& txt)
{
    if (txt != impl_->text_.text())
    {
        impl_->text_.text(txt);
        upper_height(optimal().height);
        onTextChangedHandle();
    }

    return *this;
}

class menu::item& menu::item::icon(const char* uri)
{
    impl_->icon_.load_uri(uri);

    return *this;
}

class menu::item& menu::item::icon(const stream& istream)
{
    impl_->icon_.load_stream(istream);

    return *this;
}

class menu::item& menu::item::icon(const image& image)
{
    impl_->icon_.image(image);

    return *this;
}

void menu::item::onEndPaint(graphics graphics, Rect rect)
{
    if (impl_->submenu_)
    {
        double x = width() - kHalfSpacing;
        double y = (double)height() / 2;
        vertices vertices;

        vertices.append(x, y);
        vertices.append(x - 4, y - 4);
        vertices.append(x - 4, y + 4);

        graphics.fill_path(vertices, color_scheme().theme() * 128);
    }
}

class menu::implement
{
    friend class menu;

    struct separator : public widget
    {
        separator(void)
        {
            upper_height(kSpacing);
        }

    public:
        Size optimal(OptimalPolicy policy=opUnset) const override
        {
            return Size(width(), kSpacing);
        }

        void onPaint(graphics graphics, Rect rect) override
        {
            double y = (kSpacing + 1.0) / 2;

            graphics.draw_line(0, y, width(), y, color_scheme().border() * 64);
        }
    };

public:
    implement(void)
        : parent_(nullptr), current_(nullptr)
    {}

    ~implement(void)
    {
        for (class item* item : items_)
            delete item;
        for (class separator* separator : separators_)
            delete separator;
    }

private:
    class menu* parent_;
    class item* current_;
    std::vector<class item*> items_;
    std::vector<class separator*> separators_;
};

menu::menu(void)
    : impl_(new implement)
{
    margin(kHalfSpacing + 1);
}

menu::~menu(void)
{
    delete impl_;
}

unsigned int menu::count(void)
{
    return impl_->items_.size();
}

class menu::item* menu::item(int index)
{
    return impl_->items_[index];
}

const class menu::item* menu::item(int index) const
{
    return impl_->items_[index];
}

Size menu::optimal(OptimalPolicy policy) const
{
    std::vector<widget*> children = widget::children();
    Size optimal(0, 0);

    for (widget* child : children)
    {
        Size copt = child->optimal();
        if (optimal.width < copt.width)
            optimal.width = copt.width;
        optimal.height += copt.height;
    }

    optimal.width  += kSpacing + 2;
    optimal.height += kSpacing + 2;

    return optimal;
}

void menu::add_separator(void)
{
    impl_->separators_.emplace_back(new implement::separator);
    impl_->separators_.back()->show();

    widget::add(impl_->separators_.back());
}

class menu::item& menu::add(const std::string& text)
{
    return add(text, sys::default_codec());
}

class menu::item& menu::add(const std::string& text, const std::string& codec)
{
    return add(stralgo::decode(text, codec));
}

class menu::item& menu::add(const std::wstring& text)
{
    class item* item = new class item;
    item->onTextChanged([this](class item*) { size(optimal()); });
    item->text(text);
    item->show();

    widget::add(item);
    impl_->items_.push_back(item);

    return *item;
}

void menu::submenu(int index, class menu* submenu)
{
    if (static_cast<unsigned int>(index) < impl_->items_.size())
        impl_->items_[index]->submenu(submenu);
}

void menu::text(int index, const std::string& text)
{
    if (static_cast<unsigned int>(index) < impl_->items_.size())
        impl_->items_[index]->text(text);
}

void menu::text(int index, const std::string& text, const std::string& codec)
{
    if (static_cast<unsigned int>(index) < impl_->items_.size())
        impl_->items_[index]->text(text, codec);
}

void menu::text(int index, const std::wstring& text)
{
    if (static_cast<unsigned int>(index) < impl_->items_.size())
        impl_->items_[index]->text(text);
}

void menu::icon(int index, const char* uri)
{
    if (static_cast<unsigned int>(index) < impl_->items_.size())
        impl_->items_[index]->icon(uri);
}

void menu::icon(int index, const stream& istream)
{
    if (static_cast<unsigned int>(index) < impl_->items_.size())
        impl_->items_[index]->icon(istream);
}

void menu::icon(int index, const image& image)
{
    if (static_cast<unsigned int>(index) < impl_->items_.size())
        impl_->items_[index]->icon(image);
}

void menu::show(widget* widget, Point point)
{
    typedef window::WindowStyle ws;
    unsigned long wndstyle =
        ws::wsPopup | ws::wsEvanescent | ws::wsTopmost | ws::wsDropShadow;
    if (widget) {
        point += widget->screen_xy();
        wndstyle |= ws::wsNoActivate;
    }

    if (point.x + (int)width() > GetSystemMetrics(SM_CXSCREEN))
        point.x -= width();
    if (point.y + (int)height() > GetSystemMetrics(SM_CYSCREEN))
        point.y -= height();

    impl_->current_ = nullptr;

    widget::hide();
    xy(point);
    as_window(wndstyle);
    widget::show();

    if (!widget) {
        window().event().onActivate([this](void*, int active) {
            if (!active) close();
        });
    }
}

void menu::show(widget* widget, int x, int y)
{
    show(widget, Point(x, y));
}

void menu::close(void)
{
    class menu* menu = this;

    while (true) {
        menu->window().close();
        if (!menu->impl_->current_ || !menu->impl_->current_->submenu())
            break;
        menu = menu->impl_->current_->submenu();
    }
}

void menu::onAdd(widget*)
{
    size(optimal());
}

void menu::onRemove(widget*)
{
    size(optimal());
}

void menu::onPaint(graphics graphics, Rect)
{
    class color_scheme cs = color_scheme();
    rectf rect = client_rect();

    if (impl_->current_)
        graphics.fill_rectangle(impl_->current_->rect(), cs.selection() * 64);
    graphics.draw_rectangle(rect.inflate(-0.5), cs.border() * 128);
}

bool menu::onMouseDown(sys::Button button, int times, Point point)
{
    return true;
}

bool menu::onMouseUp(sys::Button button, int times, Point point)
{
    for (class item* item : impl_->items_) {
        if (item->inside(point.x, point.y) && !item->submenu()) {
            item->onClickHandle();
            onItemClickedHandle(*item);
            class menu* menu = this;
            do { menu->close(); } while ((menu = menu->impl_->parent_));
            break;
        }
    }

    return true;
}

bool menu::onMouseMove(Point point)
{
    for (class item* item : impl_->items_) {
        if (item->inside(point.x, point.y)) {
            if (item != impl_->current_) {
                if (impl_->current_) {
                    if (impl_->current_->submenu())
                        impl_->current_->submenu()->close();
                    repaint(impl_->current_->rect());
                }
                impl_->current_ = item;
                repaint(impl_->current_->rect());
                if (impl_->current_->submenu()) {
                    impl_->current_->submenu()->show(this,
                            impl_->current_->x() + impl_->current_->width(),
                            impl_->current_->y());
                    impl_->current_->submenu()->impl_->parent_ = this;
                }
            }
            return true;
        }
    }

    // if (impl_->current_) {
    //     repaint(impl_->current_->rect());
    //     impl_->current_ = nullptr;
    // }

    return true;
}
