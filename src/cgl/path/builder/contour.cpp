#include <takisy/core/math.h>
#include <takisy/cgl/path/vertex_set.h>
#include <takisy/cgl/path/vertex_fetcher.h>
#include <takisy/cgl/path/vertices_wrapper.h>
#include <takisy/cgl/path/builder/circle.h>
#include <takisy/cgl/path/builder/contour.h>

class contour::implement
{
    friend class contour;

    class reversed_path : public path
    {
    public:
        inline reversed_path(const path& path) : path_(path) {}

    public:
        inline unsigned int size(void) const
            { return path_.size(); }
        inline path::vertex_type fetch_vertex(unsigned int index) const
            { return path_.fetch_vertex(path_.size() - 1 - index); }

    private:
        const path& path_;
    };

public:
    implement(double _width, LineCap cap, LineJoint joint, bool closed)
        : cap_(cap), joint_(joint), closed_(closed)
    {
        width(_width);
    }

public:
    inline void width(double width)
    {
        width_ = math::abs(width);
    }

    void build(const path& path)
    {
        if (path.size() < 2)
            return;

        vertex_set np;

        for (unsigned int i = 0; i < path.size(); ++i)
        {
            path::vertex_type vertex = path.fetch_vertex(i);

            if (np.empty() || vertex != np.back())
                np.append(vertex);
        }
        if (closed_ && np.front() == np.back())
            np.remove();

        reversed_path rp(np);

        if (closed_)
        {
            paths_.append(build_half(new vertex_set, np));
            paths_.append(build_half(new vertex_set, rp));
        }
        else
        {
            vertex_set* vs = new vertex_set;

            build_half(vs, np);
            build_half(vs, rp);

            paths_.append(vs);
        }
    }

private:
    vertex_set* build_half(vertex_set* vertex_set, const path& path)
    {
        vertex_fetcher vf(path);

        if (path.size() == 2)
            build_c(vertex_set, vf.fetch_vertex(0), vf.fetch_vertex(1));
        else
        {
            if (!closed_)
                build_c(vertex_set, vf.fetch_vertex(0), vf.fetch_vertex(1));
            else
                build_j(vertex_set, vf.fetch_vertex(-1),
                                    vf.fetch_vertex(-0),
                                    vf.fetch_vertex(+1));

            for (unsigned int ss2 = path.size() - 2, i = 0; i < ss2; ++i)
                build_j(vertex_set, vf.fetch_vertex(i + 0),
                                    vf.fetch_vertex(i + 1),
                                    vf.fetch_vertex(i + 2));

            if (closed_)
                build_j(vertex_set, vf.fetch_vertex(-2),
                                    vf.fetch_vertex(-1),
                                    vf.fetch_vertex(-0));
        }

        return vertex_set;
    }

#define CALCULAE_OFFSET(FROM, TO)                                           \
    double dx_##FROM##TO = _##TO.x - _##FROM.x;                             \
    double dy_##FROM##TO = _##TO.y - _##FROM.y;                             \
    double distance_##FROM##TO = math::sqrt(dx_##FROM##TO * dx_##FROM##TO + \
                                            dy_##FROM##TO * dy_##FROM##TO); \
    double foobar_##FROM##TO = 0.5 * width_ / distance_##FROM##TO;          \
    double offset_x_##FROM##TO = dy_##FROM##TO * foobar_##FROM##TO;         \
    double offset_y_##FROM##TO = dx_##FROM##TO * foobar_##FROM##TO;

    inline void build_c(vertex_set* vertex_set,
                        path::vertex_type _1, const path::vertex_type& _2)
    {
        if (cap_ == LineCap::lcRound)
        {
            double angle = math::atan2(_2 - _1);

            vertex_set->append(
                circle(_1.x, _1.y,
                    0.5 * width_).build_segment(angle - math::deg2rad(90),
                                                angle - math::deg2rad(270)));
        }
        else
        {
            CALCULAE_OFFSET(1, 2);

            if (cap_ == LineCap::lcSquare)
            {
                _1.x -= offset_y_12;
                _1.y -= offset_x_12;
            }

            vertex_set->append(_1.x + offset_x_12, _1.y - offset_y_12);
            vertex_set->append(_1.x - offset_x_12, _1.y + offset_y_12);
        }
    }

    inline void build_j(vertex_set* vertex_set,
                        const path::vertex_type& _1,
                        const path::vertex_type& _2,
                        const path::vertex_type& _3)
    {
        if (joint_ == LineJoint::ljBevel)
            append_bevel_vertex(vertex_set, _1, _2, _3);
        else
        {
            double angle_21 = math::atan2(_1 - _2);
            double angle_23 = math::atan2(_3 - _2);

            if (angle_23 < angle_21)
                angle_23 += math::deg2rad(360);

            double angle = angle_23 - angle_21;

            if (angle < math::deg2rad(180))
            {
                if (joint_ == LineJoint::ljRound)
                {
                    vertex_set->append(
                        circle(_2.x, _2.y, 0.5 * width_).build_segment(
                            angle_21 - math::deg2rad(90),
                            angle_21 - math::deg2rad(270) + angle));
                }
                else
                {
                    vertex_set->append(
                        _2 + path::vertex_type::polar(
                            +0.5 * (angle_21 + angle_23),
                            -0.5 * width_ / math::sin(angle * 0.5)));
                }
            }
            else
                append_bevel_vertex(vertex_set, _1, _2, _3);
        }
    }

    inline void append_bevel_vertex(vertex_set* vertex_set,
                                    const path::vertex_type& _1,
                                    const path::vertex_type& _2,
                                    const path::vertex_type& _3)
    {
        CALCULAE_OFFSET(2, 1);
        CALCULAE_OFFSET(2, 3);

        vertex_set->append(_2.x + offset_x_21, _2.y - offset_y_21);
        vertex_set->append(_2.x - offset_x_23, _2.y + offset_y_23);
    }

#undef CALCULAE_OFFSET

private:
    paths      paths_;
    double     width_;
    LineCap    cap_;
    LineJoint  joint_;
    bool       closed_;
};

contour::contour(void)
    : contour(1, lcButt, ljBevel, false)
{}

contour::contour(double width, LineCap cap, LineJoint joint, bool closed)
    : impl_(new implement(width, cap, joint, closed))
{}

contour::contour(const contour& bl)
    : contour(bl.impl_->width_, bl.impl_->cap_,
              bl.impl_->joint_, bl.impl_->closed_)
{}

contour::~contour(void)
{
    delete impl_;
}

contour& contour::operator=(const contour& bl)
{
    if (this != &bl)
    {
        impl_->width_  = bl.impl_->width_;
        impl_->cap_    = bl.impl_->cap_;
        impl_->joint_  = bl.impl_->joint_;
        impl_->closed_ = bl.impl_->closed_;
    }

    return *this;
}

contour& contour::width(double width)
{
    impl_->width(width);

    return *this;
}

contour& contour::cap(LineCap cap)
{
    impl_->cap_ = cap;

    return *this;
}

contour& contour::joint(LineJoint joint)
{
    impl_->joint_ = joint;

    return *this;
}

contour& contour::closed(bool closed)
{
    impl_->closed_ = closed;

    return *this;
}

double contour::width(void) const
{
    return impl_->width_;
}

contour::LineCap contour::cap(void) const
{
    return impl_->cap_;
}

contour::LineJoint contour::joint(void) const
{
    return impl_->joint_;
}

bool contour::closed(void) const
{
    return impl_->closed_;
}

const paths& contour::build(const path& path)
{
    impl_->paths_.clear();

    impl_->build(path);

    return impl_->paths_;
}

const paths& contour::build(const path::vertex_type* vertices,
                            unsigned int size)
{
    impl_->paths_.clear();

    impl_->build(vertices_wrapper(vertices, size));

    return impl_->paths_;
}

const paths& contour::build(const paths& paths)
{
    impl_->paths_.clear();

    for (unsigned int i = 0; i < paths.size(); ++i)
        impl_->build(*paths.fetch_path(i));

    return impl_->paths_;
}

const paths& contour::build(const path* paths, unsigned int size)
{
    impl_->paths_.clear();

    for (unsigned int i = 0; i < size; ++i)
        impl_->build(paths[i]);

    return impl_->paths_;
}
