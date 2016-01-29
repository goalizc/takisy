#include <takisy/core/math.h>
#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/path/vertices_wrapper.h>
#include <takisy/cgl/path/vertex_set.h>
#include <takisy/cgl/path/builder/dashed_line.h>

class dashed_line::implement
{
    friend class dashed_line;

    struct vd
    {
        path::vertex_type vertex;
        double distance_to_next_vertex;
    };

public:
    implement(const double* dash_array, unsigned int size,
              bool closed, double offset)
        : dash_array_(dash_array, size & ~1), closed_(closed), offset_(offset)
    {
        for (double& dash : dash_array_)
            if (dash < 0) dash = -dash;
    }

public:
    void build(const path& path)
    {
        if (path.size() < 2)
            return;

        if (dash_array_.size() < 2)
            paths_.append(new vertex_set(path));
        else
        {
            stretchy_buffer<vd> vds(path.size() + (closed_ ? 1 : 0));

            for (unsigned int i = 0; i < vds.size(); ++i)
            {
                vds[i].vertex = path.fetch_vertex(i % path.size());
                vds[i].distance_to_next_vertex = vds[i].vertex.distance(
                        path.fetch_vertex((i + 1) % path.size()));
            }

            unsigned int di     = 0;
            double       dash   = offset(di);
            unsigned int vi     = 0;
            double       offset = 0;

            while (vi < vds.size())
            {
                if (di % 2 == 0)
                    paths_.append(calculate_dash<true>(dash, vds, vi, offset));
                else
                    calculate_dash<false>(dash, vds, vi, offset);

                dash = dash_array_[++di %= dash_array_.size()];
            }
        }
    }

private:
    double offset(unsigned int& di)
    {
        double dash, offset = offset_;

        if (offset < 0)
            do
            {
                di   = (di - 1 + dash_array_.size()) % dash_array_.size();
                dash = dash_array_[di];

                if (offset < -dash)
                    offset += dash;
                else
                {
                    dash = -offset;
                    break;
                }
            } while (true);
        else
        {
            dash = dash_array_[di];

            while (true)
                if (offset >= dash)
                {
                    offset -= dash;
                    dash = dash_array_[++di %= dash_array_.size()];
                }
                else
                {
                    dash -= offset;
                    break;
                }
        }

        return dash;
    }

    template <bool Malloc>
    vertex_set* calculate_dash(double dash, const stretchy_buffer<vd>& vds,
                               unsigned int& vi, double& offset)
    {
        vertex_set* segment = Malloc ? new vertex_set : nullptr;

        if (Malloc && offset > 0)
            segment->append(calc_vtx(offset, vds[vi - 1], vds[vi]));

        while (dash > offset && vi < vds.size())
        {
            dash  -= offset;
            offset = vds[vi].distance_to_next_vertex;
            if (Malloc) segment->append(vds[vi].vertex);
            ++vi;
        }

        if (dash <= offset && vi < vds.size())
        {
            offset -= dash;
            if (Malloc) segment->append(calc_vtx(offset, vds[vi - 1], vds[vi]));
        }

        return segment;
    }

    // calculate vertex
    inline path::vertex_type calc_vtx(double offset, const vd& _1, const vd& _2)
    {
        double foobar = offset / _1.distance_to_next_vertex;

        return path::vertex_type {
            .x = _2.vertex.x - (_2.vertex.x - _1.vertex.x) * foobar,
            .y = _2.vertex.y - (_2.vertex.y - _1.vertex.y) * foobar,
        };
    }

private:
    paths                   paths_;
    stretchy_buffer<double> dash_array_;
    bool                    closed_;
    double                  offset_;
};

dashed_line::dashed_line(const double* dash_array, unsigned int size)
    : dashed_line(dash_array, size, false)
{}

dashed_line::dashed_line(const double* dash_array, unsigned int size,
                         bool closed)
    : dashed_line(dash_array, size, closed, 0)
{}

dashed_line::dashed_line(const double* dash_array, unsigned int size,
                         double offset)
    : dashed_line(dash_array, size, false, offset)
{}

dashed_line::dashed_line(const double* dash_array, unsigned int size,
                         bool closed, double offset)
    : impl_(new implement(dash_array, size, closed, offset))
{}

dashed_line::dashed_line(std::initializer_list<double> il)
    : dashed_line(il.begin(), il.size())
{}

dashed_line::dashed_line(std::initializer_list<double> il, bool closed)
    : dashed_line(il.begin(), il.size(), closed)
{}

dashed_line::dashed_line(std::initializer_list<double> il, double offset)
    : dashed_line(il.begin(), il.size(), offset)
{}

dashed_line::dashed_line(std::initializer_list<double> il,
                         bool closed, double offset)
    : dashed_line(il.begin(), il.size(), closed, offset)
{}

dashed_line::dashed_line(const dashed_line& dl)
    : dashed_line(dl.impl_->dash_array_.data(), dl.impl_->dash_array_.size(),
                  dl.impl_->closed_, dl.impl_->offset_)
{}

dashed_line::~dashed_line(void)
{
    delete impl_;
}

dashed_line& dashed_line::operator=(const dashed_line& dl)
{
    if (this != &dl)
    {
        impl_->dash_array_ = dl.impl_->dash_array_.clone();
        impl_->closed_     = dl.impl_->closed_;
        impl_->offset_     = dl.impl_->offset_;
    }

    return *this;
}

dashed_line& dashed_line::dash_array(std::initializer_list<double> il)
{
    return dash_array(il.begin(), il.size());
}

dashed_line& dashed_line::dash_array(const double* dash_array,
                                     unsigned int size)
{
    impl_->dash_array_.clear();
    impl_->dash_array_.append(dash_array, size);

    return *this;
}

dashed_line& dashed_line::dash(unsigned int index, double dash)
{
    if (index < impl_->dash_array_.size())
        impl_->dash_array_[index] = dash;

    return *this;
}

dashed_line& dashed_line::closed(bool closed)
{
    impl_->closed_ = closed;

    return *this;
}

dashed_line& dashed_line::offset(double offset)
{
    impl_->offset_ = offset;

    return *this;
}

unsigned int dashed_line::dash_array_size(void) const
{
    return impl_->dash_array_.size();
}

double dashed_line::dash(unsigned int index) const
{
    if (index < impl_->dash_array_.size())
        return impl_->dash_array_[index];
    else
        return 0;
}

bool dashed_line::closed(void) const
{
    return impl_->closed_;
}

double dashed_line::offset(void) const
{
    return impl_->offset_;
}

const paths& dashed_line::build(const path& path)
{
    impl_->paths_.clear();

    impl_->build(path);

    return impl_->paths_;
}

const paths& dashed_line::build(const path::vertex_type* vertices,
                                unsigned int size)
{
    return build(vertices_wrapper(vertices, size));
}

const paths& dashed_line::build(const paths& paths)
{
    impl_->paths_.clear();

    for (unsigned int i = 0; i < paths.size(); ++i)
        impl_->build(*paths.fetch_path(i));

    return impl_->paths_;
}

const paths& dashed_line::build(const path* paths, unsigned int size)
{
    impl_->paths_.clear();

    for (unsigned int i = 0; i < size; ++i)
        impl_->build(paths[i]);

    return impl_->paths_;
}
