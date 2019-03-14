#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/path/vertices_wrapper.h>
#include <takisy/cgl/path/builder/bspline.h>

class bspline::implement
{
    friend class bspline;

public:
    implement(void) :
        degree_(3),
        type_(bspline::bstOpen),
        intermediate_point_count_(32)
    {}

public:
    inline void degree(unsigned int degree)
    {
        degree_ = degree < 1 ? 1 : degree;
    }

    inline void type(bspline::Type type)
    {
        type_ = type;
    }

    inline void intermediate_point_count(double count)
    {
        intermediate_point_count_ = count;
    }

public:
    vertices build(const path& path)
    {
        vertices result;

        switch (path.size())
        {
        case  2: result = path;
        case  0:
        case  1: return result;
        default: break;
        }

        stretchy_buffer<path::vertex_type> vertices;
        for (unsigned int i = 0; i < path.size(); ++i)
            vertices.append(path.at(i));

        unsigned int degree = degree_;
        if (degree > vertices.size() - 1)
            degree = vertices.size() - 1;

        if (type_ == bspline::bstClose)
            for (unsigned int i = 0; i < degree; ++i)
                vertices.append(vertices[i]);

        stretchy_buffer<double> knots = generate_knots(vertices.size(), degree);
        unsigned int l = knots.size() - degree - 1;
        double  last_u = knots[l];

        for (unsigned int i = degree; i < l; ++i)
        {
            double step = (knots[i + 1] - knots[i]) / intermediate_point_count_;

            for (double u = knots[i]; u < knots[i + 1]; u += step)
                result.append(deBoor(u, i, degree, degree, knots, vertices));
        }

        result.append(deBoor(last_u, l - 1, degree, degree, knots, vertices));

        return result;
    }

private:
    stretchy_buffer<double> generate_knots(unsigned int control_point_count,
                                           unsigned int degree) const
    {
        unsigned int order = degree + 1;
        stretchy_buffer<double> knots(control_point_count + order);
        unsigned int i = 0, foobar = 0;

        switch (type_)
        {
        default:
        case bspline::bstOpen:
        case bspline::bstClose:
            for ( ; i < knots.size();         ++i) knots[i] = foobar++;
            break;
        case bspline::bstClamp:
            for ( ; i < degree;               ++i) knots[i] = foobar;
            for ( ; i < knots.size() - order; ++i) knots[i] = foobar++;
            for ( ; i < knots.size();         ++i) knots[i] = foobar;
            break;
        }

        return knots;
    }

    path::vertex_type
        deBoor(double u, unsigned int i, unsigned int k, unsigned int p,
               const stretchy_buffer<double>& knots,
               const stretchy_buffer<path::vertex_type>& vertices) const
    {
        if (k == 0)
            return vertices[i];

        double alpha = (u - knots[i]) / (knots[i + p + 1 - k] - knots[i]);

        return deBoor(u, i - 1, k - 1, p, knots, vertices) * (1 - alpha)
             + deBoor(u, i,     k - 1, p, knots, vertices) * alpha;
    }

private:
    unsigned int  degree_;
    bspline::Type type_;
    double        intermediate_point_count_;
};

bspline::bspline(void)
    : impl_(new implement)
{}

bspline::bspline(unsigned int _degree, Type _type)
    : bspline()
{
    degree(_degree);
    type(_type);
}

bspline::bspline(unsigned int _degree, Type _type, double ip_count)
    : bspline()
{
    degree(_degree);
    type(_type);
    intermediate_point_count(ip_count);
}

bspline::bspline(const bspline& _bspline)
    : bspline()
{
    operator=(_bspline);
}

bspline::~bspline(void)
{
    delete impl_;
}

bspline& bspline::operator=(const bspline& bspline)
{
    if (this != & bspline)
    {
        impl_->degree_ = bspline.impl_->degree_;
        impl_->type_   = bspline.impl_->type_;
        impl_->intermediate_point_count_
                       = bspline.impl_->intermediate_point_count_;
    }

    return *this;
}

bspline& bspline::degree(unsigned int degree)
{
    impl_->degree(degree);

    return *this;
}

bspline& bspline::conic(void)
{
    impl_->degree(2);

    return *this;
}

bspline& bspline::cobic(void)
{
    impl_->degree(3);

    return *this;
}

bspline& bspline::type(Type type)
{
    impl_->type(type);

    return *this;
}

bspline& bspline::intermediate_point_count(double count)
{
    impl_->intermediate_point_count(count);

    return *this;
}

unsigned int bspline::degree(void) const
{
    return impl_->degree_;
}

bspline::Type bspline::type(void) const
{
    return impl_->type_;
}

double bspline::intermediate_point_count(void) const
{
    return impl_->intermediate_point_count_;
}

vertices bspline::build(const path& path)
{
    return impl_->build(path);
}

vertices bspline::build(const path::vertex_type* vertices, unsigned int size)
{
    return impl_->build(vertices_wrapper(vertices, size));
}
