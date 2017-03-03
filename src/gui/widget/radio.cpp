#include <takisy/core/sys.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/radio.h>

static constexpr int boxsize = 12;
static constexpr int spacing =  5;

class radio::implement
{
    friend class radio;
    friend class text_radio_group;

public:
    implement(widget* content, bool selected)
        : selected_(selected), content_(content)
    {}

private:
    bool selected_;
    widget* content_;
};

radio::radio(void)
    : radio(nullptr)
{}

radio::radio(widget* content)
    : radio(content, false)
{}

radio::radio(widget* content, bool selected)
    : impl_(new implement(content, selected))
{
    add(content);
}

radio::~radio(void)
{
    delete impl_;
}

widget* radio::content(void) const
{
    return impl_->content_;
}

bool radio::selected(void) const
{
    return impl_->selected_;
}

Size radio::optimal(OptimalPolicy policy) const
{
    Size optsize = impl_->content_->optimal(policy);

    optsize.width += boxsize + spacing;

    return optsize;
}

void radio::content(widget* content)
{
    remove(impl_->content_);
    add((impl_->content_ = content));

    onSize();
}

void radio::select(void)
{
    if (!impl_->selected_)
    {
        impl_->selected_ = true;

        for (widget* brother : father()->children())
        {
            radio* radio = dynamic_cast<class radio*>(brother);

            if (radio && radio != this && !radio->impl_->selected_)
            {
                radio->impl_->selected_ = false;
                radio->onDisselectedHandle();
                radio->repaint();
            }
        }

        onSelectedHandle();
        repaint();
    }
}

void radio::onSize(void)
{
    if (impl_->content_)
    {
        widget*& content = impl_->content_;

        content->x(boxsize + spacing);
        content->width(width() - content->x());
        content->height(content->optimal(opFixedWidth).height);
        content->y(int(height() - content->height()) / 2);
    }
}

void radio::onPaint(graphics graphics, Rect rect)
{
    color color  = color_scheme().theme();
    int   radius = boxsize / 2;
    int   y      = height() / 2;

    graphics.draw_circle(radius, y, radius - 0.5, color);
    if (selected())
        graphics.fill_circle(radius, y, radius - 3, color);
}

bool radio::onMouseDown(sys::Button button, int times, Point point)
{
    if (button == sys::btnLeft)
    {
        select();
        return true;
    }

    return false;
}

class text_radio::implement
{
    friend class text_radio;

public:
    implement(const wstring& text)
        : label_(text)
    {
        label_.show();
    }

private:
    label label_;
};

text_radio::text_radio(const string& text)
    : text_radio(text, sys::default_codec(), false)
{}

text_radio::text_radio(const string& text, bool selected)
    : text_radio(text, sys::default_codec(), selected)
{}

text_radio::text_radio(const string& text, const string& codec)
    : text_radio(stralgo::decode(text, codec))
{}

text_radio::text_radio(const string& text,
                       const string& codec, bool selected)
    : text_radio(stralgo::decode(text, codec), selected)
{}

text_radio::text_radio(const wstring& text)
    : text_radio(text, false)
{}

text_radio::text_radio(const wstring& text, bool selected)
    : radio(nullptr, selected), impl_(new implement(text))
{
    content(&impl_->label_);
}

text_radio::~text_radio(void)
{
    delete impl_;
}

label& text_radio::text(void)
{
    return impl_->label_;
}

const label& text_radio::text(void) const
{
    return impl_->label_;
}

class radio_group::implement
{
    friend class radio_group;

public:
    implement(const vector<widget*>& items)
        : items_(items), selected_(nullptr), margin_(0), spacing_(::spacing)
    {}

private:
    vector<widget*> items_;
           widget*  selected_;
    unsigned int    margin_;
    unsigned int    spacing_;
};

radio_group::radio_group(void)
    : radio_group(vector<widget*>())
{}

radio_group::radio_group(widget** items, unsigned int count)
    : radio_group(vector<widget*>(items, items + count))
{}

radio_group::radio_group(const vector<widget*>& items)
    : impl_(new implement(items))
{}

radio_group::radio_group(initializer_list<widget*> initlist)
    : radio_group(vector<widget*>(initlist.begin(), initlist.end()))
{}

radio_group::~radio_group(void)
{
    delete impl_;
}

unsigned int radio_group::count(void) const
{
    return impl_->items_.size();
}

vector<widget*> radio_group::items(void) const
{
    return impl_->items_;
}

widget* radio_group::item(unsigned int index) const
{
    if (index < impl_->items_.size())
        return impl_->items_[index];

    return nullptr;
}

widget* radio_group::selected(void) const
{
    return impl_->selected_;
}

int radio_group::selected_index(void) const
{
    for (unsigned int i = 0; i < impl_->items_.size(); ++i)
        if (impl_->selected_ == impl_->items_[i])
            return i;

    return -1;
}

Size radio_group::optimal(OptimalPolicy policy) const
{
    Size optsize(0, 0);

    for (const widget* item : impl_->items_)
    {
        Size item_optsize = item->optimal(policy);
        if (optsize.width < item_optsize.width)
            optsize.width = item_optsize.width;
        optsize.height += item_optsize.height;
    }

    if (!impl_->items_.empty())
    {
        optsize.width  += spacing() + boxsize;
        optsize.height += spacing() * (impl_->items_.size() - 1);
    }

    optsize.width  += margin() * 2;
    optsize.height += margin() * 2;

    return optsize;
}

unsigned int radio_group::margin(void) const
{
    return impl_->margin_;
}

unsigned int radio_group::spacing(void) const
{
    return impl_->spacing_;
}

void radio_group::append(widget* item)
{
    insert(impl_->items_.size(), item);
}

void radio_group::insert(unsigned int index, widget* item)
{
    if (index > impl_->items_.size() || !item)
        return;

    add(item);
    impl_->items_.insert(impl_->items_.begin() + index, item);
    onSize();
    repaint();
}

void radio_group::remove(widget* item)
{
    for (unsigned int i = 0; i < impl_->items_.size(); ++i)
    {
        if (impl_->items_[i] == item)
        {
            remove(i);
            break;
        }
    }
}

void radio_group::remove(unsigned int index)
{
    remove(index, index + 1);
}

void radio_group::remove(unsigned int begin, unsigned int end)
{
    if (begin >= impl_->items_.size())
        return;
    if (end >= impl_->items_.size())
        end = impl_->items_.size();
    if (begin == end)
        return;

    for (unsigned int i = begin; i < end; ++i)
        remove(impl_->items_[i]);
    impl_->items_.erase(impl_->items_.begin() + begin,
                        impl_->items_.begin() + end);

    onSize();
    repaint();
}

void radio_group::clear(void)
{
    remove(0, impl_->items_.size());
}

void radio_group::select(unsigned int index)
{
    if (index < impl_->items_.size())
        select(impl_->items_[index]);
}

void radio_group::select(widget* selected)
{
    if (selected == impl_->selected_)
        return;

    for (widget* item : impl_->items_)
    {
        if (item != selected)
            continue;

        impl_->selected_ = selected;
        onSelectionChangedHandle();
        repaint();

        break;
    }
}

void radio_group::margin(unsigned int margin)
{
    if (margin != impl_->margin_)
    {
        impl_->margin_ = margin;
        onSize();
        repaint();
    }
}

void radio_group::spacing(unsigned int spacing)
{
    if (spacing != impl_->spacing_)
    {
        impl_->spacing_ = spacing;
        onSize();
        repaint(0, 0, margin() + boxsize, height());
    }
}

void radio_group::onSize(void)
{
    int x = margin() + boxsize + spacing(), y = margin();
    int item_width = width() - x - margin();
    int item_height;

    for (widget* item : impl_->items_)
    {
        item->x(x);
        item->width(item_width);
        item->height(item->optimal(opFixedWidth).height);

        item_height = item->height();
        if (item_height < boxsize)
            item->y(y + (boxsize - item_height) / 2),
            item_height = boxsize;
        else
            item->y(y);

        y += item_height + spacing();
    }
}

void radio_group::onPaint(graphics graphics, Rect rect)
{
    color color  = color_scheme().theme();
    int x = margin() + boxsize / 2, radius = boxsize / 2;
    int y = margin();

    for (widget* item : impl_->items_)
    {
        double height = boxsize > item->height() ? boxsize : item->height();
        double cty    = y + height / 2;

        graphics.draw_circle(x, cty, radius - 0.5, color);
        if (item == impl_->selected_)
            graphics.fill_circle(x, cty, radius - 3, color);

        y += height + spacing();
    }
}

bool radio_group::onMouseDown(sys::Button button, int times, Point point)
{
    if (button == sys::btnLeft)
    {
        int y = margin();

        for (widget* item : impl_->items_)
        {
            if (point.y < y)
                break;

            int height = boxsize > item->height() ? boxsize : item->height();

            if (y <= point.y && point.y < y + height)
            {
                select(item);
                return true;
            }

            y += height + spacing();
        }
    }

    return false;
}

text_radio_group::text_radio_group(void)
    : text_radio_group(vector<wstring>())
{}

text_radio_group::text_radio_group(const char** items, unsigned int count)
    : text_radio_group(items, count, sys::default_codec())
{}

text_radio_group::text_radio_group(const char** items, unsigned int count,
                         const char* codec)
    : text_radio_group(vector<const char*>(items, items + count), codec)
{}

text_radio_group::text_radio_group(const wchar_t** items, unsigned int count)
    : text_radio_group(vector<const wchar_t*>(items, items + count))
{}

text_radio_group::text_radio_group(const string* items, unsigned int count)
    : text_radio_group(items, count, sys::default_codec())
{}

text_radio_group::text_radio_group(const string* items, unsigned int count,
                         const string& codec)
    : text_radio_group(vector<string>(items, items + count), codec)
{}

text_radio_group::text_radio_group(const wstring* items, unsigned int count)
    : text_radio_group(vector<wstring>(items, items + count))
{}

text_radio_group::text_radio_group(const vector<const char*>& items)
    : text_radio_group(items, sys::default_codec())
{}

text_radio_group::text_radio_group(const vector<const char*>& items,
                                   const char* codec)
    : text_radio_group(vector<string>(items.begin(), items.end()), codec)
{}

text_radio_group::text_radio_group(const vector<const wchar_t*>& items)
    : text_radio_group(vector<wstring>(items.begin(), items.end()))
{}

text_radio_group::text_radio_group(const vector<string>& items)
    : text_radio_group(items, sys::default_codec())
{}

text_radio_group::text_radio_group(const vector<string>& items,
                                   const string& codec)
    : text_radio_group([&items, &codec](void) -> vector<wstring>
    {
        vector<wstring> result;

        for (const string& item : items)
            result.push_back(stralgo::decode(item, codec));

        return result;
    }())
{}

text_radio_group::text_radio_group(const vector<wstring>& items)
{
    for (const wstring& item : items)
        append(item);
}

text_radio_group::text_radio_group(initializer_list<const char*> list)
    : text_radio_group(list, sys::default_codec())
{}

text_radio_group::text_radio_group(initializer_list<const char*> list,
                                   const char* codec)
    : text_radio_group(vector<string>(list.begin(), list.end()), codec)
{}

text_radio_group::text_radio_group(initializer_list<const wchar_t*> list)
    : text_radio_group(vector<wstring>(list.begin(), list.end()))
{}

text_radio_group::text_radio_group(initializer_list<string> list)
    : text_radio_group(list, sys::default_codec())
{}

text_radio_group::text_radio_group(initializer_list<string> list,
                                   const string& codec)
    : text_radio_group(vector<string>(list), codec)
{}

text_radio_group::text_radio_group(initializer_list<wstring> list)
    : text_radio_group(vector<wstring>(list))
{}

text_radio_group::~text_radio_group(void)
{}

void text_radio_group::append(const string& item)
{
    append(item, sys::default_codec());
}

void text_radio_group::append(const string& item, const string& codec)
{
    append(stralgo::decode(item, codec));
}

void text_radio_group::append(const wstring& item)
{
    insert(count(), item);
}

void text_radio_group::insert(unsigned int index, const string& item)
{
    insert(index, item, sys::default_codec());
}

void text_radio_group::insert(unsigned int index, const string& item,
                              const string& codec)
{
    insert(index, stralgo::decode(item, codec));
}

void text_radio_group::insert(unsigned int index, const wstring& item)
{
    class label* label = new class label(item);
    label->show();
    insert(index, label);
}

void text_radio_group::onRemove(widget* widget)
{
    delete widget;
}

label& text_radio_group::text(unsigned int index)
{
    return dynamic_cast<label&>(*item(index));
}

const label& text_radio_group::text(unsigned int index) const
{
    return dynamic_cast<const label&>(*item(index));
}
