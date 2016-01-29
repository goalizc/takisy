#ifndef vertices_wrapper_h_20131024
#define vertices_wrapper_h_20131024

#include <takisy/cgl/path/path.h>

class vertices_wrapper : public path
{
    class implement;

public:
    vertices_wrapper(const path::vertex_type* vertices, unsigned int size);
    vertices_wrapper(const vertices_wrapper& ps);
   ~vertices_wrapper(void);

    vertices_wrapper& operator=(const vertices_wrapper& ps);

public:
    unsigned int size(void) const override;
    path::vertex_type fetch_vertex(unsigned int index) const override;

private:
    class implement* impl_;
};

#endif //vertices_wrapper_h_20131024
