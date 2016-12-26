#include <takisy/cgl/path/vertices_wrapper.h>

class vertices_wrapper::implement
{
    friend class vertices_wrapper;

public:
    implement(const path::vertex_type* vertices, unsigned int size)
        : size_(size), vertices_(vertices)
    {}

private:
    unsigned int size_;
    const path::vertex_type* vertices_;
};

vertices_wrapper::vertices_wrapper(const path::vertex_type* vertices,
                                   unsigned int size)
    : impl_(new implement(vertices, size))
{}

vertices_wrapper::vertices_wrapper(const vertices_wrapper& vw)
    : vertices_wrapper(vw.impl_->vertices_, vw.impl_->size_)
{}

vertices_wrapper::~vertices_wrapper(void)
{
    delete impl_;
}

vertices_wrapper& vertices_wrapper::operator=(const vertices_wrapper& vw)
{
    if (this != &vw)
    {
        impl_->size_     = vw.impl_->size_;
        impl_->vertices_ = vw.impl_->vertices_;
    }

    return *this;
}

unsigned int vertices_wrapper::size(void) const
{
    return impl_->size_;
}

path::vertex_type vertices_wrapper::at(unsigned int index) const
{
    return impl_->vertices_[index];
}

