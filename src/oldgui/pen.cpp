#include <takisy/oldgui/basic/pen.h>
#include "impl/pen.hpp"

Pen::Pen(void)
    : Pen(1)
{}

Pen::Pen(double width)
    : Pen(width, Color::black())
{}

Pen::Pen(Color color)
    : Pen(1, color)
{}

Pen::Pen(const std::shared_ptr<Brush>& brush)
    : Pen(1, brush)
{}

Pen::Pen(double width, Color color)
    : Pen(width, std::shared_ptr<Brush>(new ColorBrush(color)))
{}

Pen::Pen(double width, const std::shared_ptr<Brush>& brush)
    : impl_(new Implement)
{
    impl_->contour_.width(width);
    impl_->brush_ = brush;
}

Pen::Pen(const Pen& pen)
    : impl_(new Implement)
{
    operator=(pen);
}

Pen::~Pen(void)
{
    delete impl_;
}

Pen& Pen::operator=(const Pen& pen)
{
    if (this != &pen)
    {
        impl_->brush_       = pen.impl_->brush_;
        impl_->contour_     = pen.impl_->contour_;
        impl_->dashed_line_ = pen.impl_->dashed_line_;
    }

    return *this;
}

double Pen::width(void) const
{
    return impl_->contour_.width();
}

Brush* Pen::brush(void) const
{
    return impl_->brush_.get();
}

Pen::Cap Pen::cap(void) const
{
    switch (impl_->contour_.cap())
    {
    default:
    case contour::lcButt:   return cButt;
    case contour::lcSquare: return cSquare;
    case contour::lcRound:  return cRound;
    }
}

Pen::Joint Pen::joint(void) const
{
    switch (impl_->contour_.joint())
    {
    default:
    case contour::ljBevel: return jBevel;
    case contour::ljRound: return jRound;
    case contour::ljMiter: return jMiter;
    }
}

int Pen::dashArrayCount(void) const
{
    return !impl_->dashed_line_ ? 0 : impl_->dashed_line_->dash_array_size();
}

double Pen::dash(int index) const
{
    return !impl_->dashed_line_ ? 0 : impl_->dashed_line_->dash(index);
}

double Pen::offset(void) const
{
    return !impl_->dashed_line_ ? 0 : impl_->dashed_line_->offset();
}

void Pen::width(double width)
{
    impl_->contour_.width(width);
}

void Pen::brush(const std::shared_ptr<Brush>& brush)
{
    impl_->brush_ = brush;
}

void Pen::cap(Cap cap)
{
    switch (cap)
    {
    default:
    case cButt:   impl_->contour_.cap(contour::lcButt);   break;
    case cSquare: impl_->contour_.cap(contour::lcSquare); break;
    case cRound:  impl_->contour_.cap(contour::lcRound);  break;
    }
}

void Pen::joint(Joint joint)
{
    switch (joint)
    {
    default:
    case jBevel: impl_->contour_.joint(contour::ljBevel); break;
    case jRound: impl_->contour_.joint(contour::ljRound); break;
    case jMiter: impl_->contour_.joint(contour::ljMiter); break;
    }
}

void Pen::dashArray(std::initializer_list<double> il)
{
    dashArray(il.begin(), il.size());
}

void Pen::dashArray(const double* dash_array, unsigned int size)
{
    if (impl_->dashed_line_)
        impl_->dashed_line_->dash_array(dash_array, size);
    else
        impl_->dashed_line_.reset(new dashed_line(dash_array, size, false));
}

void Pen::offset(double offset)
{
    if (impl_->dashed_line_)
        impl_->dashed_line_->offset(offset);
}
