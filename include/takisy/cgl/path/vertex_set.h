#ifndef vertex_set_h_20130820
#define vertex_set_h_20130820

#include <takisy/cgl/path/path.h>

class vertex_set : public path
{
    class implement;

public:
    vertex_set(void);
    vertex_set(const path& path);
    vertex_set(const path::vertex_type* vertices, unsigned int size);
    vertex_set(const vertex_set& vertex_set);
   ~vertex_set(void);

    vertex_set& operator=(const path& path);
    vertex_set& operator=(const vertex_set& vertex_set);

public:
    unsigned int size(void) const override;
    path::vertex_type fetch_vertex(unsigned int index) const override;

    path::vertex_type& at(unsigned int index);
    const path::vertex_type& at(unsigned int index) const;
    path::vertex_type& operator[](unsigned int index);
    const path::vertex_type& operator[](unsigned int index) const;

    path::vertex_type& front(void);
    const path::vertex_type& front(void) const;
    path::vertex_type& back(void);
    const path::vertex_type& back(void) const;

public:
    bool empty(void) const;

public:
    void clear(void);
    void reverse(void);

    void insert(unsigned int index, const path::vertex_type& vertex);
    void insert(unsigned int index,
                path::vertex_type::axis_type x,
                path::vertex_type::axis_type y);
    void insert(unsigned int index, const path& path);
    void insert(unsigned int index, const vertex_set& vertex_set);
    void insert(unsigned int index, const path::vertex_type* vertices,
                unsigned int size);

    void append(const path::vertex_type& vertex);
    void append(path::vertex_type::axis_type x, path::vertex_type::axis_type y);
    void append(const path& path);
    void append(const vertex_set& vertex_set);
    void append(const path::vertex_type* vertices, unsigned int size);

    void remove(void);
    void remove(unsigned int index);
    void remove(unsigned int from, unsigned int to);

private:
    class implement* impl_;
};

#endif //vertex_set_h_20130820
