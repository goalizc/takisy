#ifndef rectangle_h_20130719
#define rectangle_h_20130719

#include <takisy/cgl/basic/rect.h>
#include <takisy/cgl/path/path.h>

class rectangle
{
public:
    static const path& build(path::vertex_type::axis_type left,
                             path::vertex_type::axis_type top,
                             path::vertex_type::axis_type right,
                             path::vertex_type::axis_type bottom);
    static const path& build(const path::vertex_type& left_top,
                             const path::vertex_type& right_bottom);
    static const path& build(const rectf& rect);

    static const path& build(path::vertex_type::axis_type left,
                             path::vertex_type::axis_type top,
                             path::vertex_type::axis_type right,
                             path::vertex_type::axis_type bottom,
                             double radius);
    static const path& build(const path::vertex_type& left_top,
                             const path::vertex_type& right_bottom,
                             double radius);
    static const path& build(const rectf& rect, double radius);

    static const path& build(path::vertex_type::axis_type left,
                             path::vertex_type::axis_type top,
                             path::vertex_type::axis_type right,
                             path::vertex_type::axis_type bottom,
                             double width, double height);
    static const path& build(const path::vertex_type& left_top,
                             const path::vertex_type& right_bottom,
                             double width, double height);
    static const path& build(const rectf& rect, double width, double height);
};

#endif //rectangle_h_20130719
