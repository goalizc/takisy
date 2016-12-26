#ifndef rectangle_h_20130719
#define rectangle_h_20130719

#include <takisy/cgl/basic/rect.h>
#include <takisy/cgl/path/path.h>
#include <takisy/cgl/path/vertices.h>

class rectangle
{
public:
    static vertices build(path::vertex_type::axis_type left,
                          path::vertex_type::axis_type top,
                          path::vertex_type::axis_type right,
                          path::vertex_type::axis_type bottom);
    static vertices build(const path::vertex_type& left_top,
                          const path::vertex_type& right_bottom);
    static vertices build(const rectf& rect);

    static vertices build(path::vertex_type::axis_type left,
                          path::vertex_type::axis_type top,
                          path::vertex_type::axis_type right,
                          path::vertex_type::axis_type bottom,
                          double radius);
    static vertices build(const path::vertex_type& left_top,
                          const path::vertex_type& right_bottom,
                          double radius);
    static vertices build(const rectf& rect, double radius);

    static vertices build(path::vertex_type::axis_type left,
                          path::vertex_type::axis_type top,
                          path::vertex_type::axis_type right,
                          path::vertex_type::axis_type bottom,
                          double width, double height);
    static vertices build(const path::vertex_type& left_top,
                          const path::vertex_type& right_bottom,
                          double width, double height);
    static vertices build(const rectf& rect,
                          double width, double height);
};

#endif //rectangle_h_20130719
