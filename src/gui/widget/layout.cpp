#include <cstring>
#include <vector>
#include <algorithm>
#include <takisy/gui/widget/layout.h>
#include "impl/layout.hpp"

void layout::implement::recalculate(void)
{
    if (recalculating_)
        return;

    struct david
    {
        bool& recalculating;
    public:
        david(bool& recalculating) : recalculating(recalculating)
                    { recalculating = true; }
       ~david(void) { recalculating = false; }
    } __david__(recalculating_);

    std::vector<widget*> children = this_->children();
    children.erase(std::remove_if(children.begin(), children.end(),
                     [](widget* child) -> bool { return !child->visible(); }),
                   children.end());
    if (children.size() == 0)
        return;

    if (!vertical_)
    {
        int available = this_->width() - margin_.left - margin_.right
                        - spacing_ * (children.size() - spacers_.size() - 1);
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

        unsigned int height = this_->height() - margin_.top - margin_.bottom;
        unsigned int offset = margin_.left;
        if (height < 0)
            height = 0;

        for (widget* child : children)
        {
            if (!child->visible())
                continue;

            Alignment alignment = get_alignment(child);
            int y = 0;

            child->height(height);
            if (alignment == aFrontward)
                y = margin_.top;
            else
            if (alignment == aCenter)
                y = int(this_->height() - child->height()
                        + margin_.top - margin_.bottom) / 2;
            else
                y = this_->height() - margin_.bottom - child->height();
            child->xy(offset, y);

            offset += child->width();
            if (!dynamic_cast<implement::spacer*>(child))
                offset += spacing_;
        }
    }
    else
    {
        int available = this_->height() - margin_.top - margin_.bottom
                        - spacing_ * (children.size() - spacers_.size() - 1);
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

        unsigned int width = this_->width() - margin_.left - margin_.right;
        unsigned int offset = margin_.top;
        if (width < 0)
            width = 0;

        for (widget* child : children)
        {
            if (!child->visible())
                continue;

            Alignment alignment = get_alignment(child);
            int x = 0;

            child->width(width);
            if (alignment == aFrontward)
                x = margin_.left;
            else
            if (alignment == aCenter)
                x = int(this_->width() - child->width()
                        + margin_.left - margin_.right) / 2;
            else
                x = this_->width() - margin_.right - child->width();
            child->xy(x, offset);

            offset += child->height();
            if (!dynamic_cast<implement::spacer*>(child))
                offset += spacing_;
        }
    }
}

layout::layout(void)
    : impl_(new implement(this))
{
    impl_->vertical_ = false;
}

layout::~layout(void)
{
    for (implement::spacer* spacer : impl_->spacers_)
        delete spacer;

    delete impl_;
}

layout::Alignment layout::alignment(void) const
{
    return impl_->alignment_;
}

layout::Margin layout::margin(void) const
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
    impl_->recalculate();
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

    impl_->recalculate();
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
    impl_->recalculate();
}

void layout::add_spacer(void)
{
    add_spacer(0, ~0u);
}

void layout::add_spacer(unsigned int size)
{
    add_spacer(size, size);
}

void layout::add_spacer(unsigned int minimal_size, unsigned int maximal_size)
{
    implement::spacer* spacer = new implement::spacer;
    spacer->minimal_width(minimal_size);
    spacer->maximal_width(maximal_size);
    spacer->show();

    add(spacer);

    impl_->spacers_.push_back(spacer);
}

void layout::onSize(Size)
{
    impl_->recalculate();
}

vertical_layout::vertical_layout(void)
    : layout()
{
    impl_->vertical_ = true;
}
