#ifndef ellipse_h_20130822
#define ellipse_h_20130822

#include <takisy/cgl/path/path.h>
#include <takisy/cgl/path/vertices.h>

class ellipse
{
    class implement;

public:
    ellipse(void);
    ellipse(const path::vertex_type& center, double width, double height);
    ellipse(path::vertex_type::axis_type center_x,
            path::vertex_type::axis_type center_y, double width, double height);
    ellipse(const ellipse& ellipse);
   ~ellipse(void);

    ellipse& operator=(const ellipse& ellipse);

public:
    ellipse& center(const path::vertex_type& center);
    ellipse& width(double width);
    ellipse& height(double height);

    const path::vertex_type& center(void) const;
    double width(void) const;
    double height(void) const;

public:
    vertices build(void);
    vertices build_sector(double angle_from, double angle_to);
    vertices build_sector_v2(double angle_from, double segment_length);
    vertices build_segment(double angle_from, double angle_to);
    vertices build_segment_v2(double angle_from, double segment_length);

private:
    class implement* impl_;
};

#endif //ellipse_h_20130822
