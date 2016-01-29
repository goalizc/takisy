#ifndef contour_h_20130820
#define contour_h_20130820

#include <takisy/cgl/path/path.h>

class contour
{
    class implement;

public:
    enum LineCap
    {
        lcButt      = 0,
        lcSquare    = 1,
        lcRound     = 2,
    };

    enum LineJoint
    {
        ljBevel     = 0,
        ljRound     = 1,
        ljMiter     = 2,
    };

public:
    contour(void);
    contour(double width, LineCap cap, LineJoint joint, bool closed);
    contour(const contour& ol);
   ~contour(void);

    contour& operator=(const contour& ol);

public:
    contour& width(double width);
    contour& cap(LineCap cap);
    contour& joint(LineJoint joint);
    contour& closed(bool closed);

    double    width(void) const;
    LineCap   cap(void) const;
    LineJoint joint(void) const;
    bool      closed(void) const;

public:
    const paths& build(const path& path);
    const paths& build(const path::vertex_type* vertices, unsigned int size);
    const paths& build(const paths& paths);
    const paths& build(const path* paths, unsigned int size);

private:
    class implement* impl_;
};

#endif //contour_h_20130820
