#include <algorithm>
#include <takisy/core/sys.h>
#include <takisy/core/math.h>
#include <takisy/core/codec.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/gui/basic/cursor.h>
#include <takisy/gui/widget/table.h>
#include "editbox.h"

class table::implement
{
    friend class table;

    struct brush
    {
        brush_sptr text, background, selection;
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

    struct item
    {
        text text;
        bool editable;
        std::shared_ptr<brush> brush;

    public:
        item(void)
            : editable(false)
        {
            text.margin(2);
            text.alignment(aCenter);
            text.word_wrap(false);
        }
    };

    struct row_header : public item
    {
        bool scalable;
        unsigned int height;

    public:
        row_header(void)
            : scalable(true), height(18)
        {}
    };

    struct row_headers
    {
        brush brush;
        bool visible;
        bool scalable;
        unsigned int width;

    public:
        row_headers(void)
            : visible(false), scalable(true), width(32)
        {
            brush.background.reset(new color_brush(color::light_gray()));
        }
    };

    struct column_header : public item
    {
        bool scalable;
        unsigned int width;

    public:
        column_header(void)
            : scalable(true), width(80)
        {}
    };

    struct column_headers
    {
        brush brush;
        bool visible;
        bool scalable;
        unsigned int height;

    public:
        column_headers(void)
            : visible(true), scalable(true), height(18)
        {
            brush.background.reset(new color_brush(color::light_gray()));
        }
    };

    struct row_type
    {
        brush brush;
        gridline gridline;
        row_header header;
        std::vector<item> items;

    public:
        unsigned int height(void) const
        {
            return header.height + gridline.width();
        }
    };

    struct column_type
    {
        brush brush;
        gridline gridline;
        column_header header;

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
        , dragging_({.underway = false})
    {}

public:
    int glrow_hittest(Point point)
    {
        point.x += self.horizontal_scroll().valued();
        point.y += self.vertical_scroll().valued();

        if (!row_headers_.visible
            || !row_headers_.scalable
            || point.x > (int)row_headers_.width)
            return -2;

        int offset = 0;

        if (column_headers_.visible)
        {
            if (glhittest(point.y, column_headers_.height,
                          rows_.empty() ? 0 : rows_.front().gridline.width()))
                return -1;
            offset += column_headers_.height;
        }

        for (unsigned int i = 0; i < rows_.size(); ++i)
        {
            offset += rows_[i].gridline.width() + rows_[i].header.height;
            if (rows_[i].header.scalable &&
                glhittest(point.y, offset,
                          i + 1 >= rows_.size() ? 0
                                 : rows_[i + 1].gridline.width()))
                return i;
        }

        return -2;
    }

    int glcol_hittest(Point point)
    {
        point.x += self.horizontal_scroll().valued();
        point.y += self.vertical_scroll().valued();

        if (!column_headers_.visible
            || !column_headers_.scalable
            || point.y > (int)column_headers_.height)
            return -2;

        int offset = 0;

        if (row_headers_.visible)
        {
            if (glhittest(point.x, row_headers_.width,
                          columns_.empty() ? 0
                                           : columns_.front().gridline.width()))
                return -1;
            offset += row_headers_.width;
        }

        for (unsigned int i = 0; i < columns_.size(); ++i)
        {
            offset += columns_[i].gridline.width() + columns_[i].header.width;
            if (columns_[i].header.scalable &&
                glhittest(point.x, offset,
                          i + 1 >= columns_.size() ? 0
                                 : columns_[i + 1].gridline.width()))
                return i;
        }

        return -2;
    }

    Size optimal_world(text& text)
    {
        if (text.word_wrap())
        {
            class text cptext = text;
            cptext.word_wrap(false);
            return cptext.world();
        }

        return text.world();
    }

public:
    unsigned int vaccumulate(unsigned int row) const
    {
        unsigned int border_width = border_.width();
        unsigned int height = std::accumulate(
            rows_.begin(), rows_.begin() + row, border_width,
            [](unsigned int height, const row_type& other)
            {
                return height + other.height();
            });

        if (column_headers_.visible)
            height += column_headers_.height;
        if (row == rows_.size())
            height += border_width;

        return height;
    }

    unsigned int haccumulate(unsigned int col) const
    {
        unsigned int border_width = border_.width();
        unsigned int width = std::accumulate(
            columns_.begin(), columns_.begin() + col, border_width,
            [](unsigned int width, const column_type& other)
            {
                return width + other.width();
            });

        if (row_headers_.visible)
            width += row_headers_.width;
        if (col == columns_.size())
            width += border_width;

        return width;
    }

    Rect item_rect(const index& index) const
    {
        Rect rect;

        if (index.row == -2)
        {
            rect.top    = 0;
            rect.bottom = self.vertical_scroll().max();
        }
        else if (index.row == -1)
        {
            rect.top    = border_.width();
            rect.bottom = column_headers_.height;
        }
        else if (static_cast<unsigned int>(index.row) < rows_.size())
        {
            const row_type& row = rows_[index.row];

            rect.top    = vaccumulate(index.row) + row.gridline.width();
            rect.bottom = rect.top + row.header.height;
        }

        if (index.col == -2)
        {
            rect.left   = 0;
            rect.right  = self.horizontal_scroll().max();
        }
        else if (index.col == -1)
        {
            rect.left   = border_.width();
            rect.right  = row_headers_.width;
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

    Rect items_rect(const std::vector<index>& indexes)
    {
        if (indexes.empty())
            return Rect();

        index min = indexes.front(), max = min;

        for (const index& index : indexes)
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

    void repaint_selections(const std::vector<index>& indexes)
    {
        std::vector<index> real_indexes;

        switch (selbehavior_)
        {
        case sbSelectItems:
            real_indexes = indexes;
            break;
        case sbSelectRows:
            {
                std::set<index> rowset;
                for (const struct index& index : indexes)
                    rowset.insert({ index.row, -2 });
                real_indexes.assign(rowset.begin(), rowset.end());
            }
            break;
        case sbSelectColumns:
            {
                std::set<index> columnset;
                for (const struct index& index : indexes)
                    columnset.insert({ -2, index.col });
                real_indexes.assign(columnset.begin(), columnset.end());
            }
            break;
        }

        self.repaint(items_rect(real_indexes));
    }

public:
    void update(void)
    {
        self.vertical_scroll()
            .max(vaccumulate(rows_.size()) + self.horizontal_scroll().height());
        self.horizontal_scroll()
            .max(haccumulate(columns_.size()) + self.vertical_scroll().width());
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

public:
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

        if (row_headers_.visible)
            dx += row_headers_.width;
        if (column_headers_.visible)
            dy += column_headers_.height;

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

    void draw_header(graphics& graphics, brush_sptr txtbrush, int x, int y)
    {
        column_headers& colhds = column_headers_;
        Rect rect(x, y, 0, y + colhds.height);

        for (column_type& column : columns_)
        {
            rect.left += column.gridline.width();
            rect.width(column.header.width);

            column_header& header = column.header;
            if (header.brush && header.brush->background)
                graphics.fill_rectangle(rect, *header.brush->background);
            else if (colhds.brush.background)
                graphics.fill_rectangle(rect, *colhds.brush.background);

            text_rect(header.text, rect);
            if (header.brush && header.brush->text)
                header.text.draw(graphics, rect, nullptr, *header.brush->text);
            else if (colhds.brush.text)
                header.text.draw(graphics, rect, nullptr, *colhds.brush.text);
            else
                header.text.draw(graphics, rect, nullptr, *txtbrush);

            rect.left += rect.width();
        }
    }

    int draw_row(graphics& graphics,
                 const brush_sptr& txtbrush,
                 const brush_sptr& selbrush, int x, int y, int index)
    {
        row_type& row = rows_[index];
        Rect rect;

        rect.left   = x;
        rect.top    = y + row.gridline.width();
        rect.bottom = rect.top + row.header.height;

        if (row_headers_.visible)
        {
            row_headers& rowhds = row_headers_;
            row_header&  header = row.header;

            rect.width(rowhds.width);
            if (header.brush && header.brush->background)
                graphics.fill_rectangle(rect, *header.brush->background);
            else if (rowhds.brush.background)
                graphics.fill_rectangle(rect, *rowhds.brush.background);

            text_rect(header.text, rect);
            if (header.brush && header.brush->text)
                header.text.draw(graphics, rect, nullptr, *header.brush->text);
            else if (rowhds.brush.text)
                header.text.draw(graphics, rect, nullptr, *rowhds.brush.text);
            else
                header.text.draw(graphics, rect, nullptr, *txtbrush);

            rect.left += rect.width();
        }

        for (unsigned int i = 0; i < columns_.size(); ++i)
        {
            column_type& column = columns_[i];
            item& item = row.items[i];

            rect.left += column.gridline.width();
            rect.width(column.header.width);

            if (item.brush && item.brush->background)
                graphics.fill_rectangle(rect, *item.brush->background);
            else if (row.brush.background)
                graphics.fill_rectangle(rect, *row.brush.background);
            else if (column.brush.background)
                graphics.fill_rectangle(rect, *column.brush.background);
            else if (brush_.background)
                graphics.fill_rectangle(rect, *brush_.background);

            if (self.selected({index, (int)i}))
            {
                if (item.brush && item.brush->selection)
                    graphics.fill_rectangle(rect, *item.brush->selection);
                else if (row.brush.selection)
                    graphics.fill_rectangle(rect, *row.brush.selection);
                else if (column.brush.selection)
                    graphics.fill_rectangle(rect, *column.brush.selection);
                else if (brush_.selection)
                    graphics.fill_rectangle(rect, *brush_.selection);
                else
                    graphics.fill_rectangle(rect, *selbrush);
            }

            text_rect(item.text, rect);
            if (item.brush && item.brush->text)
                item.text.draw(graphics, rect, nullptr, *item.brush->text);
            else if (row.brush.text)
                item.text.draw(graphics, rect, nullptr, *row.brush.text);
            else if (column.brush.text)
                item.text.draw(graphics, rect, nullptr, *column.brush.text);
            else if (brush_.text)
                item.text.draw(graphics, rect, nullptr, *brush_.text);
            else
                item.text.draw(graphics, rect, nullptr, *txtbrush);

            rect.left += rect.width();
        }

        return rect.bottom;
    }

private:
    bool glhittest(int w, int offset, int glwidth)
    {
        return offset <= w && w <= offset + glwidth + 1;
    }

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
    table&                   self;
    brush                    brush_;
    gridline                 border_;
    std::vector<row_type>    rows_;
    std::vector<column_type> columns_;
    row_headers              row_headers_;
    column_headers           column_headers_;
    SelectionMode            selmode_;
    SelectionBehavior        selbehavior_;
    unsigned int             edit_trigger_;
    index                    selbegin_, current_, enter_;
    std::set<index>          selecteds_;
    struct {
        bool underway;
        struct {
            Point point;
            int glrow, glcol, size;
        } milestone;
    } dragging_;
};

bool table::index::operator==(const index& other) const
{
    return row == other.row && col == other.col;
}

bool table::index::operator!=(const index& other) const
{
    return !operator==(other);
}

bool table::index::operator<(const index& other) const
{
    return row < other.row || (row == other.row && col < other.col);
}

struct table::item::implement
{
    table& host;
    table::implement::item& item;
    index index;

public:
    implement(table& host, table::implement::item& item,
              const struct index& index)
        : host(host), item(item), index(index)
    {}

public:
    void repaint(void)
    {
        host.repaint_item(index);
    }
};

struct table::gridline::implement
{
    table& host;
    table::implement::gridline& gridline;

public:
    implement(table& host, table::implement::gridline& gridline)
        : host(host), gridline(gridline)
    {}
};

struct table::row_headers::implement
{
    table& host;
    table::implement::row_headers& headers;

public:
    implement(table& host, table::implement::row_headers& headers)
        : host(host), headers(headers)
    {}
};

struct table::row_header::implement
{
    table& host;
    table::implement::row_header& header;

public:
    implement(table& host, table::implement::row_header& header)
        : host(host), header(header)
    {}
};

struct table::row::implement
{
    table& host;
    table::implement::row_type& row;
    int index;

public:
    implement(table& host, table::implement::row_type& row, int index)
        : host(host), row(row), index(index)
    {}

public:
    void repaint(void)
    {
        host.repaint_item({index, -2});
    }
};

struct table::column_headers::implement
{
    table& host;
    table::implement::column_headers& headers;

public:
    implement(table& host, table::implement::column_headers& headers)
        : host(host), headers(headers)
    {}
};

struct table::column_header::implement
{
    table& host;
    table::implement::column_header& header;

public:
    implement(table& host, table::implement::column_header& header)
        : host(host), header(header)
    {}
};

struct table::column::implement
{
    table& host;
    table::implement::column_type& column;
    int index;

public:
    implement(table& host, table::implement::column_type& column, int index)
        : host(host), column(column), index(index)
    {}

public:
    void repaint(void)
    {
        host.repaint_item({-2, index});
    }
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

table::index table::current(void) const
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

bool table::selected(const index& index) const
{
    switch (impl_->selbehavior_)
    {
    default:
    case sbSelectItems:
        return impl_->selecteds_.find(index) != impl_->selecteds_.end();
    case sbSelectRows:
        for (const struct index& i : impl_->selecteds_)
            if (i.row == index.row)
                return true;
        return false;
    case sbSelectColumns:
        for (const struct index& i : impl_->selecteds_)
            if (i.col == index.col)
                return true;
        return false;
    }
}

std::set<table::index> table::selected(void) const
{
    return impl_->selecteds_;
}

bool table::exists(const index& index) const
{
    return static_cast<unsigned int>(index.row) < impl_->rows_.size()
        && static_cast<unsigned int>(index.col) < impl_->columns_.size();
}

const struct table::item table::item(const index& index) const
{
    table& self = const_cast<table&>(*this);
    implement::item& item = impl_->rows_[index.row].items[index.col];
    struct item ret;

    ret.impl.reset(new item::implement(self, item, index));

    return ret;
}

const struct table::gridline table::border(void) const
{
    table& self = const_cast<table&>(*this);
    struct gridline ret;

    ret.impl.reset(new gridline::implement(self, impl_->border_));

    return ret;
}

const struct table::row_headers table::row_headers(void) const
{
    table& self = const_cast<table&>(*this);
    implement::row_headers& headers = impl_->row_headers_;
    struct row_headers ret;

    ret.impl.reset(new row_headers::implement(self, headers));

    return ret;
}

const struct table::row table::row(unsigned index) const
{
    table& self = const_cast<table&>(*this);
    implement::row_type& row = impl_->rows_[index];
    struct row ret;

    ret.impl.reset(new row::implement(self, row, index));

    return ret;
}

const struct table::column table::column(unsigned index) const
{
    table& self = const_cast<table&>(*this);
    implement::column_type& column = impl_->columns_[index];
    struct column ret;

    ret.impl.reset(new column::implement(self, column, index));

    return ret;
}

const struct table::column_headers table::column_headers(void) const
{
    table& self = const_cast<table&>(*this);
    implement::column_headers& headers = impl_->column_headers_;
    struct column_headers ret;

    ret.impl.reset(new column_headers::implement(self, headers));

    return ret;
}

struct table::item table::item(const index& index)
{
    implement::item& item = impl_->rows_[index.row].items[index.col];
    struct item ret;

    ret.impl.reset(new item::implement(*this, item, index));

    return ret;
}

struct table::gridline table::border(void)
{
    struct gridline ret;
    ret.impl.reset(new gridline::implement(*this, impl_->border_));
    return ret;
}

struct table::row_headers table::row_headers(void)
{
    implement::row_headers& headers = impl_->row_headers_;
    struct row_headers ret;

    ret.impl.reset(new row_headers::implement(*this, headers));

    return ret;
}

struct table::column_headers table::column_headers(void)
{
    implement::column_headers& headers = impl_->column_headers_;
    struct column_headers ret;

    ret.impl.reset(new column_headers::implement(*this, headers));

    return ret;
}

struct table::row table::row(unsigned int index)
{
    implement::row_type& row = impl_->rows_[index];
    struct row ret;

    ret.impl.reset(new row::implement(*this, row, index));

    return ret;
}

struct table::column table::column(unsigned int index)
{
    implement::column_type& column = impl_->columns_[index];
    struct column ret;

    ret.impl.reset(new column::implement(*this, column, index));

    return ret;
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
    insert_row(row, stralgo::strf(row + 1));
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

    std::vector<index> newselecteds;
    for (const index& index : impl_->selecteds_)
        if (index.row < (int)row)
            newselecteds.push_back(index);
        else
            newselecteds.push_back({ index.row + 1, index.col });
    impl_->selecteds_.clear();
    for (const index& index : newselecteds)
        impl_->selecteds_.insert(index);

    impl_->update();
}

void table::remove_row(unsigned int row)
{
    if (row < rows())
    {
        impl_->rows_.erase(impl_->rows_.begin() + row);

        std::vector<index> newselecteds;
        for (const index& index : impl_->selecteds_)
            if (index.row < (int)row)
                newselecteds.push_back(index);
            else if (index.row > (int)row)
                newselecteds.push_back({ index.row - 1, index.col });
        impl_->selecteds_.clear();
        for (const index& index : newselecteds)
            impl_->selecteds_.insert(index);

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
    insert_column(col, stralgo::strf(col + 1));
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

    std::vector<index> newselecteds;
    for (const index& index : impl_->selecteds_)
        if (index.col < (int)col)
            newselecteds.push_back(index);
        else
            newselecteds.push_back({ index.row, index.col + 1 });
    impl_->selecteds_.clear();
    for (const index& index : newselecteds)
        impl_->selecteds_.insert(index);

    impl_->update();
}

void table::remove_column(unsigned int col)
{
    if (col < columns())
    {
        impl_->columns_.erase(impl_->columns_.begin() + col);

        for (implement::row_type& row : impl_->rows_)
            row.items.erase(row.items.begin() + col);

        std::vector<index> newselecteds;
        for (const index& index : impl_->selecteds_)
            if (index.col < (int)col)
                newselecteds.push_back(index);
            else if (index.col > (int)col)
                newselecteds.push_back({ index.row, index.col - 1 });
        impl_->selecteds_.clear();
        for (const index& index : newselecteds)
            impl_->selecteds_.insert(index);

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
    clear_selection();
    impl_->rows_.clear();
    impl_->update();
}

void table::clear_content(void)
{
    for (implement::row_type& row : impl_->rows_)
    for (struct implement::item& item : row.items)
        item.text.content(std::wstring());

    repaint();
}

void table::clear_row(unsigned int row)
{
    if (row < rows())
    {
        for (struct implement::item& item : impl_->rows_[row].items)
            item.text.content(std::wstring());

        repaint(impl_->item_rect({(int)row, -2}));
    }
}

void table::clear_column(unsigned int col)
{
    if (col < columns())
    {
        for (implement::row_type& row : impl_->rows_)
            row.items[col].text.content(std::wstring());

        repaint(impl_->item_rect({-2, (int)col}));
    }
}

void table::current(const index& index)
{
    if (exists(index) && index != impl_->current_)
    {
        struct index oldcurrent = impl_->current_;

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

void table::selected(const index& index, bool _selected)
{
    selected(index, index, _selected);
}

void table::selected(const index& _begin, const index& _end, bool _selected)
{
    index begin = _begin, end = _end;

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

    std::set<index> inserting;
    for (int row = begin.row; row <= end.row; ++row)
    for (int col = begin.col; col <= end.col; ++col)
        inserting.insert({row, col});

    selected(inserting, _selected);
}

void table::selected(const std::set<index>& indexes, bool selected)
{
    std::vector<index> result;
    using namespace std;

    if (selected)
    {
        set_difference(indexes.begin(), indexes.end(),
                       impl_->selecteds_.begin(), impl_->selecteds_.end(),
                       inserter(result, result.begin()));

        for (const index& index : result)
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

        switch (impl_->selbehavior_)
        {
        default:
        case sbSelectItems:
            break;
        case sbSelectRows:
            {
                std::set<int> rows;
                result.assign(indexes.begin(), indexes.end());
                for (const index& index : result)
                    rows.insert(index.row);
                result.clear();
                for (int row : rows)
                for (const index& index : impl_->selecteds_)
                    if (row == index.row)
                        result.push_back(index);
            }
            break;
        case sbSelectColumns:
            {
                std::set<int> cols;
                result.assign(indexes.begin(), indexes.end());
                for (const index& index : result)
                    cols.insert(index.col);
                result.clear();
                for (int col : cols)
                for (const index& index : impl_->selecteds_)
                    if (col == index.col)
                        result.push_back(index);
            }
            break;
        }

        for (const index& index : result)
        {
            impl_->selecteds_.erase(index);
            onItemDisselectedHandle(index);
            onItemSelectionChangedHandle(index);
        }
    }

    if (!result.empty())
    {
        onSelectionChangedHandle(result, selected);

        impl_->repaint_selections(result);
    }

}

void table::select(const index& index)
{
    selected(index, true);
}

void table::select(const index& begin, const index& end)
{
    selected(begin, end, true);
}

void table::select(const std::set<index>& indexes)
{
    selected(indexes, true);
}

void table::disselect(const index& index)
{
    selected(index, false);
}

void table::disselect(const index& begin, const index& end)
{
    selected(begin, end, false);
}

void table::disselect(const std::set<index>& indexes)
{
    selected(indexes, false);
}

void table::clear_selection(void)
{
    disselect(impl_->selecteds_);
}

void table::text_color(color color)
{
    text_brush(make_spbrush<color_brush>(color));
}

void table::text_brush(const brush_sptr& brush)
{
    impl_->brush_.text = brush;

    for (implement::row_type& row : impl_->rows_)
    {
        row.brush.text.reset();

        for (implement::item& item : row.items)
            if (item.brush)
                item.brush->text.reset();
    }

    for (implement::column_type& column : impl_->columns_)
        column.brush.text.reset();

    repaint();
}

void table::background_color(color color)
{
    background_brush(make_spbrush<color_brush>(color));
}

void table::background_brush(const brush_sptr& brush)
{
    impl_->brush_.background = brush;

    for (implement::row_type& row : impl_->rows_)
    {
        row.brush.background.reset();

        for (implement::item& item : row.items)
            if (item.brush)
                item.brush->background.reset();
    }

    for (implement::column_type& column : impl_->columns_)
        column.brush.background.reset();

    repaint();
}

void table::selection_color(color color)
{
    selection_brush(make_spbrush<color_brush>(color));
}

void table::selection_brush(const brush_sptr& brush)
{
    impl_->brush_.selection = brush;

    for (implement::row_type& row : impl_->rows_)
    {
        row.brush.selection.reset();

        for (implement::item& item : row.items)
            if (item.brush)
                item.brush->selection.reset();
    }

    for (implement::column_type& column : impl_->columns_)
        column.brush.selection.reset();

    repaint();
}

table::index table::hittest(Point point) const
{
    unsigned int border_width = impl_->border_.width();
    index index = {-1, -1};

    point.y += vertical_scroll().valued() - border_width;
    if (point.y < 0)
        index.row = rows();
    else if (!impl_->column_headers_.visible
             || point.y >= (int)impl_->column_headers_.height)
    {
        if (impl_->column_headers_.visible)
            point.y -= impl_->column_headers_.height;
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
    else if (!impl_->row_headers_.visible
             || point.x >= (int)impl_->row_headers_.width)
    {
        if (impl_->row_headers_.visible)
            point.x -= impl_->row_headers_.width;
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

void table::scrollto(const index& index)
{
    scrollto_row(index.row, aUndefined);
    scrollto_column(index.col, aUndefined);
}

void table::scrollto(const index& index, unsigned int alignment)
{
    scrollto_row(index.row, alignment);
    scrollto_column(index.col, alignment);
}

void table::scrollto_row(unsigned int row)
{
    scrollto_row(row, aUndefined);
}

void table::scrollto_row(unsigned int row, unsigned int alignment)
{
    if (row >= rows())
        row  = rows() - 1;

    int row_height = impl_->rows_[row].header.height;
    int top    = impl_->vaccumulate(row);
    int middle = top + (height() - row_height) / 2;
    int bottom = top + row_height - height();

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

void table::scrollto_column(unsigned int col)
{
    scrollto_column(col, aUndefined);
}

void table::scrollto_column(unsigned int col, unsigned int alignment)
{
    if (col >= columns())
        col  = columns() - 1;

    int column_width = impl_->columns_[col].header.width;
    int left   = impl_->haccumulate(col);
    int middle = left - (width() - column_width) / 2;
    int right  = left + column_width - width();

    if (alignment == aUndefined)
    {
        if (left < horizontal_scroll().valued())
            horizontal_scroll().value(left);
        else if (right > horizontal_scroll().valued())
            horizontal_scroll().value(right);
    }
    else
    {
        switch (alignment & aHorizontal)
        {
        case aLeft:
            horizontal_scroll().value(left);
            break;
        case aCenter:
            horizontal_scroll().value(middle);
            break;
        case aRight:
            horizontal_scroll().value(right);
            break;
        }
    }
}

void table::repaint_item(const index& index)
{
    repaint(impl_->item_rect(index));
}

void table::sort(int col)
{
    sort(col, sort_handler());
}

void table::sort(int i, const sort_handler& h)
{
    if (i == -1)
        std::sort(impl_->rows_.begin(), impl_->rows_.end(),
            [&](const implement::row_type& a, const implement::row_type& b) {
                return h(a.header.text.content(), b.header.text.content());
            });
    else
        std::sort(impl_->rows_.begin(), impl_->rows_.end(),
            [&](const implement::row_type& a, const implement::row_type& b) {
                return h(a.items[i].text.content(), b.items[i].text.content());
            });

    repaint();
}

void table::onSize(void)
{
    scroll_area::onSize();
}

void table::onPaint(graphics graphics, Rect rect)
{
    class color_scheme cs = color_scheme();
    color selcolor = focused() ? cs.selection() : cs.inactive_selection();
    brush_sptr txtbrush = make_spbrush<color_brush>(cs.text());
    brush_sptr selbrush = make_spbrush<color_brush>(selcolor);
    int x = impl_->border_.width() - horizontal_scroll().valued();
    int y = impl_->border_.width() - vertical_scroll().valued();

    impl_->draw_lines(graphics, rect);

    if (impl_->column_headers_.visible)
    {
        int dx = x;
        if (impl_->row_headers_.visible)
            dx += impl_->row_headers_.width;
        if (y + (int)impl_->column_headers_.height > rect.top)
            impl_->draw_header(graphics, txtbrush, dx, y);
        y += impl_->column_headers_.height;
    }

    for (unsigned int row = 0; row < rows() && y < rect.bottom; ++row)
    {
        int rowheight = impl_->rows_[row].header.height;

        if (y + rowheight > rect.top)
            y = impl_->draw_row(graphics, txtbrush, selbrush, x, y, row);
        else
            y += impl_->rows_[row].gridline.width() + rowheight;

        if (y >= rect.bottom)
            break;
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
    std::vector<index> sels(impl_->selecteds_.begin(), impl_->selecteds_.end());
    impl_->repaint_selections(sels);
    return true;
}

bool table::onSetCursor(Point point)
{
    int glrow = -2, glcol = -2;
    if (impl_->dragging_.underway) {
        glrow = impl_->dragging_.milestone.glrow;
        glcol = impl_->dragging_.milestone.glcol;
    } else {
        glcol = impl_->glcol_hittest(point);
        if (glcol == -2)
            glrow = impl_->glrow_hittest(point);
    }

    if (glcol > -2)
        cursor::set(cursor::ctSizeWE);
    else if (glrow > -2)
        cursor::set(cursor::ctSizeNS);
    else
        cursor::set(cursor::ctArrow);

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
            index index = current();
            if (vkey == sys::vkUp)
                --index.row;
            else if (vkey == sys::vkDown)
                ++index.row;
            else if (vkey == sys::vkLeft)
                --index.col;
            else
                ++index.col;

            if (!exists(index))
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
                    bool ctrl  = window::key_pressed(sys::vkControl);
                    bool shift = window::key_pressed(sys::vkShift);

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
                if (window::key_pressed(sys::vkShift))
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
            if (!window::key_pressed(sys::vkControl))
            {
                clear_selection();
                select(current());
            }
            else
                selected(current(), !selected(current()));
        }
        break;
    case sys::vkKeyA:
        if (window::key_pressed(sys::vkControl)
            && (impl_->selmode_ == smMultiSelection
             || impl_->selmode_ == smExtendedSelection
             || impl_->selmode_ == smContiguousSelection))
            select({0, 0}, {(int)rows(), (int)columns()});
        break;
    case sys::vkKeyI:
        if (window::key_pressed(sys::vkControl)
            && (impl_->selmode_ == smMultiSelection
             || impl_->selmode_ == smExtendedSelection
             || impl_->selmode_ == smContiguousSelection))
        {
            switch (impl_->selbehavior_)
            {
            default:
            case sbSelectItems:
                {
                    std::set<index> oldsel = impl_->selecteds_;
                    selected({0, 0}, {(int)rows(), (int)columns()}, true);
                    selected(oldsel, false);
                }
                break;
            case sbSelectRows:
                for (int limit = rows(), i = 0; i < limit; ++i)
                    selected(index {i, 0}, !selected(index {i, 0}));
                break;
            case sbSelectColumns:
                for (int limit = columns(), i = 0; i < limit; ++i)
                    selected(index {0, i}, !selected(index {0, i}));
                break;
            }
        }
        break;
    case sys::vkHome:
        vertical_scroll().home();
        break;
    case sys::vkEnd:
        vertical_scroll().end();
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

bool table::onKeyPress(unsigned int chr)
{
    if (!exists(current()) || !item(current()).editable())
        return true;

    if (    (edit_trigger() & etAnyKeyPressed)
        || ((edit_trigger() & etEnterPressed) && chr == 13))
    {
        chr = codec::gbk2uni(chr);

        if (stralgo::iswprint(chr))
            item(current()).edit(std::wstring(1, chr));
        else
            item(current()).edit();
    }

    return true;
}

bool table::onMouseDown(sys::Button button, int times, Point point)
{
    if (button == sys::btnLeft) {
        int glcol = impl_->glcol_hittest(point);
        if (glcol > -2) {
            impl_->dragging_.underway = true;
            if (glcol >= 0 && times == 2)
                column(glcol).width(column(glcol).optimal_width());
            else {
                capture(true);
                impl_->dragging_.milestone.point = point;
                impl_->dragging_.milestone.glrow = -2;
                impl_->dragging_.milestone.glcol = glcol;
                impl_->dragging_.milestone.size  = glcol == -1
                     ? impl_->row_headers_.width
                     : impl_->columns_[glcol].header.width;
            }
            return true;
        } else {
            int glrow = impl_->glrow_hittest(point);
            if (glrow > -2) {
                impl_->dragging_.underway = true;
                if (glrow >= 0 && times == 2)
                    row(glrow).height(row(glrow).optimal_height());
                else {
                    capture(true);
                    impl_->dragging_.milestone.point = point;
                    impl_->dragging_.milestone.glrow = glrow;
                    impl_->dragging_.milestone.glcol = -2;
                    impl_->dragging_.milestone.size  = glrow == -1
                         ? impl_->column_headers_.height
                         : impl_->rows_[glrow].header.height;
                }
                return true;
            }
        }
    }

    index index = hittest(point);
    if (!exists(index))
    {
        if (times == 2 && (index.row == -1 || index.col == -1))
        {
            if (0 <= index.row && index.row < (int)impl_->rows_.size())
                onRowHeaderDoubleClickedHandle(index.row, button);
            else if (0 <= index.col && index.col < (int)impl_->columns_.size())
                onColumnHeaderDoubleClickedHandle(index.col, button);
        }

        return true;
    }
    else if (times == 2)
    {
        if (button == sys::btnLeft
            && item(index).editable()
            && edit_trigger() & etDoubleClicked)
            item(index).edit();
        else
            onItemDoubleClickedHandle(index, button);

        return true;
    }

    if (button != sys::btnLeft)
        return true;
    else
    if (index == current()
        && item(index).editable()
        && edit_trigger() & etSelectedClicked)
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
            bool ctrl  = window::key_pressed(sys::vkControl);
            bool shift = window::key_pressed(sys::vkShift);

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
        if (window::key_pressed(sys::vkShift))
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

bool table::onMouseUp(sys::Button button, int times, Point point)
{
    capture(false);

    if (impl_->dragging_.underway)
        impl_->dragging_.underway = false;
    else
    {
        index index = hittest(point);

        if (exists(index))
            onItemClickedHandle(index, button);
        else if (index.row == -1 || index.col == -1)
        {
            if (0 <= index.row && index.row < (int)impl_->rows_.size())
                onRowHeaderClickedHandle(index.row, button);
            else if (0 <= index.col && index.col < (int)impl_->columns_.size())
                onColumnHeaderClickedHandle(index.col, button);
        }
    }

    return true;
}

bool table::onMouseMove(Point point)
{
    if (impl_->dragging_.underway) {
        int glcol = impl_->dragging_.milestone.glcol;
        if (glcol == -1) {
            int newsize = impl_->dragging_.milestone.size
                        + point.x - impl_->dragging_.milestone.point.x;
            row_headers().width(newsize < 5 ? 5 : newsize);
        } else if (glcol > -1) {
            int newsize = impl_->dragging_.milestone.size
                        + point.x - impl_->dragging_.milestone.point.x;
            column(glcol).width(newsize < 5 ? 5 : newsize);
        }

        int glrow = impl_->dragging_.milestone.glrow;
        if (glrow == -1) {
            int newsize = impl_->dragging_.milestone.size
                        + point.y - impl_->dragging_.milestone.point.y;
            column_headers().height(newsize < 5 ? 5 : newsize);
        } else if (glrow > -1) {
            int newsize = impl_->dragging_.milestone.size
                        + point.y - impl_->dragging_.milestone.point.y;
            row(glrow).height(newsize < 5 ? 5 : newsize);
        }

        return true;
    }

    index index = hittest(point), oldcurrent = current();
    if (!exists(index))
        return true;

    if (impl_->enter_ != index)
    {
        if (exists(impl_->enter_))
        {
            onItemMouseLeaveHandle(impl_->enter_);
            if (impl_->enter_.row != index.row)
                onRowMouseLeaveHandle(impl_->enter_.row);
            if (impl_->enter_.col != index.col)
                onColumnMouseLeaveHandle(impl_->enter_.col);
        }
        onItemMouseEnterHandle(index);
        if (impl_->enter_.row != index.row)
            onRowMouseEnterHandle(index.row);
        if (impl_->enter_.col != index.col)
            onColumnMouseEnterHandle(index.col);
        impl_->enter_ = index;
    }

    if (!window::key_pressed(sys::vkLButton) || (index == oldcurrent))
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
    if (impl->item.brush && impl->item.brush->text)
        return impl->item.brush->text;
    else if (impl->host.impl_->rows_[impl->index.row].brush.text)
        return impl->host.impl_->rows_[impl->index.row].brush.text;
    else if (impl->host.impl_->columns_[impl->index.col].brush.text)
        return impl->host.impl_->columns_[impl->index.col].brush.text;
    else
        return impl->host.impl_->brush_.text;
}

brush_sptr table::item::background_brush(void) const
{
    if (impl->item.brush && impl->item.brush->background)
        return impl->item.brush->background;
    else if (impl->host.impl_->rows_[impl->index.row].brush.background)
        return impl->host.impl_->rows_[impl->index.row].brush.background;
    else if (impl->host.impl_->columns_[impl->index.col].brush.background)
        return impl->host.impl_->columns_[impl->index.col].brush.background;
    else
        return impl->host.impl_->brush_.background;
}

brush_sptr table::item::selection_brush(void) const
{
    if (impl->item.brush && impl->item.brush->selection)
        return impl->item.brush->selection;
    else if (impl->host.impl_->rows_[impl->index.row].brush.selection)
        return impl->host.impl_->rows_[impl->index.row].brush.selection;
    else if (impl->host.impl_->columns_[impl->index.col].brush.selection)
        return impl->host.impl_->columns_[impl->index.col].brush.selection;
    else
        return impl->host.impl_->brush_.selection;
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
        std::wstring previous = impl->item.text.content();
        impl->item.text.content(text);
        impl->host.onItemTextChangedHandle(impl->index, text, previous);
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

void table::item::text_color(color color)
{
    text_brush(make_spbrush<color_brush>(color));
}

void table::item::text_brush(const brush_sptr& brush)
{
    if (!impl->item.brush)
        impl->item.brush.reset(new table::implement::brush);

    if (impl->item.brush->text != brush)
    {
        impl->item.brush->text = brush;
        impl->repaint();
    }
}

void table::item::background_color(color color)
{
    background_brush(make_spbrush<color_brush>(color));
}

void table::item::background_brush(const brush_sptr& brush)
{
    if (!impl->item.brush)
        impl->item.brush.reset(new table::implement::brush);

    if (impl->item.brush->background != brush)
    {
        impl->item.brush->background = brush;
        impl->repaint();
    }
}

void table::item::selection_color(color color)
{
    selection_brush(make_spbrush<color_brush>(color));
}

void table::item::selection_brush(const brush_sptr& brush)
{
    if (!impl->item.brush)
        impl->item.brush.reset(new table::implement::brush);

    if (impl->item.brush->selection != brush)
    {
        impl->item.brush->selection = brush;
        impl->repaint();
    }
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
    std::shared_ptr<item> self(new item(*this));
    editbox& eb = editbox::pop(&impl->host, rect(), impl->item.text, text);

    impl->host.scrollto(impl->index);

    handler::sptr handler =
        impl->host.vertical_scroll().onScroll(
        impl->host.horizontal_scroll().onScroll(
        [self, &eb](scroll*)
        {
            eb.xy(self->rect().left_top());
        }));

    eb.background_color(color::white());
    eb.onEditComplete(
        [self](editbox*, std::wstring text)
        {
            self->impl->host.onItemEditCompleteHandle(self->impl->index, text);
            self->text(text);
        });
    eb.onEditFinish(
        [self, handler](editbox*)
        {
            self->impl->host.vertical_scroll().onScrollRemove(handler);
            self->impl->host.horizontal_scroll().onScrollRemove(handler);
        });
}

unsigned int table::gridline::width(void) const
{
    return impl->gridline.width();
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

void table::gridline::color(const struct color& color)
{
    brush(make_spbrush<color_brush>(color));
}

void table::gridline::brush(const brush_sptr& brush)
{
    impl->gridline.pen.brush(brush);
    impl->host.impl_->update();
}

void table::gridline::dash_array(const std::vector<double>& _dash_array)
{
    dash_array(_dash_array.data(), _dash_array.size());
}

void table::gridline::dash_array(const double* dash_array, unsigned int count)
{
    impl->gridline.pen.dash_array(dash_array, count);
    impl->host.impl_->update();
}

bool table::row_headers::visible(void) const
{
    return impl->headers.visible;
}

bool table::row_headers::scalable(void) const
{
    return impl->headers.scalable;
}

unsigned int table::row_headers::width(void) const
{
    return impl->headers.width;
}

void table::row_headers::show(void)
{
    visible(true);
}

void table::row_headers::hide(void)
{
    visible(false);
}

void table::row_headers::visible(bool visible)
{
    if (impl->headers.visible != visible)
    {
        impl->headers.visible = visible;
        impl->host.impl_->update();
    }
}

void table::row_headers::scalable(bool scalable)
{
    impl->headers.scalable = scalable;
}

void table::row_headers::width(unsigned int width)
{
    if (impl->headers.width != width)
    {
        impl->headers.width = width;
        impl->host.impl_->update();
    }
}

bool table::row_header::scalable(void) const
{
    return impl->header.scalable;
}

unsigned int table::row_header::height(void) const
{
    return impl->header.height;
}

void table::row_header::scalable(bool scalable)
{
    impl->header.scalable = scalable;
}

void table::row_header::height(unsigned int height)
{
    if (impl->header.height != height)
    {
        impl->header.height = height;
        impl->host.impl_->update();
    }
}

unsigned int table::row::height(void) const
{
    return impl->row.header.height;
}

unsigned int table::row::optimal_height(void) const
{
    unsigned int optheight = impl->host.
        impl_->optimal_world(impl->row.header.text).height;

    for (unsigned int i = 0; i < impl->row.items.size(); ++i)
    {
        unsigned int item_optheight = impl->host.
            impl_->optimal_world(impl->row.items[i].text).height;
        if (optheight < item_optheight)
            optheight = item_optheight;
    }

    return optheight + 4;
}

const table::gridline table::row::gridline(void) const
{
    return const_cast<row*>(this)->gridline();
}

const table::row_header table::row::header(void) const
{
    return const_cast<row*>(this)->header();
}

class table::item table::row::item(int col)
{
    return impl->host.item({impl->index, col});
}

const class table::item table::row::item(int col) const
{
    return impl->host.item({impl->index, col});
}

void table::row::height(unsigned int height)
{
    if (impl->row.header.height != height)
    {
        impl->row.header.height = height;
        impl->host.impl_->update();
    }
}

void table::row::text_color(color color)
{
    text_brush(make_spbrush<color_brush>(color));
}

void table::row::text_brush(const brush_sptr& brush)
{
    impl->row.brush.text = brush;

    for (table::implement::item& item : impl->row.items)
        if (item.brush)
            item.brush->text.reset();

    impl->repaint();
}

void table::row::background_color(color color)
{
    background_brush(make_spbrush<color_brush>(color));
}

void table::row::background_brush(const brush_sptr& brush)
{
    impl->row.brush.background = brush;

    for (table::implement::item& item : impl->row.items)
        if (item.brush)
            item.brush->background.reset();

    impl->repaint();
}

void table::row::selection_color(color color)
{
    selection_brush(make_spbrush<color_brush>(color));
}

void table::row::selection_brush(const brush_sptr& brush)
{
    impl->row.brush.selection = brush;

    for (table::implement::item& item : impl->row.items)
        if (item.brush)
            item.brush->selection.reset();

    impl->repaint();
}

table::gridline table::row::gridline(void)
{
    struct gridline ret;
    ret.impl.reset(new gridline::implement(impl->host, impl->row.gridline));
    return ret;
}

table::row_header table::row::header(void)
{
    table::implement::row_header& header = impl->row.header;
    struct index i = {impl->index, -1};
    struct row_header ret;

    ret.item::impl.reset(new item::implement(impl->host, header, i));
    ret.impl.reset(new row_header::implement(impl->host, header));

    return ret;
}

bool table::column_headers::visible(void) const
{
    return impl->headers.visible;
}

bool table::column_headers::scalable(void) const
{
    return impl->headers.scalable;
}

unsigned int table::column_headers::height(void) const
{
    return impl->headers.height;
}

void table::column_headers::show(void)
{
    visible(true);
}

void table::column_headers::hide(void)
{
    visible(false);
}

void table::column_headers::visible(bool visible)
{
    if (impl->headers.visible != visible)
    {
        impl->headers.visible = visible;
        impl->host.impl_->update();
    }
}

void table::column_headers::scalable(bool scalable)
{
    impl->headers.scalable = scalable;
}

void table::column_headers::height(unsigned int height)
{
    if (impl->headers.height != height)
    {
        impl->headers.height = height;
        impl->host.impl_->update();
    }
}

bool table::column_header::scalable(void) const
{
    return impl->header.scalable;
}

unsigned int table::column_header::width(void) const
{
    return impl->header.width;
}

void table::column_header::scalable(bool scalable)
{
    impl->header.scalable = scalable;
}

void table::column_header::width(unsigned int width)
{
    if (impl->header.width != width)
    {
        impl->header.width = width;
        impl->host.impl_->update();
    }
}

unsigned int table::column::width(void) const
{
    return impl->column.header.width;
}

unsigned int table::column::optimal_width(void) const
{
    unsigned int optwidth = impl->host.
        impl_->optimal_world(impl->column.header.text).width;

    for (unsigned int i = 0; i < impl->host.impl_->rows_.size(); ++i)
    {
        text& t = impl->host.impl_->rows_[i].items[impl->index].text;
        unsigned int item_optwidth = impl->host.impl_->optimal_world(t).width;
        if (optwidth < item_optwidth)
            optwidth = item_optwidth;
    }

    return optwidth + 10;
}

const table::gridline table::column::gridline(void) const
{
    return const_cast<column*>(this)->gridline();
}

const table::column_header table::column::header(void) const
{
    return const_cast<column*>(this)->header();
}

class table::item table::column::item(int row)
{
    return impl->host.item({row, impl->index});
}

const class table::item table::column::item(int row) const
{
    return impl->host.item({row, impl->index});
}

void table::column::width(unsigned int width)
{
    if (impl->column.header.width != width)
    {
        impl->column.header.width = width;
        impl->host.impl_->update();
    }
}

void table::column::text_color(color color)
{
    text_brush(make_spbrush<color_brush>(color));
}

void table::column::text_brush(const brush_sptr& brush)
{
    std::vector<table::implement::row_type>& rows = impl->host.impl_->rows_;

    impl->column.brush.text = brush;
    for (unsigned int i = 0; i < impl->host.columns(); ++i)
        if (rows[i].items[impl->index].brush)
            rows[i].items[impl->index].brush->text.reset();

    impl->repaint();
}

void table::column::background_color(color color)
{
    background_brush(make_spbrush<color_brush>(color));
}

void table::column::background_brush(const brush_sptr& brush)
{
    std::vector<table::implement::row_type>& rows = impl->host.impl_->rows_;

    impl->column.brush.background = brush;
    for (unsigned int i = 0; i < impl->host.columns(); ++i)
        if (rows[i].items[impl->index].brush)
            rows[i].items[impl->index].brush->background.reset();

    impl->repaint();
}

void table::column::selection_color(color color)
{
    selection_brush(make_spbrush<color_brush>(color));
}

void table::column::selection_brush(const brush_sptr& brush)
{
    std::vector<table::implement::row_type>& rows = impl->host.impl_->rows_;

    impl->column.brush.selection = brush;
    for (unsigned int i = 0; i < impl->host.columns(); ++i)
        if (rows[i].items[impl->index].brush)
            rows[i].items[impl->index].brush->selection.reset();

    impl->repaint();
}

table::gridline table::column::gridline(void)
{
    struct gridline ret;
    ret.impl.reset(new gridline::implement(impl->host, impl->column.gridline));
    return ret;
}

table::column_header table::column::header(void)
{
    table::implement::column_header& header = impl->column.header;
    struct index i = {-1, impl->index};
    struct column_header ret;

    ret.item::impl.reset(new item::implement(impl->host, header, i));
    ret.impl.reset(new column_header::implement(impl->host, header));

    return ret;
}
