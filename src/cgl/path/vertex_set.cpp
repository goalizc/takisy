#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/path/vertices.h>

class vertices::implement
{
    friend class vertices;

public:
    implement(void)
    {}

    implement(unsigned int capacity)
    {
        vertices_.recapacity(capacity);
    }

private:
    stretchy_buffer<path::vertex_type> vertices_;
};

vertices::vertices(void)
    : impl_(new implement)
{}

vertices::vertices(const path& path)
    : impl_(new implement(path.size()))
{
    operator=(path);
}

vertices::vertices(const path::vertex_type* vertices, unsigned int size)
    : impl_(new implement(size))
{
    append(vertices, size);
}

vertices::vertices(const vertices& vs)
    : impl_(new implement(vs.size()))
{
    impl_->vertices_ = vs.impl_->vertices_;
}

vertices::~vertices(void)
{
    delete impl_;
}

vertices& vertices::operator=(const path& path)
{
    if (this != &path)
    {
        clear();
        append(path);
    }

    return *this;
}

vertices& vertices::operator=(const vertices& vs)
{
    if (this != &vs)
        impl_->vertices_ = vs.impl_->vertices_;

    return *this;
}

unsigned int vertices::size(void) const
{
    return impl_->vertices_.size();
}

path::vertex_type vertices::at(unsigned int index) const
{
    return impl_->vertices_[index];
}

path::vertex_type& vertices::operator[](unsigned int index)
{
    return impl_->vertices_[index];
}

const path::vertex_type& vertices::operator[](unsigned int index) const
{
    return impl_->vertices_[index];
}

path::vertex_type& vertices::front(void)
{
    return impl_->vertices_.front();
}

const path::vertex_type& vertices::front(void) const
{
    return impl_->vertices_.front();
}

path::vertex_type& vertices::back(void)
{
    return impl_->vertices_.back();
}

const path::vertex_type& vertices::back(void) const
{
    return impl_->vertices_.back();
}


bool vertices::empty(void) const
{
    return impl_->vertices_.empty();
}

void vertices::clear(void)
{
    impl_->vertices_.clear();
}

void vertices::insert(unsigned int index, const path::vertex_type& vertex)
{
    impl_->vertices_.insert(index, vertex);
}

void vertices::insert(unsigned int index,
                      path::vertex_type::axis_type x,
                      path::vertex_type::axis_type y)
{
    impl_->vertices_.insert(index, path::vertex_type(x, y));
}

void vertices::insert(unsigned int index, const path& path)
{
    stretchy_buffer<path::vertex_type> sbpath(path.size());

    for (unsigned int i = 0; i < path.size(); ++i)
        sbpath[i] = path[i];

    impl_->vertices_.insert(index, sbpath.data(), sbpath.size());
}

void vertices::insert(unsigned int index, const vertices& vs)
{
    impl_->vertices_.insert(index,
                            vs.impl_->vertices_.data(),
                            vs.impl_->vertices_.size());
}

void vertices::insert(unsigned int index,
                      const path::vertex_type* vertices, unsigned int size)
{
    impl_->vertices_.insert(index, vertices, size);
}

void vertices::append(const path::vertex_type& vertex)
{
    impl_->vertices_.append(vertex);
}

void vertices::append(path::vertex_type::axis_type x,
                      path::vertex_type::axis_type y)
{
    impl_->vertices_.append(path::vertex_type(x, y));
}

void vertices::append(const path& path)
{
    stretchy_buffer<path::vertex_type> sbpath(path.size());

    for (unsigned int i = 0; i < path.size(); ++i)
        sbpath[i] = path[i];

    impl_->vertices_.append(sbpath.data(), sbpath.size());
}

void vertices::append(const vertices& vs)
{
    impl_->vertices_.append(vs.impl_->vertices_.data(),
                            vs.impl_->vertices_.size());
}

void vertices::append(const path::vertex_type* vertices, unsigned int size)
{
    impl_->vertices_.append(vertices, size);
}

void vertices::pop(void)
{
    impl_->vertices_.remove();
}

void vertices::remove(unsigned int index)
{
    impl_->vertices_.remove(index);
}

void vertices::remove(unsigned int from, unsigned int to)
{
    impl_->vertices_.remove(from, to);
}

void vertices::unique(void)
{
    if (empty())
        return;

    stretchy_buffer<path::vertex_type> vertices;
    vertices.append(operator[](0));

    for (unsigned int i = 1; i < impl_->vertices_.size(); ++i)
        if (operator[](i) != vertices.back())
            vertices.append(operator[](i));

    impl_->vertices_.swap(vertices);
}
