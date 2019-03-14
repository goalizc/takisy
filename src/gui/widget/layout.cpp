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
    implement(layout& self)
        : self(self), readapting_(false)
        , alignment_(aCenter), margin_({0, 0, 0, 0}), spacing_(0)
    {}

    virtual ~implement(void)
    {}

public:
    unsigned int get_alignment(widget* widget) const
    {
        if (!widget->exists_property("alignment"))
            return alignment_;

        std::string property = widget->property("alignment");

        if (property == "left")
            return aLeft;
        else if (property == "right")
            return aRight;
        else if (property == "top")
            return aTop;
        else if (property == "bottom")
            return aBottom;
        else if (property == "left-top")
            return aLeftTop;
        else if (property == "left-bottom")
            return aLeftBottom;
        else if (property == "right-top")
            return aRightTop;
        else if (property == "right-bottom")
            return aRightBottom;
        else if (property == "center")
            return aCenter;

        return alignment_;
    }

    std::vector<widget*> visual_children(void) const
    {
        std::vector<widget*> children = self.children();

        children.erase(
            std::remove_if(children.begin(), children.end(),
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
    layout&      self;
    bool         readapting_;
    unsigned int alignment_;
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

unsigned int layout::alignment(void) const
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

void layout::alignment(unsigned int alignment)
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

void spacer_layout::add_spacer(unsigned int lower, unsigned int upper)
{
    implement::spacer* spacer = new implement::spacer;

    if (!dynamic_cast<horizontal_layout*>(this))
    {
        spacer->lower_width(lower);
        spacer->upper_width(upper);
    }
    else
    if (!dynamic_cast<vertical_layout*>(this))
    {
        spacer->lower_height(lower);
        spacer->upper_height(upper);
    }

    spacer->show();
    add(spacer);
    impl_->spacers_.push_back(spacer);
}

class horizontal_layout::implement : public layout::implement
{
public:
    implement(horizontal_layout& self)
        : layout::implement(self)
    {}

public:
    void readapt(void) override
    {
        std::vector<widget*> children = visual_children();
        if (children.empty())
            return;

        class spacer_layout& sl = dynamic_cast<class spacer_layout&>(self);
        std::vector<widget*>& spacers = sl.impl_->spacers_;
        int available = self.width() - margin_.left - margin_.right
                        - spacing_ * (children.size() - spacers.size() - 1);
        int remind  = children.size();
        int average = available / remind;

        std::vector<widget*> sorted_children = children;
        std::sort(sorted_children.begin(), sorted_children.end(),
                  [](widget* a, widget* b) -> bool
                  {
                      return a->upper_width() < b->upper_width();
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

        int cheight = self.height() - margin_.top - margin_.bottom;
        int offset  = margin_.left;

        if (cheight < 0)
            cheight = 0;

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
                y = int(self.height() - child->height()
                        + margin_.top - margin_.bottom) / 2;
                break;
            case aBottom:
                y = self.height() - margin_.bottom - child->height();
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
    layout::impl_ = new implement(*this);
}

class vertical_layout::implement : public layout::implement
{
public:
    implement(vertical_layout& self)
        : layout::implement(self)
    {}

public:
    void readapt(void) override
    {
        std::vector<widget*> children = visual_children();
        if (children.empty())
            return;

        class spacer_layout& sl = dynamic_cast<class spacer_layout&>(self);
        std::vector<widget*>& spacers = sl.impl_->spacers_;
        int available = self.height() - margin_.top - margin_.bottom
                        - spacing_ * (children.size() - spacers.size() - 1);
        int remind  = children.size();
        int average = available / remind;

        std::vector<widget*> sorted_children = children;
        std::sort(sorted_children.begin(), sorted_children.end(),
                  [](widget* a, widget* b) -> bool
                  {
                      return a->upper_height() < b->upper_height();
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

        int cwidth = self.width() - margin_.left - margin_.right;
        int offset = margin_.top;

        if (cwidth < 0)
            cwidth = 0;

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
                x = int(self.width() - child->width()
                        + margin_.left - margin_.right) / 2;
                break;
            case aRight:
                x = self.width() - margin_.right - child->width();
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
    layout::impl_ = new implement(*this);
}
