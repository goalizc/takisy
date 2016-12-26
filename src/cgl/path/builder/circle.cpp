#include <takisy/cgl/path/builder/ellipse.h>
#include <takisy/cgl/path/builder/circle.h>

class circle::implement
{
    friend class circle;

public:
    implement(path::vertex_type::axis_type center_x,
              path::vertex_type::axis_type center_y, double radius)
        : ellipse_(center_x, center_y, radius, radius)
    {}

private:
    ellipse ellipse_;
};

circle::circle(void)
    : circle(0, 0, 0)
{}

circle::circle(const path::vertex_type& center, double radius)
    : circle(center.x, center.y, radius)
{}

circle::circle(path::vertex_type::axis_type center_x,
               path::vertex_type::axis_type center_y, double radius)
    : impl_(new implement(center_x, center_y, radius))
{}

circle::circle(const circle& _circle)
    : circle()
{
    operator=(_circle);
}

circle::~circle(void)
{
    delete impl_;
}

circle& circle::operator=(const circle& circle)
{
    if (this != &circle)
        impl_->ellipse_ = circle.impl_->ellipse_;

    return *this;
}

circle& circle::center(const path::vertex_type& center)
{
    impl_->ellipse_.center(center);

    return *this;
}

circle& circle::radius(double radius)
{
    impl_->ellipse_.width(radius);
    impl_->ellipse_.height(radius);

    return *this;
}

const path::vertex_type& circle::center(void) const
{
    return impl_->ellipse_.center();
}

double circle::radius(void) const
{
    return impl_->ellipse_.width();
}

vertices circle::build(void)
{
    return impl_->ellipse_.build();
}

vertices circle::build_sector(double angle_from, double angle_to)
{
    return impl_->ellipse_.build_sector(angle_from, angle_to);
}

vertices circle::build_sector_v2(double angle_from, double segment_length)
{
    return impl_->ellipse_.build_sector_v2(angle_from, segment_length);
}

vertices circle::build_segment(double angle_from, double angle_to)
{
    return impl_->ellipse_.build_segment(angle_from, angle_to);
}

vertices circle::build_segment_v2(double angle_from, double segment_length)
{
    return impl_->ellipse_.build_segment_v2(angle_from, segment_length);
}
