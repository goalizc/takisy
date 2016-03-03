#include <cstring>
#include <vector>
#include <algorithm>
#include <takisy/gui/widget/layout.h>
#include "impl/layout.hpp"

layout::layout(void)
    : impl_(new implement(this))
{}

layout::~layout(void)
{
    delete impl_;
}

Alignment layout::alignment(void) const
{
    return impl_->alignment_;
}

struct margin layout::margin(void) const
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
    readapt();
}

void layout::margin(const struct margin& _margin)
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

    readapt();
}

void layout::margin_left(unsigned int left)
{
    struct margin _margin = margin();
    _margin.left = left;
    margin(_margin);
}

void layout::margin_top(unsigned int top)
{
    struct margin _margin = margin();
    _margin.top = top;
    margin(_margin);
}

void layout::margin_right(unsigned int right)
{
    struct margin _margin = margin();
    _margin.right = right;
    margin(_margin);
}

void layout::margin_bottom(unsigned int bottom)
{
    struct margin _margin = margin();
    _margin.bottom = bottom;
    margin(_margin);
}

void layout::spacing(unsigned int spacing)
{
    impl_->spacing_ = spacing;
    readapt();
}

void layout::onSize(Size)
{
    readapt();
}

class spacer_layout::implement
{
    friend class spacer_layout;
    friend class horizontal_layout;
    friend class vertical_layout;

    class spacer final : public widget
    {};

private:
    std::vector<spacer*> spacers_;
};

spacer_layout::spacer_layout(void)
    : layout(), impl_(new implement)
{}

spacer_layout::~spacer_layout(void)
{
    for (class implement::spacer* spacer : impl_->spacers_)
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

void spacer_layout::add_spacer(unsigned int minimal_size,
                                      unsigned int maximal_size)
{
    implement::spacer* spacer = new implement::spacer;
    spacer->minimal_width(minimal_size);
    spacer->maximal_width(maximal_size);
    spacer->show();

    add(spacer);

    impl_->spacers_.push_back(spacer);
}

horizontal_layout::horizontal_layout(void)
    : spacer_layout()
{}

void horizontal_layout::readapt(void)
{
    std::vector<widget*> children = layout::impl_->visible_children();
    if (children.empty())
        return;

    struct margin margin = layout::impl_->margin_;
    unsigned int spacing = layout::impl_->spacing_;
    int available = width() - margin.left - margin.right
                    - spacing * (children.size()
                                 - spacer_layout::impl_->spacers_.size() - 1);
    int remind  = children.size();
    int average = available / remind;

    std::vector<widget*> sorted_children = children;
    std::sort(sorted_children.begin(), sorted_children.end(),
              [](widget* a, widget* b) -> bool
                { return a->maximal_width() < b->maximal_width(); });
    for (widget* child : sorted_children)
    {
        remind--;

        if (average <= 0)
            child->width(0);
        else
        {
            child->width(average);

            if (remind != 0)
            {
                available -= child->width();
                average    = available / remind;
            }
        }
    }

    unsigned int child_height = height() - margin.top - margin.bottom;
    unsigned int offset = margin.left;
    if (child_height < 0)
        child_height = 0;

    for (widget* child : children)
    {
        if (!child->visible())
            continue;

        int y = 0;

        child->height(child_height);
        switch (layout::impl_->get_alignment(child) & aVertical)
        {
        default:
        case aTop:
            y = margin.top;
            break;
        case aCenter:
            y = int(height() - child->height()
                    + margin.top - margin.bottom) / 2;
            break;
        case aBottom:
            y = height() - margin.bottom - child->height();
            break;
        }
        child->xy(offset, y);

        offset += child->width();
        if (!dynamic_cast<spacer_layout::implement::spacer*>(child))
            offset += spacing;
    }
}

vertical_layout::vertical_layout(void)
    : spacer_layout()
{}

void vertical_layout::readapt(void)
{
    std::vector<widget*> children = layout::impl_->visible_children();
    if (children.empty())
        return;

    struct margin margin = layout::impl_->margin_;
    unsigned int spacing = layout::impl_->spacing_;
    int available = height() - margin.top - margin.bottom
                    - spacing * (children.size()
                                 - spacer_layout::impl_->spacers_.size() - 1);
    int remind  = children.size();
    int average = available / remind;

    std::vector<widget*> sorted_children = children;
    std::sort(sorted_children.begin(), sorted_children.end(),
              [](widget* a, widget* b) -> bool
                { return a->maximal_height() < b->maximal_height(); });
    for (widget* child : sorted_children)
    {
        remind--;

        if (average <= 0)
            child->height(0);
        else
        {
            child->height(average);

            if (remind != 0)
            {
                available -= child->height();
                average    = available / remind;
            }
        }
    }

    unsigned int child_width = width() - margin.left - margin.right;
    unsigned int offset = margin.top;
    if (child_width < 0)
        child_width = 0;

    for (widget* child : children)
    {
        if (!child->visible())
            continue;

        int x = 0;

        child->width(child_width);
        switch (layout::impl_->get_alignment(child) & aHorizontal)
        {
        default:
        case aLeft:
            x = margin.left;
            break;
        case aCenter:
            x = int(width() - child->width()
                    + margin.left - margin.right) / 2;
            break;
        case aRight:
            x = width() - margin.right - child->width();
            break;
        }
        child->xy(x, offset);

        offset += child->height();
        if (!dynamic_cast<spacer_layout::implement::spacer*>(child))
            offset += spacing;
    }
}
