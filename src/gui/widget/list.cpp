#include <iterator>
#include <algorithm>
#include <takisy/core/sys.h>
#include <takisy/core/codec.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/list.h>
#include "editbox.h"

class list::implement
{
    friend class list;

    struct brush
    {
        brush_sptr text, background, selection;
    };

    struct item
    {
        text text;
        bool editable;
        std::shared_ptr<brush> brush;

    public:
        item(const std::wstring& _text)
            : text(_text), editable(false), brush(nullptr)
        {
            text.margin(2);
            text.word_wrap(false);
        }
    };

public:
    implement(list& self)
        : self(self)
        , selmode_(smSingleSelection), edit_trigger_(etNoEditTrigger)
        , selbegin_(0), current_(0)
    {}

public:
    unsigned int accumulate(unsigned int offset)
    {
        return std::accumulate(items_.begin(), items_.begin() + offset, 0,
            [](unsigned int height, item& item)
            {
                return height + item.text.world().height;
            });
    }

    void vupdate(void)
    {
        self.vertical_scroll()
            .max(accumulate(items_.size()) + self.horizontal_scroll().height());
        self.repaint();
    }

    void hupdate(void)
    {
        auto iter = std::max_element(items_.begin(), items_.end(),
            [](const item& a, const item& b)
            {
                return a.text.world().width < b.text.world().width;
            });

        if (iter == items_.end())
            self.horizontal_scroll().max(0);
        else
            self.horizontal_scroll()
                .max(iter->text.world().width + self.vertical_scroll().width());

        self.repaint();
    }

    void update(void)
    {
        vupdate();
        hupdate();
    }

    void adjust_hvscroll(const Size& oldworld, const Size& newworld)
    {
        class vertical_scroll&   vscroll = self.vertical_scroll();
        class horizontal_scroll& hscroll = self.horizontal_scroll();
        unsigned int width = hscroll.max() - vscroll.width();

        vscroll.max(vscroll.max() + newworld.height - oldworld.height);
        if (newworld.width > width)
            hscroll.max(newworld.width + vscroll.width());
        else
        if (newworld.width < width && oldworld.width == hscroll.max())
            return hupdate();
    }

    Rect item_rect(unsigned int index)
    {
        Rect rect;

        if (index < items_.size())
        {
            unsigned int width = self.horizontal_scroll().max();
            if (width < self.width())
                width = self.width();

            rect =
            rect.offset(-self.horizontal_scroll().valued(),
                        accumulate(index) - self.vertical_scroll().valued());
            rect.width(width);
            rect.height(items_[index].text.world().height);
        }

        return rect;
    }

private:
    list& self;
    brush brush_;
    std::vector<item> items_;
    SelectionMode selmode_;
    unsigned int edit_trigger_;
    unsigned int selbegin_, current_;
    std::set<unsigned int> selecteds_;
};

list::list(void)
    : list(std::vector<std::wstring>())
{}

list::list(const std::vector<std::string>& items)
    : list(items, sys::default_codec())
{}

list::list(const std::vector<std::string>& items, const std::string& codec)
    : list([&items, &codec](void) -> std::vector<std::wstring>
    {
        std::vector<std::wstring> result;

        for (const std::string& item : items)
            result.push_back(stralgo::decode(item, codec));

        return result;
    }())
{}

list::list(const std::vector<std::wstring>& items)
    : impl_(new implement(*this))
{
    vertical_scroll().min(0);
    vertical_scroll().step(36);
    vertical_scroll().show();
    vertical_scroll().onScroll(
        [this](const scroll* self)
        {
            repaint();
        });

    horizontal_scroll().min(0);
    horizontal_scroll().step(36);
    horizontal_scroll().show();
    horizontal_scroll().onScroll(
        [this](const scroll* self)
        {
            repaint();
        });

    add(&vertical_scroll());
    add(&horizontal_scroll());

    if (!items.empty())
        for (const std::wstring& item : items)
            impl_->items_.emplace_back(item);

    impl_->update();
}

list::list(std::initializer_list<std::string> _list)
    : list(_list, sys::default_codec())
{}

list::list(std::initializer_list<std::string> _list, const std::string& codec)
    : list(std::vector<std::string>(_list.begin(), _list.end()), codec)
{}

list::list(std::initializer_list<std::wstring> _list)
    : list(std::vector<std::wstring>(_list.begin(), _list.end()))
{}

list::~list(void)
{
    delete impl_;
}

unsigned int list::count(void) const
{
    return impl_->items_.size();
}

std::vector<std::wstring> list::items(void) const
{
    std::vector<std::wstring> items;

    for (implement::item& item : impl_->items_)
        items.push_back(item.text.content());

    return items;
}

std::wstring list::item(unsigned int index) const
{
    return impl_->items_[index].text.content();
}

unsigned int list::current(void) const
{
    return impl_->current_;
}

unsigned int list::edit_trigger(void) const
{
    return impl_->edit_trigger_;
}

bool list::editable(unsigned int index) const
{
    return impl_->items_[index].editable;
}

SelectionMode list::selection_mode(void) const
{
    return impl_->selmode_;
}

bool list::selected(unsigned int index) const
{
    return impl_->selecteds_.find(index) != impl_->selecteds_.end();
}

std::set<unsigned int> list::selected(void) const
{
    return impl_->selecteds_;
}

const font* list::font(unsigned int index) const
{
    if (index < impl_->items_.size())
        return impl_->items_[index].text.font();

    return nullptr;
}

brush_sptr list::text_brush(unsigned int index) const
{
    if (index < impl_->items_.size() && impl_->items_[index].brush)
        return impl_->items_[index].brush->text;

    return nullptr;
}

brush_sptr list::background_brush(unsigned int index) const
{
    if (index < impl_->items_.size() && impl_->items_[index].brush)
        return impl_->items_[index].brush->background;

    return nullptr;
}

brush_sptr list::selection_brush(unsigned int index) const
{
    if (index < impl_->items_.size() && impl_->items_[index].brush)
        return impl_->items_[index].brush->selection;

    return nullptr;
}

unsigned int list::alignment(unsigned int index) const
{
    if (index < impl_->items_.size())
        return impl_->items_[index].text.alignment();

    return 0;
}

void list::item(unsigned int index, const std::string& text)
{
    item(index, text, sys::default_codec());
}

void list::item(unsigned int index,
                const std::string& text, const std::string& codec)
{
    item(index, stralgo::decode(text, codec));
}

void list::item(unsigned int index, const std::wstring& text)
{
    if (index < impl_->items_.size()
        && impl_->items_[index].text.content() != text)
    {
        Size old = impl_->items_[index].text.world();
        impl_->items_[index].text.content(text);
        impl_->adjust_hvscroll(old, impl_->items_[index].text.world());
        onItemTextChangedHandle(index);
    }
}

void list::append(const std::string& text)
{
    append(text, sys::default_codec());
}

void list::append(const std::string& text, const std::string& codec)
{
    append(stralgo::decode(text, codec));
}

void list::append(const std::wstring& text)
{
    insert(impl_->items_.size(), text);
}

void list::insert(unsigned int index, const std::string& text)
{
    insert(index, text, sys::default_codec());
}

void list::insert(unsigned int index,
                  const std::string& text, const std::string& codec)
{
    insert(index, stralgo::decode(text, codec));
}

void list::insert(unsigned int index, const std::wstring& text)
{
    impl_->items_.emplace(impl_->items_.begin() + index, text);
    impl_->adjust_hvscroll({0, 0}, impl_->items_[index].text.world());
}

void list::remove(unsigned int index)
{
    Size old = impl_->items_[index].text.world();
    impl_->items_.erase(impl_->items_.begin() + index);
    impl_->adjust_hvscroll(old, {0, 0});
}

void list::clear(void)
{
    clear_selection();
    impl_->items_.clear();
    impl_->update();
}

void list::current(unsigned int index)
{
    if (index < impl_->items_.size() && index != impl_->current_)
    {
        unsigned int oldcurrent = impl_->current_;
        impl_->current_ = index;
        onCurrentItemChangedHandle(impl_->current_, oldcurrent);
        repaint();
    }
}

void list::edit_trigger(unsigned int edit_trigger)
{
    impl_->edit_trigger_ = edit_trigger;
}

void list::editable(unsigned int index, bool editable)
{
    impl_->items_[index].editable = editable;
}

void list::selection_mode(SelectionMode selection_mode)
{
    impl_->selmode_ = selection_mode;
}

void list::selected(unsigned int index, bool _selected)
{
    selected(index, index, _selected);
}

void list::selected(unsigned int begin, unsigned int end, bool _selected)
{
    if (begin > end)
        std::swap(begin, end);

    if (begin >= impl_->items_.size())
        return;
    else if (end >= impl_->items_.size())
        end = impl_->items_.size() - 1;

    std::set<unsigned int> inserting;
    for (unsigned int i = begin; i <= end; ++i)
        inserting.insert(i);

    selected(inserting, _selected);
}

void list::selected(const std::set<unsigned int>& indexes, bool selected)
{
    std::vector<unsigned int> result;
    using namespace std;

    if (selected)
    {
        set_difference(indexes.begin(), indexes.end(),
                       impl_->selecteds_.begin(), impl_->selecteds_.end(),
                       inserter(result, result.begin()));

        for (unsigned int index : result)
        {
            impl_->selecteds_.insert(index);
            onItemSelectedHandle(index);
            onItemSelectionChangedHandle(index);
        }
    }
    else
    {
        set_intersection(indexes.begin(), indexes.end(),
                         impl_->selecteds_.begin(), impl_->selecteds_.end(),
                         inserter(result, result.begin()));

        for (unsigned int index : result)
        {
            impl_->selecteds_.erase(index);
            onItemDisselectedHandle(index);
            onItemSelectionChangedHandle(index);
        }
    }

    if (!result.empty())
        onSelectionChangedHandle(result, selected);

    repaint();
}

void list::select(unsigned int index)
{
    selected(index, true);
}

void list::select(unsigned int begin, unsigned int end)
{
    selected(begin, end, true);
}

void list::select(const std::set<unsigned int>& indexes)
{
    selected(indexes, true);
}

void list::disselect(unsigned int index)
{
    selected(index, false);
}

void list::disselect(unsigned int begin, unsigned int end)
{
    selected(begin, end, false);
}

void list::disselect(const std::set<unsigned int>& indexes)
{
    selected(indexes, false);
}

void list::clear_selection(void)
{
    selected(impl_->selecteds_, false);
}

void list::font(const class font* font)
{
    for (implement::item& item : impl_->items_)
        item.text.font(font);

    impl_->update();
}

void list::font(unsigned int index, const class font* font)
{
    if (index < impl_->items_.size())
    {
        Size old = impl_->items_[index].text.world();
        impl_->items_[index].text.font(font);
        impl_->adjust_hvscroll(old, impl_->items_[index].text.world());
    }
}

void list::text_color(const color& color)
{
    text_brush(make_brushsptr<color_brush>(color));
}

void list::text_color(unsigned int index, const color& color)
{
    text_brush(index, make_brushsptr<color_brush>(color));
}

void list::text_brush(const brush_sptr& brush)
{
    impl_->brush_.text = brush;

    for (implement::item& item : impl_->items_)
        if (item.brush)
            item.brush->text.reset();

    repaint();
}

void list::text_brush(unsigned int index, const brush_sptr& brush)
{
    if (index < impl_->items_.size())
    {
        if (!impl_->items_[index].brush)
            impl_->items_[index].brush.reset(new implement::brush);

        if (impl_->items_[index].brush->text != brush)
        {
            impl_->items_[index].brush->text = brush;
            repaint();
        }
    }
}

void list::background_color(const color& color)
{
    background_brush(make_brushsptr<color_brush>(color));
}

void list::background_color(unsigned int index, const color& color)
{
    background_brush(index, make_brushsptr<color_brush>(color));
}

void list::background_brush(const brush_sptr& brush)
{
    impl_->brush_.background = brush;

    for (implement::item& item : impl_->items_)
        if (item.brush)
            item.brush->background.reset();

    repaint();
}

void list::background_brush(unsigned int index, const brush_sptr& brush)
{
    if (index < impl_->items_.size())
    {
        if (!impl_->items_[index].brush)
            impl_->items_[index].brush.reset(new implement::brush);

        if (impl_->items_[index].brush->background != brush)
        {
            impl_->items_[index].brush->background = brush;
            repaint();
        }
    }
}

void list::selection_color(const color& color)
{
    selection_brush(make_brushsptr<color_brush>(color));
}

void list::selection_color(unsigned int index, const color& color)
{
    selection_brush(index, make_brushsptr<color_brush>(color));
}

void list::selection_brush(const brush_sptr& brush)
{
    impl_->brush_.selection = brush;

    for (implement::item& item : impl_->items_)
        if (item.brush)
            item.brush->selection.reset();

    repaint();
}

void list::selection_brush(unsigned int index, const brush_sptr& brush)
{
    if (index < impl_->items_.size())
    {
        if (!impl_->items_[index].brush)
            impl_->items_[index].brush.reset(new implement::brush);

        if (impl_->items_[index].brush->selection != brush)
        {
            impl_->items_[index].brush->selection = brush;
            repaint();
        }
    }
}

void list::alignment(unsigned int alignment)
{
    for (implement::item& item : impl_->items_)
        item.text.alignment(alignment);

    repaint();
}

void list::alignment(unsigned int index, unsigned int alignment)
{
    if (index < impl_->items_.size())
    {
        impl_->items_[index].text.alignment(alignment);
        repaint();
    }
}

void list::edit(unsigned int index)
{
    edit(index, L"");
}

void list::edit(unsigned int index, const std::string& text)
{
    edit(index, text, sys::default_codec());
}

void list::edit(unsigned int index,
                const std::string& text, const std::string& codec)
{
    edit(index, stralgo::decode(text, codec));
}

void list::edit(unsigned int index, const std::wstring& text)
{
    if (index < count())
    {
        scrollto(index);

        editbox& eb = editbox::
            pop(this, impl_->item_rect(index), impl_->items_[index].text, text);

        handler::sptr handler =
            vertical_scroll().onScroll(horizontal_scroll().onScroll(
            [this, index, &eb](scroll*)
            {
                eb.xy(impl_->item_rect(index).left_top());
            }));

        eb.background_color(color::white());
        eb.onEditComplete(
            [this, index](editbox*, const std::wstring& text)
            {
                item(index, text);
            });
        eb.onEditFinish(
            [this, handler](editbox*)
            {
                vertical_scroll().onScrollRemove(handler);
                horizontal_scroll().onScrollRemove(handler);
            });
    }
}

long list::hittest(Point point) const
{
    int delta = point.y + vertical_scroll().valued();

    for (unsigned int i = 0; i < impl_->items_.size(); ++i)
    {
        if (delta < impl_->items_[i].text.world().height)
            return i;
        delta -= impl_->items_[i].text.world().height;
    }

    return -1;
}

void list::scrollto(unsigned int index)
{
    scrollto(index, aUndefined);
}

void list::scrollto(unsigned int index, unsigned int alignment)
{
    if (index >= impl_->items_.size())
        index  = impl_->items_.size() - 1;

    int item_height = impl_->items_[index].text.world().height;
    int top    = impl_->accumulate(index);
    int middle = top - (height() - item_height) / 2;
    int bottom = top + item_height - height();

    if (alignment == aUndefined)
    {
        if (top < vertical_scroll().valued())
            vertical_scroll().value(top);
        else if (bottom > vertical_scroll().valued())
            vertical_scroll().value(bottom);
    }
    else
    {
        switch (alignment & aVertical)
        {
        case aTop:
            vertical_scroll().value(top);
            break;
        case aCenter:
            vertical_scroll().value(middle);
            break;
        case aBottom:
            vertical_scroll().value(bottom);
            break;
        }
    }
}

void list::onSize(void)
{
    for (implement::item& item : impl_->items_)
        item.text.view(width(), item.text.view().height);
    scroll_area::onSize();
}

void list::onPaint(graphics graphics, Rect rect)
{
    class color_scheme cs = color_scheme();
    color selcolor = focused() ? cs.selection() : cs.inactive_selection();
    brush_sptr textbrush = make_brushsptr<color_brush>(cs.text());
    int y = -vertical_scroll().valued();

    for (int i = 0; i < (int)impl_->items_.size() && y < rect.bottom; ++i)
    {
        implement::item& item = impl_->items_[i];
        int height = item.text.world().height;

        if (y + height > rect.top)
        {
            rectf itrct(0, y, width(), y + height);

            if (item.brush && item.brush->background)
                graphics.fill_rectangle(itrct, *item.brush->background);
            else if (impl_->brush_.background)
                graphics.fill_rectangle(itrct, *impl_->brush_.background);

            if (selected(i))
            {
                if (item.brush && item.brush->selection)
                    graphics.fill_rectangle(itrct, *item.brush->selection);
                else if (impl_->brush_.selection)
                    graphics.fill_rectangle(itrct, *impl_->brush_.selection);
                else
                    for (int i = 0; i < height; i += 1)
                        graphics.draw_line(0, y + i, width(), y + i,
                                           selcolor * (i * 64 / height + 128));
                graphics.draw_rectangle(itrct.inflate(-0.5), selcolor * 100);
            }

            item.text.offset(horizontal_scroll().valued(), -y);
            if (item.brush && item.brush->text)
                item.text.draw(graphics, rect, nullptr, *item.brush->text);
            else if (impl_->brush_.text)
                item.text.draw(graphics, rect, nullptr, *impl_->brush_.text);
            else
                item.text.draw(graphics, rect, nullptr, *textbrush);
        }

        if ((y += height) >= rect.bottom)
            break;
    }

    if (current() < impl_->items_.size())
    {
        rectf itrct = impl_->item_rect(current());
        pen pen(1, selcolor, {1, 1});
        pen.offset(0.5);
        graphics.draw_rectangle(itrct.inflate(-0.5), pen);
    }
}

bool list::onFocus(bool)
{
    repaint();

    return true;
}

bool list::onKeyDown(sys::VirtualKey vkey)
{
    switch (vkey)
    {
    case sys::vkUp:
    case sys::vkDown:
        {
            int index = current();
            if (vkey == sys::vkUp)
                --index;
            else
                ++index;
            if (index < 0 || index >= (int)impl_->items_.size())
                break;
            else
                current(index);

            switch (impl_->selmode_)
            {
            case smNoSelection:
                break;
            default:
            case smSingleSelection:
                clear_selection();
                select(index);
                break;
            case smMultiSelection:
                impl_->selbegin_ = index;
                break;
            case smExtendedSelection:
                {
                    bool ctrl  = sys::key_pressed(sys::vkControl);
                    bool shift = sys::key_pressed(sys::vkShift);

                    if (!ctrl)
                        clear_selection();
                    if (shift)
                        select(impl_->selbegin_, index);
                    else
                    {
                        if (!ctrl)
                            select(index);
                        impl_->selbegin_ = index;
                    }
                }
                break;
            case smContiguousSelection:
                clear_selection();
                if (sys::key_pressed(sys::vkShift))
                    select(impl_->selbegin_, index);
                else
                {
                    select(index);
                    impl_->selbegin_ = index;
                }
                break;
            }

            scrollto(index);
        }
        break;
    case sys::vkReturn:
        if (impl_->selmode_ == smMultiSelection)
            selected(current(), !selected(current()));
        else
        if (impl_->selmode_ == smExtendedSelection)
        {
            if (!sys::key_pressed(sys::vkControl))
            {
                clear_selection();
                select(current());
            }
            else
                selected(current(), !selected(current()));
        }
        break;
    case sys::vkKeyA:
        if (sys::key_pressed(sys::vkControl)
            && (impl_->selmode_ == smMultiSelection
             || impl_->selmode_ == smExtendedSelection
             || impl_->selmode_ == smContiguousSelection))
            select(0, impl_->items_.size());
        break;
    case sys::vkHome:
        scrollto(0);
        break;
    case sys::vkEnd:
        scrollto(impl_->items_.size());
        break;
    case sys::vkPrior:
        vertical_scroll().page_up();
        break;
    case sys::vkNext:
        vertical_scroll().page_down();
        break;
    default:
        return false;
    }

    return true;
}

bool list::onKeyPress(unsigned int chr)
{
    if (!editable(current()))
        return true;

    if (    (edit_trigger() & etAnyKeyPressed)
        || ((edit_trigger() & etEnterPressed) && chr == 13))
    {
        chr = codec::gbk2uni(chr);

        if (stralgo::iswprint(chr))
            edit(current(), std::wstring(1, chr));
        else
            edit(current());
    }

    return true;
}

bool list::onMouseDown(sys::Button button, int times, Point point)
{
    int index = hittest(point);
    if (index < 0)
        return true;
    else
    {
        onItemClickedHandle(index, button, times);
        if (times == 2)
            onItemDoubleClickedHandle(index, button);
    }

    if (button != sys::btnLeft)
        return true;
    else
    if ((  ((edit_trigger() & etDoubleClicked)   && times == 2)
        || ((edit_trigger() & etSelectedClicked) && index == (int)current()))
        && editable(index))
    {
        edit(index);
        return true;
    }
    else
        current(index);

    switch (impl_->selmode_)
    {
    case smNoSelection:
        break;
    default:
    case smSingleSelection:
        clear_selection();
        select(index);
        break;
    case smMultiSelection:
        selected(index, !selected(index));
        impl_->selbegin_ = index;
        break;
    case smExtendedSelection:
        {
            bool ctrl  = sys::key_pressed(sys::vkControl);
            bool shift = sys::key_pressed(sys::vkShift);

            if (!ctrl)
                clear_selection();
            if (shift)
                select(impl_->selbegin_, index);
            else
            {
                if (!ctrl)
                    select(index);
                else
                    selected(index, !selected(index));
                impl_->selbegin_ = index;
            }
        }
        break;
    case smContiguousSelection:
        clear_selection();
        if (sys::key_pressed(sys::vkShift))
            select(impl_->selbegin_, index);
        else
        {
            select(index);
            impl_->selbegin_ = index;
        }
        break;
    }

    scrollto(index);
    capture(true);

    return true;
}

bool list::onMouseMove(Point point)
{
    if (!sys::key_pressed(sys::vkLButton))
        return true;
    int index = hittest(point), oldcurrent = current();
    if (index < 0 || index == oldcurrent)
        return true;
    else
        current(index);

    switch (impl_->selmode_)
    {
    case smNoSelection:
        break;
    default:
    case smSingleSelection:
        clear_selection();
        select(index);
        break;
    case smMultiSelection:
    case smExtendedSelection:
        selected(impl_->selbegin_, oldcurrent, !selected(impl_->selbegin_));
        selected(impl_->selbegin_, index, !selected(impl_->selbegin_));
        break;
    case smContiguousSelection:
        clear_selection();
        select(impl_->selbegin_, index);
        break;
    }

    scrollto(index);

    return true;
}

bool list::onMouseUp(sys::Button button, Point point)
{
    capture(false);

    return true;
}
