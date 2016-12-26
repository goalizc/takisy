#ifndef bezier_h_20130823
#define bezier_h_20130823

#include <takisy/cgl/path/path.h>
#include <takisy/cgl/path/vertices.h>

class bezier
{
    class implement;

public:
    bezier(void);
    bezier(const bezier& bezier);
   ~bezier(void);

    bezier& operator=(const bezier& bezier);

public:
    bezier& depth(unsigned int depth);
    bezier& angle_tolerance(double angle_tolerance);
    bezier& distance_tolerance(double distance_tolerance);

    unsigned int depth(void) const;
    double angle_tolerance(void) const;
    double distance_tolerance(void) const;

public:
    vertices build(const path::vertex_type& _1, const path::vertex_type& _2,
                   const path::vertex_type& _3);
    vertices build(path::vertex_type::axis_type _1x,
                   path::vertex_type::axis_type _1y,
                   path::vertex_type::axis_type _2x,
                   path::vertex_type::axis_type _2y,
                   path::vertex_type::axis_type _3x,
                   path::vertex_type::axis_type _3y);
    vertices build(const path::vertex_type& _1, const path::vertex_type& _2,
                   const path::vertex_type& _3, const path::vertex_type& _4);
    vertices build(path::vertex_type::axis_type _1x,
                   path::vertex_type::axis_type _1y,
                   path::vertex_type::axis_type _2x,
                   path::vertex_type::axis_type _2y,
                   path::vertex_type::axis_type _3x,
                   path::vertex_type::axis_type _3y,
                   path::vertex_type::axis_type _4x,
                   path::vertex_type::axis_type _4y);

private:
    class implement* impl_;
};

class bezier_fitting_curve
{
    class implement;

public:
    bezier_fitting_curve(void);
    bezier_fitting_curve(double k, bool closed);
    bezier_fitting_curve(const bezier_fitting_curve& bfc);
   ~bezier_fitting_curve(void);

    bezier_fitting_curve& operator=(const bezier_fitting_curve& bfc);

public:
    bezier_fitting_curve& depth(unsigned int depth);
    bezier_fitting_curve& angle_tolerance(double angle_tolerance);
    bezier_fitting_curve& distance_tolerance(double distance_tolerance);
    bezier_fitting_curve& k(double k);
    bezier_fitting_curve& closed(bool closed);

    unsigned int depth(void) const;
    double angle_tolerance(void) const;
    double distance_tolerance(void) const;
    double k(void) const;
    bool closed(void) const;

public:
    vertices build(const path& path);
    vertices build(const path::vertex_type* vertices, unsigned int size);

private:
    class implement* impl_;
};

#endif //bezier_h_20130823
