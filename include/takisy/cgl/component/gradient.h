#ifndef gradient_h_20131112
#define gradient_h_20131112

#include <takisy/cgl/basic/point.h>

class gradient
{
    class implement;

public:
    enum GradientType
    {
        gtLinear    = 0,
        gtCircle    = 1,
        gtMinXY     = 2,
        gtMaxXY     = 3,
        gtDiamond   = 4,
        gtSqrtXY    = 5,
        gtConic     = 6,
        gtCustom    = 7,
    };

    typedef double (*FnCalculator)(double x, double y);

public:
    gradient(void);
    gradient(const pointf& center, double radius, double angle,
             GradientType gradient_type = gtLinear);
    gradient(pointf::axis_type center_x, pointf::axis_type center_y,
             double radius, double angle,
             GradientType gradient_type = gtLinear);
    gradient(const gradient& lg);
   ~gradient(void);

    gradient& operator=(const gradient& lg);

public:
    gradient& gradient_type(GradientType gradient_type);
    gradient& center(const pointf& center);
    gradient& center(pointf::axis_type center_x, pointf::axis_type center_y);
    gradient& radius(double radius);
    gradient& angle(double angle);
    gradient& custom_calculator(FnCalculator calculator);

    GradientType gradient_type(void) const;
    pointf       center(void) const;
    double       radius(void) const;
    double       angle(void) const;
    FnCalculator custom_calculator(void) const;

public:
    double gradual_value(const pointf& dott) const;
    double gradual_value(double x, double y) const;

    // mapping to [0, 1)
    double normalized_gradual_value(const pointf& dott) const;
    double normalized_gradual_value(double x, double y) const;

private:
    class implement* impl_;
};

#endif //gradient_h_20131112
