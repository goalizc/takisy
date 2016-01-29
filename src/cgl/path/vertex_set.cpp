#include <takisy/core/algorithm.h>
#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/path/vertex_set.h>

class vertex_set::implement
{
    friend class vertex_set;

private:
    stretchy_buffer<path::vertex_type> vertices_;
};

vertex_set::vertex_set(void)
    : impl_(new implement)
{}

vertex_set::vertex_set(const path& path)
    : vertex_set()
{
    operator=(path);
}

vertex_set::vertex_set(const path::vertex_type* vertices, unsigned int size)
    : vertex_set()
{
    append(vertices, size);
}

vertex_set::vertex_set(const vertex_set& _vertex_set)
    : vertex_set()
{
    operator=(_vertex_set);
}

vertex_set::~vertex_set(void)
{
    delete impl_;
}

vertex_set& vertex_set::operator=(const path& path)
{
    if (this != &path)
    {
        clear();
        append(path);
    }

    return *this;
}

vertex_set& vertex_set::operator=(const vertex_set& vertex_set)
{
    if (this != &vertex_set)
    {
        clear();
        append(vertex_set);
    }

    return *this;
}

unsigned int vertex_set::size(void) const
{
    return impl_->vertices_.size();
}

path::vertex_type vertex_set::fetch_vertex(unsigned int index) const
{
    return impl_->vertices_[index];
}

path::vertex_type& vertex_set::at(unsigned int index)
{
    return impl_->vertices_[index];
}

const path::vertex_type& vertex_set::at(unsigned int index) const
{
    return impl_->vertices_[index];
}

path::vertex_type& vertex_set::operator[](unsigned int index)
{
    return impl_->vertices_[index];
}

const path::vertex_type& vertex_set::operator[](unsigned int index) const
{
    return impl_->vertices_[index];
}

path::vertex_type& vertex_set::front(void)
{
    return impl_->vertices_.front();
}

const path::vertex_type& vertex_set::front(void) const
{
    return impl_->vertices_.front();
}

path::vertex_type& vertex_set::back(void)
{
    return impl_->vertices_.back();
}

const path::vertex_type& vertex_set::back(void) const
{
    return impl_->vertices_.back();
}


bool vertex_set::empty(void) const
{
    return impl_->vertices_.empty();
}

void vertex_set::clear(void)
{
    impl_->vertices_.clear();
}

void vertex_set::reverse(void)
{
    unsigned int half = size() / 2;

    for (unsigned int i = 0; i < half; ++i)
        algorithm::swap(impl_->vertices_[i], impl_->vertices_[size() - i - 1]);
}

void vertex_set::insert(unsigned int index, const path::vertex_type& vertex)
{
    impl_->vertices_.insert(index, vertex);
}

void vertex_set::insert(unsigned int index,
                        path::vertex_type::axis_type x,
                        path::vertex_type::axis_type y)
{
    impl_->vertices_.insert(index, path::vertex_type(x, y));
}

void vertex_set::insert(unsigned int index, const path& path)
{
    stretchy_buffer<path::vertex_type> sb_path(path.size());

    for (unsigned int i = 0; i < path.size(); ++i)
        sb_path[i] = path.fetch_vertex(i);

    impl_->vertices_.insert(index, sb_path.data(), sb_path.size());
}

void vertex_set::insert(unsigned int index, const vertex_set& vertex_set)
{
    impl_->vertices_.insert(index,
                            vertex_set.impl_->vertices_.data(),
                            vertex_set.impl_->vertices_.size());
}

void vertex_set::insert(unsigned int index,
                        const path::vertex_type* vertices, unsigned int size)
{
    impl_->vertices_.insert(index, vertices, size);
}

void vertex_set::append(const path::vertex_type& vertex)
{
    impl_->vertices_.append(vertex);
}

void vertex_set::append(path::vertex_type::axis_type x,
                        path::vertex_type::axis_type y)
{
    impl_->vertices_.append(path::vertex_type(x, y));
}

void vertex_set::append(const path& path)
{
    stretchy_buffer<path::vertex_type> sb_path(path.size());

    for (unsigned int i = 0; i < path.size(); ++i)
        sb_path[i] = path.fetch_vertex(i);

    impl_->vertices_.append(sb_path.data(), sb_path.size());
}

void vertex_set::append(const vertex_set& vertex_set)
{
    impl_->vertices_.append(vertex_set.impl_->vertices_.data(),
                            vertex_set.impl_->vertices_.size());
}

void vertex_set::append(const path::vertex_type* vertices, unsigned int size)
{
    impl_->vertices_.append(vertices, size);
}

void vertex_set::remove(void)
{
    impl_->vertices_.remove();
}

void vertex_set::remove(unsigned int index)
{
    impl_->vertices_.remove(index);
}

void vertex_set::remove(unsigned int from, unsigned int to)
{
    impl_->vertices_.remove(from, to);
}
