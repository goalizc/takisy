#ifndef path_h_20130802
#define path_h_20130802

#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/basic/rect.h>

class paths;

enum ClipType
{
    ctDiff  = 0,
    ctInt   = 1,
    ctXor   = 2,
    ctUnion = 3,
};

class path
{
public:
    typedef pointf vertex_type;

public:
    virtual ~path(void);

public:
    virtual unsigned int size(void) const = 0;
    virtual vertex_type  fetch_vertex(unsigned int index) const = 0;

public:
    const paths& clip(const rectf& rect) const;
    const paths& clip(vertex_type::axis_type left,
                      vertex_type::axis_type top,
                      vertex_type::axis_type right,
                      vertex_type::axis_type bottom) const;
    const paths& clip(const path& edge) const;
    const paths& clip(const path& edge, ClipType clip_type) const;

    // Determine whether the point within the path.
    bool pnpoly(const vertex_type& point) const;
    bool pnpoly(vertex_type::axis_type x, vertex_type::axis_type y) const;

public:
    rectf bbox(void) const;
};

class paths
{
    class implement;

public:
    paths(void);
    paths(const paths& paths);
   ~paths(void);

    paths& operator=(const paths& paths);

public:
    unsigned int size(void) const;
    path* fetch_path(unsigned int index) const;

public:
    const paths& clip(const rectf& rect) const;
    const paths& clip(path::vertex_type::axis_type left,
                      path::vertex_type::axis_type top,
                      path::vertex_type::axis_type right,
                      path::vertex_type::axis_type bottom) const;
    const paths& clip(const path& edge) const;
    const paths& clip(const path& edge, ClipType clip_type) const;

    // Determine whether the point within the paths.
    bool pnpoly(const path::vertex_type& point) const;
    bool pnpoly(path::vertex_type::axis_type x,
                path::vertex_type::axis_type y) const;

public:
    rectf bbox(void) const;

public:
    void clear(void);
    void append(path* path);

private:
    class implement* impl_;
};

typedef path  polygon;
typedef paths polygons;

#endif //path_h_20130802
