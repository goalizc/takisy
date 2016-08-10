#include <cstring>
#include <vector>
#include <algorithm>
#include <takisy/gui/widget/layout.h>

class layout::implement
{
    friend class layout;
    friend class horizontal_layout;
    friend class vertical_layout;
    friend class grid_layout;

public:
    implement(layout* _this)
        : this_(_this), readapting_(false)
        , alignment_(aCenter), margin_({0, 0, 0, 0}), spacing_(0)
    {}

    virtual ~implement(void)
    {}

public:
    Alignment get_alignment(widget* widget) const
    {
        if (widget->exists_attribute("alignment"))
            return widget->attribute<Alignment>("alignment");

        return alignment_;
    }

    std::vector<widget*> visual_children(void) const
    {
        std::vector<widget*> children = this_->children();

        children.erase(std::remove_if(children.begin(), children.end(),
                                      [](widget* child) -> bool
                                      {
                                         return !child->visible();
                                      }),
                       children.end());

        return children;
    }

    void readapt_safe(void)
    {
        if (readapting_)
            return;

        struct david
        {
            bool& readapting;
        public:
            david(bool& readapting) : readapting(readapting)
                        { readapting = true; }
           ~david(void) { readapting = false; }
        } __david__(readapting_);

        readapt();
    }

    virtual void readapt(void) = 0;

protected:
    layout*      this_;
    bool         readapting_;
    Alignment    alignment_;
    Margin       margin_;
    unsigned int spacing_;
};

layout::layout(void)
    : impl_(nullptr)
{}

layout::~layout(void)
{
    delete impl_;
}

Alignment layout::alignment(void) const
{
    return impl_->alignment_;
}

Margin layout::margin(void) const
{
    return impl_->margin_;
}

unsigned int layout::spacing(void) const
{
    return impl_->spacing_;
}

void layout::alignment(Alignment alignment)
{
    impl_->alignment_ = alignment;
    impl_->readapt_safe();
}

void layout::margin(const Margin& _margin)
{
    margin(_margin.left, _margin.top, _margin.right, _margin.bottom);
}

void layout::margin(unsigned int _margin)
{
    margin(_margin, _margin, _margin, _margin);
}

void layout::margin(unsigned int left, unsigned int top,
                    unsigned int right, unsigned int bottom)
{
    impl_->margin_.left   = left;
    impl_->margin_.top    = top;
    impl_->margin_.right  = right;
    impl_->margin_.bottom = bottom;

    impl_->readapt_safe();
}

void layout::margin_left(unsigned int left)
{
    Margin _margin = margin();
    _margin.left = left;
    margin(_margin);
}

void layout::margin_top(unsigned int top)
{
    Margin _margin = margin();
    _margin.top = top;
    margin(_margin);
}

void layout::margin_right(unsigned int right)
{
    Margin _margin = margin();
    _margin.right = right;
    margin(_margin);
}

void layout::margin_bottom(unsigned int bottom)
{
    Margin _margin = margin();
    _margin.bottom = bottom;
    margin(_margin);
}

void layout::spacing(unsigned int spacing)
{
    impl_->spacing_ = spacing;
    impl_->readapt_safe();
}

void layout::onAdd(widget* widget)
{
    if (widget->visible())
        impl_->readapt_safe();
}

void layout::onRemove(widget* widget)
{
    if (widget->visible())
        impl_->readapt_safe();
}

void layout::onSize(void)
{
    impl_->readapt_safe();
}

bool layout::onChildMoving(widget* child, Point& point)
{
    return impl_->readapting_;
}

bool layout::onChildSizing(widget* child, Size& size)
{
    return impl_->readapting_;
}

void layout::onChildShown(widget* child)
{
    impl_->readapt_safe();
}

void layout::onChildHidden(widget* child)
{
    impl_->readapt_safe();
}

class spacer_layout::implement
{
    friend class spacer_layout;
    friend class horizontal_layout;
    friend class vertical_layout;

    class spacer final : public widget
    {};

private:
    std::vector<widget*> spacers_;
};

spacer_layout::spacer_layout(void)
    : layout(), impl_(new implement)
{}

spacer_layout::~spacer_layout(void)
{
    for (class widget* spacer : impl_->spacers_)
        delete spacer;

    delete impl_;
}

void spacer_layout::add_spacer(void)
{
    add_spacer(0, ~0u);
}

void spacer_layout::add_spacer(unsigned int size)
{
    add_spacer(size, size);
}

void spacer_layout::add_spacer(unsigned int minimal, unsigned int maximal)
{
    implement::spacer* spacer = new implement::spacer;

    if (!dynamic_cast<horizontal_layout*>(this))
    {
        spacer->minimal_width(minimal);
        spacer->maximal_width(maximal);
    }
    else
    if (!dynamic_cast<vertical_layout*>(this))
    {
        spacer->minimal_height(minimal);
        spacer->maximal_height(maximal);
    }

    spacer->show();
    add(spacer);
    impl_->spacers_.push_back(spacer);
}

class horizontal_layout::implement : public layout::implement
{
public:
    implement(horizontal_layout* _this)
        : layout::implement(_this)
    {}

public:
    void readapt(void) override
    {
        std::vector<widget*> children = visual_children();
        if (children.empty())
            return;

        class spacer_layout* sl = dynamic_cast<class spacer_layout*>(this_);
        std::vector<widget*>& spacers = sl->impl_->spacers_;
        int available = this_->width() - margin_.left - margin_.right
                        - spacing_ * (children.size() - spacers.size() - 1);
        int remind  = children.size();
        int average = available / remind;

        std::vector<widget*> sorted_children = children;
        std::sort(sorted_children.begin(), sorted_children.end(),
                  [](widget* a, widget* b) -> bool
                  {
                      return a->maximal_width() < b->maximal_width();
                  });

        for (widget* child : sorted_children)
        {
            child->width(average < 0 ? 0 : average);

            if (--remind != 0)
            {
                available -= child->width();
                average    = available / remind;
            }
        }

        unsigned int cheight = this_->height() - margin_.top - margin_.bottom;
        unsigned int offset  = margin_.left;

        for (widget* child : children)
        {
            if (!child->visible())
                continue;

            int y = 0;

            child->height(cheight);
            switch (get_alignment(child) & aVertical)
            {
            default:
            case aTop:
                y = margin_.top;
                break;
            case aCenter:
                y = int(this_->height() - child->height()
                        + margin_.top - margin_.bottom) / 2;
                break;
            case aBottom:
                y = this_->height() - margin_.bottom - child->height();
                break;
            }
            child->xy(offset, y);

            offset += child->width();
            if (!dynamic_cast<spacer_layout::implement::spacer*>(child))
                offset += spacing_;
        }
    }
};

horizontal_layout::horizontal_layout(void)
    : spacer_layout()
{
    layout::impl_ = new implement(this);
}

class vertical_layout::implement : public layout::implement
{
public:
    implement(vertical_layout* _this)
        : layout::implement(_this)
    {}

public:
    void readapt(void) override
    {
        std::vector<widget*> children = visual_children();
        if (children.empty())
            return;

        class spacer_layout* sl = dynamic_cast<class spacer_layout*>(this_);
        std::vector<widget*>& spacers = sl->impl_->spacers_;
        int available = this_->height() - margin_.top - margin_.bottom
                        - spacing_ * (children.size() - spacers.size() - 1);
        int remind  = children.size();
        int average = available / remind;

        std::vector<widget*> sorted_children = children;
        std::sort(sorted_children.begin(), sorted_children.end(),
                  [](widget* a, widget* b) -> bool
                  {
                      return a->maximal_height() < b->maximal_height();
                  });

        for (widget* child : sorted_children)
        {
            child->height(average < 0 ? 0 : average);

            if (--remind != 0)
            {
                available -= child->height();
                average    = available / remind;
            }
        }

        unsigned int cwidth = this_->width() - margin_.left - margin_.right;
        unsigned int offset = margin_.top;

        for (widget* child : children)
        {
            if (!child->visible())
                continue;

            int x = 0;

            child->width(cwidth);
            switch (get_alignment(child) & aHorizontal)
            {
            default:
            case aLeft:
                x = margin_.left;
                break;
            case aCenter:
                x = int(this_->width() - child->width()
                        + margin_.left - margin_.right) / 2;
                break;
            case aRight:
                x = this_->width() - margin_.right - child->width();
                break;
            }
            child->xy(x, offset);

            offset += child->height();
            if (!dynamic_cast<spacer_layout::implement::spacer*>(child))
                offset += spacing_;
        }
    }
};

vertical_layout::vertical_layout(void)
    : spacer_layout()
{
    layout::impl_ = new implement(this);
}

class grid_layout::implement : public layout::implement
{
    friend class grid_layout;

public:
    implement(grid_layout* _this, unsigned int column, unsigned int row)
        : layout::implement(_this)
        , column_(column), row_(row), items_(column_ * row_)
    {}

public:
    widget* item(unsigned int col, unsigned int row) const
    {
        if (col < column_ && row < row_)
            return items_[row * column_ + col];

        return nullptr;
    }

    widget* item(unsigned int col, unsigned int row, widget* newer)
    {
        if (col < column_ && row < row_)
        {
            widget* older = item(col, row);

            this_->layout::remove(older);
            this_->layout::add(newer);

            items_[row * column_ + col] = newer;

            return older;
        }

        return nullptr;
    }

public:
    void readapt(void) override;

private:
    unsigned int column_, row_;
    std::vector<widget*> items_;
};

void grid_layout::implement::readapt(void)
{
    if (items_.empty())
        return;

    if (true)
    {
        int available = this_->width() - margin_.left - margin_.right
                        - spacing_ * (column_ - 1);
        int remind  = column_;
        int average = available / remind;
        int offset  = margin_.left;

        for (unsigned int i = 0; i < column_; ++i)
        {
            unsigned int colwidth = 0;
            unsigned int width = average < 0 ? 0 : average;

            for (unsigned int j = 0; j < row_; ++j)
            {
                widget* w = item(i, j);
                if (!w)
                    continue;

                w->width(width);

                if (colwidth < w->width())
                    colwidth = w->width();
            }

            for (unsigned int j = 0; j < row_; ++j)
            {
                widget* w = item(i, j);
                if (!w)
                    continue;

                switch (get_alignment(w) & aHorizontal)
                {
                default:
                case aLeft:
                    w->x(offset);
                    break;
                case aCenter:
                    w->x(offset + (colwidth - w->width()) / 2);
                    break;
                case aRight:
                    w->x(offset + colwidth - w->width());
                    break;
                }
            }

            if (--remind != 0)
            {
                available -= colwidth;
                average    = available / remind;
            }

            offset += colwidth + spacing_;
        }
    }

    if (true)
    {
        int available = this_->height() - margin_.top - margin_.bottom
                        - spacing_ * (row_ - 1);
        int remind  = row_;
        int average = available / remind;
        int offset  = margin_.top;

        for (unsigned int i = 0; i < row_; ++i)
        {
            unsigned int rowheight = 0;
            unsigned int height = average < 0 ? 0 : average;

            for (unsigned int j = 0; j < column_; ++j)
            {
                widget* w = item(j, i);
                if (!w)
                    continue;

                w->height(height);

                if (rowheight < w->height())
                    rowheight = w->height();
            }

            for (unsigned int j = 0; j < column_; ++j)
            {
                widget* w = item(j, i);
                if (!w)
                    continue;

                switch (get_alignment(w) & aVertical)
                {
                default:
                case aTop:
                    w->y(offset);
                    break;
                case aCenter:
                    w->y(offset + (rowheight - w->height()) / 2);
                    break;
                case aBottom:
                    w->y(offset + rowheight - w->height());
                    break;
                }
            }

            if (--remind != 0)
            {
                available -= rowheight;
                average    = available / remind;
            }

            offset += rowheight + spacing_;
        }
    }
}

grid_layout::grid_layout(unsigned int column, unsigned int row)
    : layout(), impl_(new implement(this, column, row))
{
    layout::impl_ = impl_;
}

widget* grid_layout::item(unsigned int col, unsigned int row) const
{
    return impl_->item(col, row);
}

widget* grid_layout::item(unsigned int col, unsigned int row, widget* item)
{
    return impl_->item(col, row, item);
}

bool grid_layout::onAdding(widget* widget)
{
    for (class widget*& item : impl_->items_)
        if (!item)
            return item = widget, true;

    return false;
}

bool grid_layout::onRemoving(widget* widget)
{
    for (class widget*& item : impl_->items_)
        if (item == widget)
            item = nullptr;

    return true;
}
