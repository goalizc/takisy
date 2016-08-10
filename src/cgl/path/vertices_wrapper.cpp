#include <takisy/cgl/path/vertices_wrapper.h>

class vertices_wrapper::implement
{
    friend class vertices_wrapper;

public:
    implement(const path::vertex_type* vertices, unsigned int size)
        : vertices_(vertices), size_(size)
    {}

private:
    const path::vertex_type* vertices_;
    int size_;
};

vertices_wrapper::vertices_wrapper(const path::vertex_type* vertices,
                                   unsigned int size)
    : impl_(new implement(vertices, size))
{}

vertices_wrapper::vertices_wrapper(const vertices_wrapper& ps)
    : vertices_wrapper(ps.impl_->vertices_, ps.impl_->size_)
{}

vertices_wrapper::~vertices_wrapper(void)
{
    delete impl_;
}

vertices_wrapper& vertices_wrapper::operator=(const vertices_wrapper& ps)
{
    if (this != &ps)
    {
        impl_->vertices_ = ps.impl_->vertices_;
        impl_->size_     = ps.impl_->size_;
    }

    return *this;
}

unsigned int vertices_wrapper::size(void) const
{
    return impl_->size_;
}

path::vertex_type vertices_wrapper::fetch_vertex(unsigned int index) const
{
    return impl_->vertices_[index];
}

