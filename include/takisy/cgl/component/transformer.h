#ifndef transformer_h_20131029
#define transformer_h_20131029

#include <takisy/cgl/path/path.h>

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
    transformer& offset(double offset_x, double offset_y);

    transformer& scale(double scale);
    transformer& scale(double scale_x, double scale_y);
    transformer& scale(const path::vertex_type& center, double scale);
    transformer& scale(const path::vertex_type& center,
                       double scale_x,
                       double scale_y);
    transformer& scale(double x, double y, double scale);
    transformer& scale(double x, double y, double scale_x, double scale_y);

    transformer& rotate(double angle);
    transformer& rotate(const path::vertex_type& center, double angle);
    transformer& rotate(double x, double y, double angle);

    transformer& reflect(const path::vertex_type& axis);
    transformer& reflect(double x, double y);

    transformer& shear(double shear_x, double shear_y);
    transformer& shear(const path::vertex_type& center,
                       double shear_x,
                       double shear_y);
    transformer& shear(double x, double y, double shear_x, double shear_y);

public:
    transformer& inverse(void);
    transformer  inversed(void) const;

public:
    path::vertex_type transform(const path::vertex_type& point) const;
    path::vertex_type transform(const path::vertex_type::axis_type x,
                                const path::vertex_type::axis_type y) const;
          const path& transform(const path& path) const;
          const path& transform(const path::vertex_type* vertices,
                                unsigned int size) const;
         const paths& transform(const paths& paths) const;
         const paths& transform(const path* paths, unsigned int size) const;

private:
    class implement* impl_;
};

#endif //transformer_h_20131029
