#include <takisy/core/math.h>
#include <takisy/cgl/path/vertex_set.h>
#include <takisy/cgl/path/builder/ellipse.h>

class ellipse::implement
{
    friend class ellipse;

public:
    implement(path::vertex_type::axis_type center_x,
              path::vertex_type::axis_type center_y,
              double _width, double _height)
        : center_(center_x, center_y)
    {
        width(_width);
        height(_height);
    }

public:
    inline void width(double width)
    {
        width_ = math::abs(width);
    }

    inline void height(double height)
    {
        height_ = math::abs(height);
    }

    void build(double angle_from, double angle_to, bool is_sector)
    {
        result_.clear();

        if ((width_ == 0 && height_ == 0) || angle_from == angle_to)
        {
            result_.append(center_);
            return;
        }

        if (math::abs(angle_to - angle_from) >= math::deg2rad(360))
        {
            angle_from = 0;
            angle_to   = math::deg2rad(360);
        }

        double s = width_ + height_, step = math::deg2rad(30);
        while (s * (step /= 2) > math::deg2rad(600) && step > math::deg2rad(3));

        double angle;
        if (angle_from < angle_to)
            for (angle = angle_from; angle < angle_to; angle += step)
                result_.append(edge_vertex(angle));
        else
            for (angle = angle_from; angle > angle_to; angle -= step)
                result_.append(edge_vertex(angle));

        result_.append(edge_vertex(angle_to));

        if (is_sector)
            result_.append(center_);
    }

    inline double calculate_angle_to(double angle_from, double segment_length)
    {
        return (math::pi / 360) * segment_length /
                    ((width_ + height_) * 1.5 - math::sqrt(width_ * height_));
    }

private:
    inline path::vertex_type edge_vertex(double angle)
    {
        return path::vertex_type(center_.x + math::cos(angle) * width_,
                                 center_.y + math::sin(angle) * height_);
    }

private:
    vertex_set        result_;
    path::vertex_type center_;
    double            width_, height_;
};

ellipse::ellipse(void)
    : ellipse(0, 0, 0, 0)
{}

ellipse::ellipse(const path::vertex_type& center, double width, double height)
    : ellipse(center.x, center.y, width, height)
{}

ellipse::ellipse(path::vertex_type::axis_type center_x,
                 path::vertex_type::axis_type center_y,
                 double width, double height)
    : impl_(new implement(center_x, center_y, width, height))
{}

ellipse::ellipse(const ellipse& _ellipse)
    : ellipse(_ellipse.impl_->center_.x, _ellipse.impl_->center_.y,
              _ellipse.impl_->width_, _ellipse.impl_->height_)
{}

ellipse::~ellipse(void)
{
    delete impl_;
}

ellipse& ellipse::operator=(const ellipse& ellipse)
{
    if (this != &ellipse)
    {
        impl_->center_ = ellipse.impl_->center_;
        impl_->width_  = ellipse.impl_->width_;
        impl_->height_ = ellipse.impl_->height_;
    }

    return *this;
}

ellipse& ellipse::center(const path::vertex_type& center)
{
    impl_->center_ = center;

    return *this;
}

ellipse& ellipse::width(double width)
{
    impl_->width(width);

    return *this;
}

ellipse& ellipse::height(double height)
{
    impl_->height(height);

    return *this;
}

const path::vertex_type& ellipse::center(void) const
{
    return impl_->center_;
}

double ellipse::width(void) const
{
    return impl_->width_;
}

double ellipse::height(void) const
{
    return impl_->height_;
}

const path& ellipse::build(void)
{
    impl_->build(0, math::deg2rad(360), false);

    return impl_->result_;
}

const path& ellipse::build_sector(double angle_from, double angle_to)
{
    impl_->build(angle_from, angle_to, true);

    return impl_->result_;
}

const path& ellipse::build_sector_v2(double angle_from,
                                     double segment_length)
{
    build_sector(angle_from,
                 impl_->calculate_angle_to(angle_from, segment_length));

    return impl_->result_;
}

const path& ellipse::build_segment(double angle_from, double angle_to)
{
    impl_->build(angle_from, angle_to, false);

    return impl_->result_;
}

const path& ellipse::build_segment_v2(double angle_from,
                                      double segment_length)
{
    build_segment(angle_from,
                  impl_->calculate_angle_to(angle_from, segment_length));

    return impl_->result_;
}
