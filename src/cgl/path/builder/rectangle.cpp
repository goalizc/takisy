#include <takisy/core/algorithm.h>
#include <takisy/core/math.h>
#include <takisy/cgl/path/vertex_set.h>
#include <takisy/cgl/path/builder/ellipse.h>
#include <takisy/cgl/path/builder/rectangle.h>

const path& rectangle::build(path::vertex_type::axis_type left,
                             path::vertex_type::axis_type top,
                             path::vertex_type::axis_type right,
                             path::vertex_type::axis_type bottom)
{
    static vertex_set rectangle;

    rectangle.clear();
    rectangle.append(left,  top);
    rectangle.append(right, top);
    rectangle.append(right, bottom);
    rectangle.append(left,  bottom);

    return rectangle;
}

const path& rectangle::build(const path::vertex_type& left_top,
                             const path::vertex_type& right_bottom)
{
    return build(left_top.x, left_top.y, right_bottom.x, right_bottom.y);
}

const path& rectangle::build(const rectf& rect)
{
    return build(rect.left, rect.top, rect.right, rect.bottom);
}

const path& rectangle::build(path::vertex_type::axis_type left,
                             path::vertex_type::axis_type top,
                             path::vertex_type::axis_type right,
                             path::vertex_type::axis_type bottom, double radius)
{
    return build(left, top, right, bottom, radius, radius);
}

const path& rectangle::build(const path::vertex_type& left_top,
                             const path::vertex_type& right_bottom,
                             double radius)
{
    return build(left_top, right_bottom, radius, radius);
}

const path& rectangle::build(const rectf& rect, double radius)
{
    return build(rect.left, rect.top, rect.right, rect.bottom, radius);
}

const path& rectangle::build(path::vertex_type::axis_type l,
                             path::vertex_type::axis_type t,
                             path::vertex_type::axis_type r,
                             path::vertex_type::axis_type b,
                             double width, double height)
{
    static vertex_set round_rectangle;

    if (l > r) algorithm::swap(l, r);
    if (t > b) algorithm::swap(t, b);

    if (r - l <  width * 2)  width = (r - l) / 2;
    if (b - t < height * 2) height = (b - t) / 2;

    round_rectangle.clear();

    round_rectangle.append(ellipse(l + width, t + height, width, height)
                   .build_segment(math::deg2rad(180), math::deg2rad(270)));
    round_rectangle.append(ellipse(r - width, t + height, width, height)
                   .build_segment(math::deg2rad(270), math::deg2rad(360)));
    round_rectangle.append(ellipse(r - width, b - height, width, height)
                   .build_segment(math::deg2rad(  0), math::deg2rad( 90)));
    round_rectangle.append(ellipse(l + width, b - height, width, height)
                   .build_segment(math::deg2rad( 90), math::deg2rad(180)));

    return round_rectangle;
}

const path& rectangle::build(const path::vertex_type& left_top,
                             const path::vertex_type& right_bottom,
                             double width, double height)
{
    return build(left_top.x, left_top.y,
                 right_bottom.x, right_bottom.y, width, height);
}

const path& rectangle::build(const rectf& rect, double width, double height)
{
    return build(rect.left, rect.top, rect.right, rect.bottom, width, height);
}
