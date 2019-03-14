#ifndef transformer_h_20131029
#define transformer_h_20131029

#include <takisy/cgl/path/path.h>
#include <takisy/cgl/path/vertices.h>

class transformer
{
    class implement;

public:
    transformer(void);
    transformer(const transformer& transformer);
   ~transformer(void);

    transformer& operator=(const transformer& transformer);

public:
    transformer& reset(void);
    transformer& offset(const path::vertex_type& offset);
    transformer& offset(double dx, double dy);
    transformer& scale(double scale);
    transformer& scale(double fx, double fy);
    transformer& scale(const path::vertex_type& center, double scale);
    transformer& scale(const path::vertex_type& center, double fx, double fy);
    transformer& scale(double x, double y, double scale);
    transformer& scale(double x, double y, double fx, double fy);
    transformer& rotate(double angle);
    transformer& rotate(const path::vertex_type& center, double angle);
    transformer& rotate(double x, double y, double angle);
    transformer& reflect(const path::vertex_type& axis);
    transformer& reflect(double x, double y);
    transformer& shear(double fx, double fy);
    transformer& shear(const path::vertex_type& center, double fx, double fy);
    transformer& shear(double x, double y, double fx, double fy);

public:
    transformer& inverse(void);
    transformer  inversed(void) const;

public:
    path::vertex_type transform(const path::vertex_type& point) const;
    path::vertex_type transform(const path::vertex_type::axis_type x,
                                const path::vertex_type::axis_type y) const;
    vertices          transform(const path& path) const;
    vertices          transform(const path::vertex_type* vertices,
                                unsigned int size) const;
    paths             transform(const paths& paths) const;
    paths             transform(const path* paths, unsigned int size) const;

private:
    class implement* impl_;
};

#endif //transformer_h_20131029
