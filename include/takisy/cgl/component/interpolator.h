#ifndef interpolator_h_20131112
#define interpolator_h_20131112

#include <vector>
#include <takisy/cgl/basic/point.h>

class interpolator
{
    class implement;

public:
    typedef pointf knot_type;
    typedef std::vector<knot_type> knots_type;

public:
    interpolator(void);
    interpolator(const interpolator& interpolator);
   ~interpolator(void);

    interpolator& operator=(const interpolator& interpolator);

public:
    double       lower(void) const;
    double       upper(void) const;
    unsigned int knots_size(void) const;
    knot_type    knot(unsigned int index) const;
    knots_type   knots(void) const;

public:
    interpolator& clear(void);
    interpolator& clamp(double lower, double upper);
    interpolator& lower(double lower);
    interpolator& upper(double upper);
    interpolator& add_knot(const knot_type& knot);
    interpolator& add_knot(double x, double fy);

public:
    double fy(double x) const;
    double operator[](double x) const;

private:
    class implement* impl_;
};

#endif //interpolator_h_20131112
