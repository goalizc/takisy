#ifndef vertices_h_20130820
#define vertices_h_20130820

#include <takisy/cgl/path/path.h>

class vertices : public path
{
    class implement;

public:
    vertices(void);
    vertices(const path& path);
    vertices(const path::vertex_type* vertices, unsigned int size);
    vertices(const vertices& vertices);
   ~vertices(void);

    vertices& operator=(const path& path);
    vertices& operator=(const vertices& vertices);

public:
    unsigned int             size(void) const override;
    path::vertex_type        at(unsigned int index) const override;
    path::vertex_type&       operator[](unsigned int index);
    const path::vertex_type& operator[](unsigned int index) const;

    path::vertex_type&       front(void);
    const path::vertex_type& front(void) const;
    path::vertex_type&       back(void);
    const path::vertex_type& back(void) const;

public:
    bool empty(void) const;

public:
    void clear(void);
    void insert(unsigned int index, const path::vertex_type& vertex);
    void insert(unsigned int index,
                path::vertex_type::axis_type x,
                path::vertex_type::axis_type y);
    void insert(unsigned int index, const path& path);
    void insert(unsigned int index, const vertices& vertices);
    void insert(unsigned int index,
                const path::vertex_type* vertices, unsigned int size);
    void append(const path::vertex_type& vertex);
    void append(path::vertex_type::axis_type x, path::vertex_type::axis_type y);
    void append(const path& path);
    void append(const vertices& vertices);
    void append(const path::vertex_type* vertices, unsigned int size);
    void pop(void);
    void remove(unsigned int index);
    void remove(unsigned int from, unsigned int to);
    void unique(void);

private:
    class implement* impl_;
};

#endif //vertices_h_20130820
