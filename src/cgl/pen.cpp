#include <takisy/cgl/pen.h>
#include "impl/pen.hpp"

pen::pen(void)
    : pen(1)
{}

pen::pen(double width)
    : pen(width, color::black())
{}

pen::pen(double width, const ::color& color)
    : pen(width, make_spbrush<color_brush>(color))
{}

pen::pen(double width, const brush_sptr& brush)
    : impl_(new implement)
{
    impl_->contour_.width(width);
    impl_->brush_ = brush;
}

pen::pen(double width, const ::color& color, initlist_type initlist)
    : pen(width, make_spbrush<color_brush>(color), initlist)
{}

pen::pen(double width, const brush_sptr& brush, initlist_type initlist)
    : pen(width, brush)
{
    dash_array(initlist);
}

pen::pen(const ::color& color)
    : pen(1, color)
{}

pen::pen(const brush_sptr& brush)
    : pen(1, brush)
{}

pen::pen(const pen& pen)
    : impl_(new implement)
{
    operator=(pen);
}

pen::~pen(void)
{
    delete impl_;
}

pen& pen::operator=(const pen& pen)
{
    if (this != &pen)
    {
        impl_->brush_       = pen.impl_->brush_;
        impl_->contour_     = pen.impl_->contour_;
        impl_->dashed_line_ = pen.impl_->dashed_line_;
    }

    return *this;
}

double pen::width(void) const
{
    return impl_->contour_.width();
}

brush_sptr pen::brush(void) const
{
    return impl_->brush_;
}

pen::Cap pen::cap(void) const
{
    switch (impl_->contour_.cap())
    {
    default:
    case contour::lcButt:   return cButt;
    case contour::lcSquare: return cSquare;
    case contour::lcRound:  return cRound;
    }
}

pen::Joint pen::joint(void) const
{
    switch (impl_->contour_.joint())
    {
    default:
    case contour::ljBevel: return jBevel;
    case contour::ljRound: return jRound;
    case contour::ljMiter: return jMiter;
    }
}

int pen::dash_array_size(void) const
{
    return !impl_->dashed_line_ ? 0 : impl_->dashed_line_->dash_array_size();
}

double pen::dash(int index) const
{
    return !impl_->dashed_line_ ? 0 : impl_->dashed_line_->dash(index);
}

double pen::offset(void) const
{
    return !impl_->dashed_line_ ? 0 : impl_->dashed_line_->offset();
}

void pen::width(double width)
{
    impl_->contour_.width(width);
}

void pen::color(const ::color& color)
{
    impl_->brush_ = make_spbrush<color_brush>(color);
}

void pen::brush(const brush_sptr& brush)
{
    impl_->brush_ = brush;
}

void pen::cap(Cap cap)
{
    switch (cap)
    {
    default:
    case cButt:   impl_->contour_.cap(contour::lcButt);   break;
    case cSquare: impl_->contour_.cap(contour::lcSquare); break;
    case cRound:  impl_->contour_.cap(contour::lcRound);  break;
    }
}

void pen::joint(Joint joint)
{
    switch (joint)
    {
    default:
    case jBevel: impl_->contour_.joint(contour::ljBevel); break;
    case jRound: impl_->contour_.joint(contour::ljRound); break;
    case jMiter: impl_->contour_.joint(contour::ljMiter); break;
    }
}

void pen::dash_array(initlist_type il)
{
    dash_array(il.begin(), il.size());
}

void pen::dash_array(const double* dash_array, unsigned int size)
{
    if (impl_->dashed_line_)
        impl_->dashed_line_->dash_array(dash_array, size);
    else
        impl_->dashed_line_.reset(new dashed_line(dash_array, size, false));
}

void pen::offset(double offset)
{
    if (impl_->dashed_line_)
        impl_->dashed_line_->offset(offset);
}
