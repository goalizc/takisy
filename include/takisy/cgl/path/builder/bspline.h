#ifndef bspline_h_20140516
#define bspline_h_20140516

#include <takisy/cgl/path/path.h>
#include <takisy/cgl/path/vertices.h>

class bspline
{
    class implement;

public:
    enum Type { bstOpen, bstClamp, bstClose };

public:
    bspline(void);
    bspline(unsigned int degree, Type type);
    bspline(unsigned int degree, Type type, double intermediate_point_count);
    bspline(const bspline& bspline);
   ~bspline(void);

    bspline& operator=(const bspline& bspline);

public:
    bspline& degree(unsigned int degree);
    bspline& conic(void);
    bspline& cobic(void);
    bspline& type(Type type);
    bspline& intermediate_point_count(double count);

    unsigned int degree(void) const;
    Type type(void) const;
    double intermediate_point_count(void) const;

public:
    vertices build(const path& path);
    vertices build(const path::vertex_type* vertices, unsigned int size);

private:
    class implement* impl_;
};

#endif // bspline_h_20140516
