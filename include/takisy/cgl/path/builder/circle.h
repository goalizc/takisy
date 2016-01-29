#ifndef circlr_h_20130822
#define circlr_h_20130822

#include <takisy/cgl/path/path.h>

class circle
{
    class implement;

public:
    circle(void);
    circle(const path::vertex_type& center, double radius);
    circle(path::vertex_type::axis_type center_x,
           path::vertex_type::axis_type center_y, double radius);
    circle(const circle& circle);
   ~circle(void);

    circle& operator=(const circle& circle);

public:
    circle& center(const path::vertex_type& center);
    circle& radius(double radius);

    const path::vertex_type& center(void) const;
    double radius(void) const;

public:
    const path& build(void);
    const path& build_sector(double angle_from, double angle_to);
    const path& build_sector_v2(double angle_from, double segment_length);
    const path& build_segment(double angle_from, double angle_to);
    const path& build_segment_v2(double angle_from, double segment_length);

public:
    class implement* impl_;
};

#endif //circlr_h_20130822
