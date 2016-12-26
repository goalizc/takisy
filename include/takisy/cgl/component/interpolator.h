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
    double       lower_fy(void) const;
    double       upper_fy(void) const;
    unsigned int knots_size(void) const;
    knot_type    knot(unsigned int index) const;

public:
    interpolator& clear(void);
    interpolator& clamp_fy(double lower, double upper);
    interpolator& lower_fy(double lower_fy);
    interpolator& upper_fy(double upper_fy);
    interpolator& add_knot(const knot_type& knot);
    interpolator& add_knot(double x, double y);

public:
    double fy(double x) const;

private:
    class implement* impl_;
};

#endif //interpolator_h_20131112
