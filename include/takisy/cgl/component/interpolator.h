#ifndef interpolator_h_20131112
#define interpolator_h_20131112

#include <takisy/cgl/basic/point.h>

class interpolator
{
    class implement;

public:
    typedef pointf knot_type;

public:
    interpolator(void);
    interpolator(const interpolator& interpolator);
   ~interpolator(void);

    interpolator& operator=(const interpolator& interpolator);

public:
    interpolator& clear(void);

    interpolator& clamp_fy(double min, double max);
    interpolator& min_fy(double min_fy);
    interpolator& max_fy(double max_fy);
    interpolator& add_knot(const knot_type& knot);
    interpolator& add_knot(double x, double y);

    double min_fy(void) const;
    double max_fy(void) const;

    unsigned int knots_size(void) const;
    knot_type    fetch_knot(unsigned int index) const;

public:
    double fy(double x) const;

private:
    class implement* impl_;
};

#endif //interpolator_h_20131112
