#include <algorithm>
#include <takisy/core/sys.h>
#include <takisy/core/math.h>
#include <takisy/core/codec.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/widget/table.h>
#include "../basic/text.hpp"
#include "edit_box.h"

class table::implement
{
    friend class table;

    struct item
    {
        class text text;
        brush_sptr fgbrush, bgbrush, selbrush;
        bool       editable;

    public:
        item(void)
            : fgbrush(nullptr), bgbrush(nullptr), selbrush(nullptr)
            , editable(false)
        {
            text.margin(2);
            text.alignment(aCenter);
            text.word_wrap(false);
        }
    };

    struct gridline
    {
        pen pen;

    public:
        gridline(void)
            : pen(1, color::gray())
        {}

    public:
        unsigned int width(void) const
        {
            return math::ceil(pen.width());
        }
    };

    struct header : public item
    {
    public:
        header(void)
        {
            bgbrush = make_color_brush_sptr(color::light_gray());
        }
    };

    struct row_header : public header
    {
        unsigned int height;

    public:
        row_header(void)
            : height(16)
        {}
    };

    struct column_header : public header
    {
        unsigned int width;

    public:
        column_header(void)
            : width(80)
        {}
    };

    struct row_type
    {
        struct row_header        header;
        gridline                 gridline;
        std::vector<struct item> items;

    public:
        unsigned int height(void) const
        {
            return header.height + gridline.width();
        }
    };

    struct column_type
    {
        struct column_header header;
        gridline             gridline;

    public:
        unsigned int width(void) const
        {
            return header.width + gridline.width();
        }
    };

public:
    implement(table& self)
        : self(self)
        , selmode_(smSingleSelection), selbehavior_(sbSelectItems)
        , edit_trigger_(etNoEditTrigger)
        , selbegin_({0, 0}), current_({0, 0}), enter_({-1, -1})
    {}

public:
    bool exists(const item_index& index) const
    {
        return static_cast<unsigned int>(index.row) < rows_.size()
            && static_cast<unsigned int>(index.col) < columns_.size();
    }

    unsigned int vaccumulate(unsigned int row) const
    {
        unsigned int border_width = border_.width();
        unsigned int height = std::accumulate(
            rows_.begin(), rows_.begin() + row, border_width,
            [](unsigned int height, const row_type& that)
            {
                return height + that.height();
            });

        if (column_header_.visible)
            height += column_header_.height;
        if (row == rows_.size())
            height += border_width;

        return height;
    }

    unsigned int haccumulate(unsigned int col) const
    {
        unsigned int border_width = border_.width();
        unsigned int width = std::accumulate(
            columns_.begin(), columns_.begin() + col, border_width,
            [](unsigned int width, const column_type& that)
            {
                return width + that.width();
            });

        if (row_header_.visible)
            width += row_header_.width;
        if (col == columns_.size())
            width += border_width;

        return width;
    }

    Rect item_rect(item_index index) const
    {
        Rect rect;

        if (index.row == -1)
        {
            rect.top    = border_.width();
            rect.bottom = column_header_.height;
        }
        else if (static_cast<unsigned int>(index.row) < rows_.size())
        {
            const row_type& row = rows_[index.row];

            rect.top    = vaccumulate(index.row) + row.gridline.width();
            rect.bottom = rect.top  + row.header.height;
        }

        if (index.col == -1)
        {
            rect.left   = border_.width();
            rect.right  = row_header_.width;
        }
        else if (static_cast<unsigned int>(index.col) < columns_.size())
        {
            const column_type& column = columns_[index.col];

            rect.left   = haccumulate(index.col) + column.gridline.width();
            rect.right  = rect.left + column.header.width;
        }

        return rect.offset(-self.horizontal_scroll().valued(),
                           -self.vertical_scroll().valued());
    }

    Rect items_rect(const std::vector<item_index>& indexes)
    {
        if (indexes.empty())
            return Rect();

        item_index min = indexes.front(), max = min;

        for (const item_index& index : indexes)
        {
            if (min.row > index.row)
                min.row = index.row;
            else if (max.row < index.row)
                max.row = index.row;

            if (min.col > index.col)
                min.col = index.col;
            else if (max.col < index.col)
                max.col = index.col;
        }

        return item_rect(min).unit(item_rect(max));
    }

public:
    void update(void)
    {
        self.vertical_scroll()
            .range(0, vaccumulate(rows_.size()));
        self.horizontal_scroll()
            .range(0, haccumulate(columns_.size()));
        self.repaint();
    }

    void adjust_hscroll(unsigned int before, unsigned int after)
    {
        self.horizontal_scroll()
            .max(self.horizontal_scroll().max() + after - before);
    }

    void adjust_vscroll(unsigned int before, unsigned int after)
    {
        self.vertical_scroll()
            .max(self.vertical_scroll().max() + after - before);
    }

    void draw_header(graphics& graphics,
                     const brush_sptr& txtbrush,
                     int x, int y)
    {
        Rect rect(x, y, 0, y + column_header_.height);

        for (column_type& column : columns_)
        {
            rect.left += column.gridline.width();
            rect.width(column.header.width);

            header& header = column.header;
            if (header.bgbrush)
                graphics.fill_rectangle(rect, *header.bgbrush);

            text_rect(header.text, rect);
            if (header.fgbrush)
                header.text.draw(graphics, rect, nullptr, *header.fgbrush);
            else
                header.text.draw(graphics, rect, nullptr, *txtbrush);

            rect.left += rect.width();
        }
    }

    void draw_item(graphics& graphics,
                   const brush_sptr& selbrush, const brush_sptr& txtbrush,
                   struct item& item, bool selected, const Rect& rect)
    {
        if (item.bgbrush)
            graphics.fill_rectangle(rect, *item.bgbrush);

        if (selected)
        {
            if (item.selbrush)
                graphics.fill_rectangle(rect, *item.selbrush);
            else
                graphics.fill_rectangle(rect, *selbrush);
        }

        text_rect(item.text, rect);
        if (item.fgbrush)
            item.text.draw(graphics, rect, nullptr, *item.fgbrush);
        else
            item.text.draw(graphics, rect, nullptr, *txtbrush);
    }

    int draw_row(graphics& graphics,
                 const brush_sptr& selbrush, const brush_sptr& txtbrush,
                 int x, int y, int index)
    {
        row_type& row = rows_[index];
        Rect rect;

        rect.left   = x;
        rect.top    = y + row.gridline.width();
        rect.bottom = rect.top + row.header.height;

        if (row_header_.visible)
        {
            header& header = row.header;
            rect.width(row_header_.width);
            if (header.bgbrush)
                graphics.fill_rectangle(rect, *header.bgbrush);
            text_rect(header.text, rect);
            if (header.fgbrush)
                header.text.draw(graphics, rect, nullptr, *header.fgbrush);
            else
                header.text.draw(graphics, rect, nullptr, *txtbrush);
            rect.left += rect.width();
        }

        for (unsigned int col = 0; col < columns_.size(); ++col)
        {
            rect.left += columns_[col].gridline.width();
            rect.width(columns_[col].header.width);

            draw_item(graphics, selbrush, txtbrush,
                      row.items[col], self.selected({index, (int)col}),
                      rect);

            rect.left += rect.width();
        }

        return rect.bottom;
    }

    void draw_lines(graphics& graphics, Rect rect)
    {
        int hsvalue = self.horizontal_scroll().valued(), dx = -hsvalue;
        int vsvalue = self.vertical_scroll().valued(),   dy = -vsvalue;
        int right   = dx + haccumulate(columns_.size());
        int bottom  = dy + vaccumulate(rows_.size());

        if (rect.right > right)
            rect.right = right;
        if (rect.bottom > bottom)
            rect.bottom = bottom;

        int border_width = border_.width();
        if (border_width)
        {
            graphics.draw_rectangle(
                rectf(dx, dy, right, bottom).inflate(border_width / -2.0),
                border_.pen);
            dx += border_width;
            dy += border_width;
        }

        if (row_header_.visible)
            dx += row_header_.width;
        if (column_header_.visible)
            dy += column_header_.height;

        for (column_type& column : columns_)
        {
            double glwidth = column.gridline.width();

            if (dx + glwidth > rect.left)
            {
                double x = dx + glwidth / 2;
                pen& pen = column.gridline.pen;
                pen.offset(vsvalue);
                graphics.draw_line(x, rect.top, x, rect.bottom, pen);
            }

            dx += column.width();
            if (dx >= rect.right)
                break;
        }

        for (row_type& row : rows_)
        {
            double glwidth = row.gridline.width();

            if (dy + glwidth > rect.top)
            {
                double y = dy + glwidth / 2;
                pen& pen = row.gridline.pen;
                pen.offset(hsvalue);
                graphics.draw_line(rect.left, y, rect.right, y, pen);
            }

            dy += row.height();
            if (dy >= rect.bottom)
                break;
        }
    }

private:
    void text_rect(class text& text, const Rect& rect)
    {
        int a = text.height(), b = rect.height();

        switch (text.alignment() & aVertical)
        {
        default:
        case aTop:    text.offset(-rect.left, -rect.top); break;
        case aCenter: text.offset(-rect.left, -rect.top + (a - b) / 2); break;
        case aBottom: text.offset(-rect.left, -rect.top +  a - b); break;
        }

        text.view(rect.width(), rect.height());
    }

private:
    table&                                   self;
    gridline                                 border_;
    std::vector<row_type>                    rows_;
    std::vector<column_type>                 columns_;
    struct { unsigned int width   = 32;
             bool         visible = false; } row_header_;
    struct { unsigned int height  = 16;
             bool         visible = true; }  column_header_;
    SelectionMode                            selmode_;
    SelectionBehavior                        selbehavior_;
    unsigned int                             edit_trigger_;
    item_index                               selbegin_, current_, enter_;
    std::set<item_index>                     selecteds_;
};

bool table::item_index::operator==(const item_index& that) const
{
    return row == that.row && col == that.col;
}

bool table::item_index::operator!=(const item_index& that) const
{
    return !operator==(that);
}

bool table::item_index::operator <(const item_index& that) const
{
    return row < that.row || (row == that.row && col < that.col);
}

struct table::item::implement
{
    table& host;
    table::implement::item& item;
    item_index index;

public:
    implement(table& host, table::implement::item& item,
              const item_index& index)
        : host(host), item(item), index(index)
    {}

public:
    void repaint(void)
    {
        host.item_repaint(index);
    }
};

struct table::gridline::implement
{
    table& host;
    table::implement::gridline& gridline;
    unsigned int index;

public:
    implement(table& host, table::implement::gridline& gridline,
              unsigned int index)
        : host(host), gridline(gridline), index(index)
    {}
};

struct table::row_header::implement
{
    table& host;
    table::implement::row_header& header;
    unsigned int index;

public:
    implement(table& host, table::implement::row_header& header,
              unsigned int index)
        : host(host), header(header), index(index)
    {}
};

struct table::column_header::implement
{
    table& host;
    table::implement::column_header& header;
    unsigned int index;

public:
    implement(table& host, table::implement::column_header& header,
              unsigned int index)
        : host(host), header(header), index(index)
    {}
};

table::table(void)
    : table(0)
{}

table::table(unsigned int column)
    : table(column, 0)
{}

table::table(unsigned int column, unsigned int row)
    : impl_(new implement(*this))
{
    vertical_scroll().step(36);
    vertical_scroll().show();
    vertical_scroll().onScroll(
        [this](const scroll* self)
        {
            repaint();
        });

    horizontal_scroll().step(36);
    horizontal_scroll().show();
    horizontal_scroll().onScroll(
        [this](const scroll* self)
        {
            repaint();
        });

    add(&vertical_scroll());
    add(&horizontal_scroll());

    for (unsigned int i = 0; i < column; ++i)
        append_column();
    for (unsigned int i = 0; i < row; ++i)
        append_row();

    impl_->update();
}

table::~table(void)
{
    delete impl_;
}

unsigned int table::rows(void) const
{
    return impl_->rows_.size();
}

unsigned int table::columns(void) const
{
    return impl_->columns_.size();
}

bool table::row_header_visible(void) const
{
    return impl_->row_header_.visible;
}

unsigned int table::row_header_width(void) const
{
    return impl_->row_header_.width;
}

bool table::column_header_visible(void) const
{
    return impl_->column_header_.visible;
}

unsigned int table::column_header_height(void) const
{
    return impl_->column_header_.height;
}

unsigned int table::row_height(unsigned int row) const
{
    if (row < rows())
        return impl_->rows_[row].header.height;

    return 0;
}

unsigned int table::column_width(unsigned int col) const
{
    if (col < columns())
        return impl_->columns_[col].header.width;

    return 0;
}

table::item_index table::current(void) const
{
    return impl_->current_;
}

SelectionMode table::selection_mode(void) const
{
    return impl_->selmode_;
}

SelectionBehavior table::selection_behavior(void) const
{
    return impl_->selbehavior_;
}

unsigned int table::edit_trigger(void) const
{
    return impl_->edit_trigger_;
}

bool table::selected(const item_index& index) const
{
    switch (impl_->selbehavior_)
    {
    default:
    case sbSelectItems:
        return impl_->selecteds_.find(index) != impl_->selecteds_.end();
    case sbSelectRows:
        for (const item_index& ii : impl_->selecteds_)
            if (ii.row == index.row)
                return true;
        return false;
    case sbSelectColumns:
        for (const item_index& ii : impl_->selecteds_)
            if (ii.col == index.col)
                return true;
        return false;
    }
}

const class table::item table::item(const item_index& index) const
{
    table& self = const_cast<table&>(*this);
    implement::item& item = impl_->rows_[index.row].items[index.col];
    class item ret;

    ret.impl.reset(new item::implement(self, item, index));

    return ret;
}

const class table::row_header table::row_header(unsigned int index) const
{
    table& self = const_cast<table&>(*this);
    implement::row_header& header = impl_->rows_[index].header;
    item_index ii = {(int)index, -1};
    class row_header ret;

    ret.item::impl.reset(new item::implement(self, header, ii));
    ret.impl.reset(new row_header::implement(self, header, index));

    return ret;
}

const class table::column_header table::column_header(unsigned int index) const
{
    table& self = const_cast<table&>(*this);
    implement::column_header& header = impl_->columns_[index].header;
    item_index ii = {-1, (int)index};
    class column_header ret;

    ret.item::impl.reset(new item::implement(self, header, ii));
    ret.impl.reset(new column_header::implement(self, header, index));

    return ret;
}

const class table::gridline table::border(void) const
{
    table& self = const_cast<table&>(*this);
    class gridline ret;

    ret.impl.reset(new gridline::implement(self, impl_->border_, 0));

    return ret;
}

const class table::gridline table::vertical_gridline(unsigned int index) const
{
    table& self = const_cast<table&>(*this);
    implement::gridline& gridline = impl_->rows_[index].gridline;
    class gridline ret;

    ret.impl.reset(new gridline::implement(self, gridline, index));

    return ret;
}

const class table::gridline table::horizontal_gridline(unsigned int index) const
{
    table& self = const_cast<table&>(*this);
    implement::gridline& gridline = impl_->rows_[index].gridline;
    class gridline ret;

    ret.impl.reset(new gridline::implement(self, gridline, index));

    return ret;
}

class table::item table::item(const item_index& index)
{
    implement::item& item = impl_->rows_[index.row].items[index.col];
    class item ret;

    ret.impl.reset(new item::implement(*this, item, index));

    return ret;
}

class table::row_header table::row_header(unsigned int index)
{
    implement::row_header& header = impl_->rows_[index].header;
    item_index ii = {(int)index, -1};
    class row_header ret;

    ret.item::impl.reset(new item::implement(*this, header, ii));
    ret.impl.reset(new row_header::implement(*this, header, index));

    return ret;
}

class table::column_header table::column_header(unsigned int index)
{
    implement::column_header& header = impl_->columns_[index].header;
    item_index ii = {-1, (int)index};
    class column_header ret;

    ret.item::impl.reset(new item::implement(*this, header, ii));
    ret.impl.reset(new column_header::implement(*this, header, index));

    return ret;
}

class table::gridline table::border(void)
{
    class gridline ret;

    ret.impl.reset(new gridline::implement(*this, impl_->border_, 0));

    return ret;
}

class table::gridline table::vertical_gridline(unsigned int index)
{
    implement::gridline& gridline = impl_->rows_[index].gridline;
    class gridline ret;

    ret.impl.reset(new gridline::implement(*this, gridline, index));

    return ret;
}

class table::gridline table::horizontal_gridline(unsigned int index)
{
    implement::gridline& gridline = impl_->rows_[index].gridline;
    class gridline ret;

    ret.impl.reset(new gridline::implement(*this, gridline, index));

    return ret;
}

void table::header_visible(bool visible)
{
    row_header_visible(visible);
    column_header_visible(visible);
}

void table::row_header_visible(bool visible)
{
    if (impl_->row_header_.visible != visible)
    {
        impl_->row_header_.visible = visible;
        impl_->update();
    }
}

void table::row_header_width(unsigned int width)
{
    if (impl_->row_header_.width != width)
    {
        impl_->row_header_.width = width;
        impl_->update();
    }
}

void table::row_height(unsigned int row, unsigned int height)
{
    if (row < rows())
    {
        impl_->rows_[row].header.height = height;
        impl_->update();
    }
}

void table::column_header_visible(bool visible)
{
    if (impl_->column_header_.visible != visible)
    {
        impl_->column_header_.visible = visible;
        impl_->update();
    }
}

void table::column_header_height(unsigned int height)
{
    if (impl_->column_header_.height != height)
    {
        impl_->column_header_.height = height;
        impl_->update();
    }
}

void table::column_width(unsigned int col, unsigned int width)
{
    if (col < columns())
    {
        impl_->columns_[col].header.width = width;
        impl_->update();
    }
}

void table::show_header(void)
{
    show_row_header();
    show_column_header();
}

void table::hide_header(void)
{
    hide_row_header();
    hide_column_header();
}

void table::show_row_header(void)
{
    row_header_visible(true);
}

void table::hide_row_header(void)
{
    row_header_visible(false);
}

void table::show_column_header(void)
{
    column_header_visible(true);
}

void table::hide_column_header(void)
{
    column_header_visible(false);
}

void table::append_row(void)
{
    insert_row(rows());
}

void table::append_row(const std::string& header)
{
    insert_row(rows(), header);
}

void table::append_row(const std::string& header, const std::string& codec)
{
    insert_row(rows(), header, codec);
}

void table::append_row(const std::wstring& header)
{
    insert_row(rows(), header);
}

void table::insert_row(unsigned int row)
{
    insert_row(row, stralgo::format("%d", row + 1));
}

void table::insert_row(unsigned int row, const std::string& header)
{
    insert_row(row, header, sys::default_codec());
}

void table::insert_row(unsigned int row,
                       const std::string& header, const std::string& codec)
{
    insert_row(row, stralgo::decode(header, codec));
}

void table::insert_row(unsigned int row, const std::wstring& header)
{
    if (row > rows())
        row = rows();

    impl_->rows_.emplace(impl_->rows_.begin() + row);
    impl_->rows_[row].header.text.content(header);
    impl_->rows_[row].items.resize(columns());

    impl_->update();
}

void table::remove_row(unsigned int row)
{
    if (row < rows())
    {
        impl_->rows_.erase(impl_->rows_.begin() + row);
        impl_->update();
    }
}

void table::append_column(void)
{
    insert_column(columns());
}

void table::append_column(const std::string& header)
{
    insert_column(columns(), header);
}

void table::append_column(const std::string& header, const std::string& codec)
{
    insert_column(columns(), header, codec);
}

void table::append_column(const std::wstring& header)
{
    insert_column(columns(), header);
}

void table::insert_column(unsigned int col)
{
    insert_column(col, stralgo::format("%d", col + 1));
}

void table::insert_column(unsigned int col, const std::string& header)
{
    insert_column(col, header, sys::default_codec());
}

void table::insert_column(unsigned int col,
                          const std::string& header, const std::string& codec)
{
    insert_column(col, stralgo::decode(header, codec));
}

void table::insert_column(unsigned int col, const std::wstring& header)
{
    if (col > columns())
        col = columns();

    impl_->columns_.emplace(impl_->columns_.begin() + col);
    impl_->columns_[col].header.text.content(header);

    for (implement::row_type& row : impl_->rows_)
        row.items.emplace(row.items.begin() + col);

    impl_->update();
}

void table::remove_column(unsigned int col)
{
    if (col < columns())
    {
        impl_->columns_.erase(impl_->columns_.begin() + col);

        for (implement::row_type& row : impl_->rows_)
            row.items.erase(row.items.begin() + col);

        impl_->update();
    }
}

void table::clear(void)
{
    impl_->columns_.clear();
    clear_rows();
}

void table::clear_rows(void)
{
    impl_->rows_.clear();
    impl_->update();
}

void table::clear_content(void)
{
    for (implement::row_type& row : impl_->rows_)
    for (class implement::item& item : row.items)
        item.text.content(std::wstring());

    repaint();
}

void table::clear_row(unsigned int row)
{
    if (row < rows())
    {
        for (class implement::item& item : impl_->rows_[row].items)
            item.text.content(std::wstring());

        repaint(impl_->item_rect({(int)row, -1}));
    }
}

void table::clear_column(unsigned int col)
{
    if (col < columns())
    {
        for (implement::row_type& row : impl_->rows_)
            row.items[col].text.content(std::wstring());

        repaint(impl_->item_rect({-1, (int)col}));
    }
}

void table::current(const item_index& index)
{
    if (impl_->exists(index) && index != impl_->current_)
    {
        item_index oldcurrent = impl_->current_;

        repaint(impl_->item_rect(impl_->current_));
        impl_->current_ = index;
        repaint(impl_->item_rect(impl_->current_));

        onCurrentItemChangedHandle(impl_->current_, oldcurrent);
        if (oldcurrent.row != impl_->current_.row)
            onCurrentRowChangedHandle(impl_->current_.row, oldcurrent.row);
        if (oldcurrent.col != impl_->current_.col)
            onCurrentColumnChangedHandle(impl_->current_.col, oldcurrent.col);
    }
}

void table::current_row(unsigned int row)
{
    current({(int)row, impl_->current_.col});
}

void table::current_column(unsigned int col)
{
    current({impl_->current_.row, (int)col});
}

void table::selection_mode(SelectionMode selection_mode)
{
    impl_->selmode_ = selection_mode;
}

void table::selection_behavior(SelectionBehavior selection_behavior)
{
    if (impl_->selbehavior_ != selection_behavior)
    {
        impl_->selbehavior_ = selection_behavior;
        repaint();
    }
}

void table::edit_trigger(unsigned int edit_trigger)
{
    impl_->edit_trigger_ = edit_trigger;
}

void table::selected(const item_index& index, bool _selected)
{
    selected(index, index, _selected);
}

void table::selected(const item_index& _begin, const item_index& _end,
                     bool _selected)
{
    item_index begin = _begin, end = _end;

    if (begin.row > end.row)
        std::swap(begin.row, end.row);
    if (begin.col > end.col)
        std::swap(begin.col, end.col);

    if (begin.row >= (int)rows())
        return;
    else if (end.row >= (int)rows())
        end.row = rows() - 1;
    if (begin.col >= (int)columns())
        return;
    else if (end.col >= (int)columns())
        end.col = columns() - 1;

    std::set<item_index> inserting;
    for (int row = begin.row; row <= end.row; ++row)
    for (int col = begin.col; col <= end.col; ++col)
        inserting.insert({row, col});

    selected(inserting, _selected);
}

void table::selected(const std::set<item_index>& indexes, bool selected)
{
    std::vector<item_index> result;
    using namespace std;

    if (selected)
    {
        set_difference(indexes.begin(), indexes.end(),
                       impl_->selecteds_.begin(), impl_->selecteds_.end(),
                       inserter(result, result.begin()));

        for (item_index& index : result)
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

        for (item_index& index : result)
        {
            impl_->selecteds_.erase(index);
            onItemDisselectedHandle(index);
            onItemSelectionChangedHandle(index);
        }
    }

    if (!result.empty())
    {
        onSelectionChangedHandle(result, selected);

        repaint(impl_->items_rect(result));
    }

}

void table::select(const item_index& index)
{
    selected(index, true);
}

void table::select(const item_index& begin, const item_index& end)
{
    selected(begin, end, true);
}

void table::select(const std::set<item_index>& indexes)
{
    selected(indexes, true);
}

void table::disselect(const item_index& index)
{
    selected(index, false);
}

void table::disselect(const item_index& begin, const item_index& end)
{
    selected(begin, end, false);
}

void table::disselect(const std::set<item_index>& indexes)
{
    selected(indexes, false);
}

void table::clear_selection(void)
{
    std::vector<item_index>
        indexes(impl_->selecteds_.begin(), impl_->selecteds_.end());
    impl_->selecteds_.clear();
    repaint(impl_->items_rect(indexes));
}

table::item_index table::hittest(Point point) const
{
    unsigned int border_width = impl_->border_.width();
    item_index index = {-1, -1};

    point.y += vertical_scroll().valued() - border_width;
    if (point.y < 0)
        index.row = rows();
    else if (!column_header_visible() || point.y >= (int)column_header_height())
    {
        point.y  -= column_header_height();
        index.row = 0;

        for (implement::row_type& row : impl_->rows_)
        {
            Point::axis_type row_height = row.height();
            if (point.y < row_height)
                break;
            point.y -= row_height;
            index.row++;
        }
    }

    point.x += horizontal_scroll().valued() - border_width;
    if (point.x < 0)
        index.col = columns();
    else if (!row_header_visible() || point.x >= (int)row_header_width())
    {
        point.x  -= row_header_width();
        index.col = 0;

        for (implement::column_type& column : impl_->columns_)
        {
            Point::axis_type column_width = column.width();
            if (point.x < column_width)
                break;
            point.x -= column_width;
            index.col++;
        }
    }

    return index;
}

void table::scrollto(const item_index& index)
{
    scrollto_row(index.row);
    scrollto_column(index.col);
}

void table::scrollto_row(unsigned int row)
{
    if (row >= rows())
        row  = rows() - 1;

    unsigned int top    = impl_->vaccumulate(row);
    unsigned int bottom = top + impl_->rows_[row].header.height;

    if (top < vertical_scroll().valued())
        vertical_scroll().value(top);
    else if (bottom > vertical_scroll().valued() + height())
        vertical_scroll().value(bottom - height());
}

void table::scrollto_column(unsigned int col)
{
    if (col >= columns())
        col  = columns() - 1;

    unsigned int left  = impl_->haccumulate(col);
    unsigned int right = left + impl_->columns_[col].header.width;

    if (left < horizontal_scroll().valued())
        horizontal_scroll().value(left);
    else if (right > horizontal_scroll().valued() + width())
        horizontal_scroll().value(right - width());
}

void table::item_repaint(const item_index& index)
{
    repaint(impl_->item_rect(index));
}

void table::onSize(void)
{
    scroll_area::onSize();
}

void table::onPaint(graphics graphics, Rect rect)
{
    class color_scheme cs = color_scheme();
    color selcolor = focused() ? cs.selection() : cs.inactive_selection();
    brush_sptr selbrush = make_color_brush_sptr(selcolor);
    brush_sptr txtbrush = make_color_brush_sptr(cs.text());
    int x = impl_->border_.width() - horizontal_scroll().valued();
    int y = impl_->border_.width() - vertical_scroll().valued();

    impl_->draw_lines(graphics, rect);

    if (impl_->column_header_.visible)
    {
        int dx = x;
        if (row_header_visible())
            dx += row_header_width();
        if (y + (int)column_header_height() > rect.top)
            impl_->draw_header(graphics, txtbrush, dx, y);
        y += column_header_height();
    }

    for (unsigned int row = 0; row < rows() && y < rect.bottom; ++row)
    {
        int rowheight = row_height(row);

        if (y + rowheight > rect.top)
            y = impl_->draw_row(graphics, selbrush, txtbrush, x, y, row);
        else
            y += impl_->rows_[row].gridline.width() + rowheight;
    }

    rectf item_rect = impl_->item_rect(current());
    if (!item_rect.empty())
    {
        pen pen(1, selcolor, {1, 1});
        pen.offset(0.5);
        graphics.draw_rectangle(item_rect.inflate(-1.5), pen);
    }
}

bool table::onFocus(bool focus)
{
    std::vector<item_index>
        indexes(impl_->selecteds_.begin(), impl_->selecteds_.end());
    repaint(impl_->items_rect(indexes));

    return true;
}

bool table::onKeyDown(sys::VirtualKey vkey)
{
    switch (vkey)
    {
    case sys::vkUp:
    case sys::vkDown:
    case sys::vkLeft:
    case sys::vkRight:
        {
            item_index index = current();
            if (vkey == sys::vkUp)
                --index.row;
            else if (vkey == sys::vkDown)
                ++index.row;
            else if (vkey == sys::vkLeft)
                --index.col;
            else
                ++index.col;

            if (!impl_->exists(index))
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
            select({0, 0}, {(int)rows(), (int)columns()});
        break;
    case sys::vkHome:
        scrollto_row(0);
        break;
    case sys::vkEnd:
        scrollto_row(rows());
        break;
    case sys::vkPrior:
        vertical_scroll().page_up();
        break;
    case sys::vkNext:
        vertical_scroll().page_down();
        break;
    default:
        break;
    }

    return true;
}

bool table::onKeyPress(unsigned int chr)
{
    if (!item(current()).editable())
        return true;

    if (    (edit_trigger() & etAnyKeyPressed)
        || ((edit_trigger() & etEnterPressed) && chr == 13))
    {
        chr = codec::gbk2unicode(chr);

        if (stralgo::iswprint(chr))
            item(current()).edit(std::wstring(1, chr));
        else
            item(current()).edit();
    }

    return true;
}

bool table::onMouseDown(sys::Button button, int times, Point point)
{
    item_index index = hittest(point);
    if (!impl_->exists(index))
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
        || ((edit_trigger() & etSelectedClicked) && index == current()))
        && item(index).editable())
    {
        item(index).edit();
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

bool table::onMouseUp(sys::Button button, Point point)
{
    capture(false);

    return true;
}

bool table::onMouseMove(Point point)
{
    item_index index = hittest(point), oldcurrent = current();
    if (!impl_->exists(index))
        return true;

    if (impl_->enter_ != index)
    {
        if (impl_->exists(impl_->enter_))
            onItemMouseLeaveHandle(impl_->enter_);
        impl_->enter_ = index;
        onItemMouseEnterHandle(impl_->enter_);
    }

    if (!sys::key_pressed(sys::vkLButton) || (index == oldcurrent))
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

Rect table::item::rect(void) const
{
    return impl->host.impl_->item_rect(impl->index);
}

const std::wstring& table::item::text(void) const
{
    return impl->item.text.content();
}

Margin table::item::margin(void) const
{
    return impl->item.text.margin();
}

unsigned int table::item::indent(void) const
{
    return impl->item.text.indent();
}

unsigned int table::item::alignment(void) const
{
    return impl->item.text.alignment();
}

bool table::item::multiline(void) const
{
    return impl->item.text.multiline();
}

bool table::item::word_wrap(void) const
{
    return impl->item.text.word_wrap();
}

unsigned int table::item::line_spacing(void) const
{
    return impl->item.text.line_spacing();
}

unsigned int table::item::word_spacing(void) const
{
    return impl->item.text.word_spacing();
}

table::fontptr table::item::font(void) const
{
    return impl->item.text.font();
}

brush_sptr table::item::text_brush(void) const
{
    return impl->item.fgbrush;
}

brush_sptr table::item::background_brush(void) const
{
    return impl->item.bgbrush;
}

brush_sptr table::item::selection_brush(void) const
{
    return impl->item.selbrush;
}

bool table::item::editable(void) const
{
    return impl->item.editable;
}

void table::item::text(const std::string& _text)
{
    text(_text, sys::default_codec());
}

void table::item::text(const std::string& _text, const std::string& codec)
{
    text(stralgo::decode(_text, codec));
}

void table::item::text(const std::wstring& text)
{
    if (impl->item.text.content() != text)
    {
        impl->item.text.content(text);
        impl->host.onItemTextChangedHandle(impl->index);
        impl->repaint();
    }
}

void table::item::margin(int margin)
{
    impl->item.text.margin(margin);
    impl->repaint();
}

void table::item::margin(int left, int top, int right, int bottom)
{
    impl->item.text.margin(left, top, right, bottom);
    impl->repaint();
}

void table::item::margin_left(int left)
{
    impl->item.text.margin_left(left);
    impl->repaint();
}

void table::item::margin_top(int top)
{
    impl->item.text.margin_top(top);
    impl->repaint();
}

void table::item::margin_right(int right)
{
    impl->item.text.margin_right(right);
    impl->repaint();
}

void table::item::margin_bottom(int bottom)
{
    impl->item.text.margin_bottom(bottom);
    impl->repaint();
}

void table::item::margin(const Margin& margin)
{
    impl->item.text.margin(margin.left, margin.top, margin.right, margin.bottom);
    impl->repaint();
}

void table::item::indent(unsigned int indent)
{
    impl->item.text.indent(indent);
    impl->repaint();
}

void table::item::alignment(unsigned int alignment)
{
    impl->item.text.alignment(alignment);
    impl->repaint();
}

void table::item::multiline(bool multiline)
{
    impl->item.text.multiline(multiline);
    impl->repaint();
}

void table::item::word_wrap(bool word_wrap)
{
    impl->item.text.word_wrap(word_wrap);
    impl->repaint();
}

void table::item::line_spacing(unsigned int line_spacing)
{
    impl->item.text.line_spacing(line_spacing);
    impl->repaint();
}

void table::item::word_spacing(unsigned int word_spacing)
{
    impl->item.text.word_spacing(word_spacing);
    impl->repaint();
}

void table::item::font(fontptr font)
{
    impl->item.text.font(font);
    impl->repaint();
}

void table::item::text_color(const color& color)
{
    text_brush(make_color_brush_sptr(color));
}

void table::item::text_brush(const brush_sptr& brush)
{
    impl->item.fgbrush = brush;
    impl->repaint();
}

void table::item::background_color(const color& color)
{
    background_brush(make_color_brush_sptr(color));
}

void table::item::background_brush(const brush_sptr& brush)
{
    impl->item.bgbrush = brush;
    impl->repaint();
}

void table::item::selection_color(const color& color)
{
    selection_brush(make_color_brush_sptr(color));
}

void table::item::selection_brush(const brush_sptr& brush)
{
    impl->item.selbrush = brush;
    impl->repaint();
}

void table::item::editable(bool editable)
{
    impl->item.editable = editable;
}

void table::item::edit(void)
{
    edit(L"");
}

void table::item::edit(const std::string& text)
{
    edit(text, sys::default_codec());
}

void table::item::edit(const std::string& text, const std::string& codec)
{
    edit(stralgo::decode(text, codec));
}

void table::item::edit(const std::wstring& text)
{
    impl->host.scrollto(impl->index);

    std::shared_ptr<item> self(new item(*this));
    edit_box& eb = edit_box::pop(&impl->host, rect(), impl->item.text, text);

    handler::sptr handler = impl->host.horizontal_scroll().onScroll(
                            impl->host.vertical_scroll().onScroll(
        [self, &eb](scroll*)
        {
            eb.xy(self->rect().left_top());
        }));

    eb.background_color(color::white());
    eb.onEditComplete(
        [self](edit_box*, const std::wstring& txt)
        {
            self->text(txt);
        });
    eb.onEditFinish(
        [self, handler](edit_box*)
        {
            self->impl->host.horizontal_scroll().onScrollRemove(handler);
            self->impl->host.vertical_scroll().onScrollRemove(handler);
        });
}

unsigned int table::gridline::width(void) const
{
    return math::ceil(impl->gridline.pen.width());
}

brush_sptr table::gridline::brush(void) const
{
    return impl->gridline.pen.brush();
}

std::vector<double> table::gridline::dash_array(void) const
{
    std::vector<double> dash_array(impl->gridline.pen.dash_array_size());

    for (unsigned int i = 0; i < dash_array.size(); ++i)
        dash_array[i] = impl->gridline.pen.dash(i);

    return dash_array;
}

void table::gridline::width(unsigned int width)
{
    impl->gridline.pen.width(width);
    impl->host.impl_->update();
}

void table::gridline::color(const class color& color)
{
    brush(make_color_brush_sptr(color));
}

void table::gridline::brush(brush_sptr brush)
{
    impl->gridline.pen.brush(brush);
    impl->host.impl_->update();
}

void table::gridline::dash_array(const std::vector<double>& _dash_array)
{
    dash_array(_dash_array.data(), _dash_array.size());
}

void table::gridline::dash_array(std::initializer_list<double> initlist)
{
    dash_array(std::vector<double>(initlist.begin(), initlist.end()));
}

void table::gridline::dash_array(const double* dash_array, unsigned int count)
{
    impl->gridline.pen.dash_array(dash_array, count);
    impl->host.impl_->update();
}

unsigned int table::row_header::height(void) const
{
    return impl->header.height;
}

void table::row_header::height(unsigned int height)
{
    if (impl->header.height != height)
    {
        impl->header.height = height;
        impl->host.impl_->update();
    }
}

unsigned int table::column_header::width(void) const
{
    return impl->header.width;
}

void table::column_header::width(unsigned int width)
{
    if (impl->header.width != width)
    {
        impl->header.width = width;
        impl->host.impl_->update();
    }
}
