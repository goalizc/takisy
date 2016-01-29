#include <takisy/core/math.h>
#include <takisy/cgl/path/vertices_wrapper.h>
#include <takisy/cgl/path/vertex_set.h>
#include <takisy/cgl/path/vertex_fetcher.h>
#include <takisy/cgl/path/builder/bezier.h>

class bezier_algorithm
{
    static const unsigned int kDefaultDepth = 20;

public:
    bezier_algorithm()
        : depth_(kDefaultDepth),
          angle_tolerance_(0.052359877),
          distance_tolerance_(0.25)
    {}

    bezier_algorithm(const bezier_algorithm& ba)
        : depth_(ba.depth_),
          angle_tolerance_(ba.angle_tolerance_),
          distance_tolerance_(ba.distance_tolerance_)
    {}

    void operator=(const bezier_algorithm& ba)
    {
        if (this != &ba)
        {
            depth_              = ba.depth_;
            angle_tolerance_    = ba.angle_tolerance_;
            distance_tolerance_ = ba.distance_tolerance_;
        }
    }

public:
    inline void depth(unsigned int depth)
    {
        depth_ = depth > 32 ? 32 : depth;
    }

    inline void angle_tolerance(double angle_tolerance)
    {
        if (angle_tolerance < 0)
            angle_tolerance_ = -angle_tolerance;
        else
            angle_tolerance_ = +angle_tolerance;

        if (angle_tolerance < math::deg2rad(0.1))
            angle_tolerance = math::deg2rad(0.1);
    }

    inline void distance_tolerance(double distance_tolerance)
    {
        if (distance_tolerance < 0)
            distance_tolerance_ = -distance_tolerance;
        else
            distance_tolerance_ = +distance_tolerance;

        if (distance_tolerance < 0.01)
            distance_tolerance = 0.01;
    }

    inline unsigned int depth(void) const
    {
        return depth_;
    }

    inline double angle_tolerance(void) const
    {
        return angle_tolerance_;
    }

    inline double distance_tolerance(void) const
    {
        return distance_tolerance_;
    }

    inline class vertex_set& vertex_set(void)
    {
        return result_;
    }

    inline void build(const path::vertex_type& _1,
                      const path::vertex_type& _2,
                      const path::vertex_type& _3)
    {
        build(_1.x, _1.y, _2.x, _2.y, _3.x, _3.y);
    }

    inline void build(double _1x, double _1y,
                      double _2x, double _2y,
                      double _3x, double _3y)
    {
        result_.append(_1x, _1y);

        build(_1x, _1y, _2x, _2y,_3x, _3y, depth_);
    }

    inline void build(const path::vertex_type& _1,
                      const path::vertex_type& _2,
                      const path::vertex_type& _3,
                      const path::vertex_type& _4)
    {
        build(_1.x, _1.y, _2.x, _2.y, _3.x, _3.y, _4.x, _4.y);
    }

    inline void build(double _1x, double _1y,
                      double _2x, double _2y,
                      double _3x, double _3y,
                      double _4x, double _4y)
    {
        result_.append(_1x, _1y);

        build(_1x, _1y, _2x, _2y,_3x, _3y, _4x, _4y, depth_);
    }

private: // de Casteljau 算法
    void build(double _1x, double _1y,
               double _2x, double _2y,
               double _3x, double _3y, unsigned int depth)
    {
        double
         _12x = ( _1x +  _2x) * 0.5,  _12y = ( _1y +  _2y) * 0.5,
         _23x = ( _2x +  _3x) * 0.5,  _23y = ( _2y +  _3y) * 0.5,
        _123x = (_12x + _23x) * 0.5, _123y = (_12y + _23y) * 0.5;

        if (depth <= 0
            || (   check_angle   (_1x, _1y, _2x, _2y, _3x, _3y)
                && check_distance(_1x, _1y, _2x, _2y, _3x, _3y)))
            result_.append(_123x, _123y);
        else
        {
            --depth;

            build(_1x, _1y, _12x, _12y, _123x, _123y, depth);
            build(_123x, _123y, _23x, _23y, _3x, _3y, depth);
        }
    }

    void build(double _1x, double _1y,
               double _2x, double _2y,
               double _3x, double _3y,
               double _4x, double _4y, unsigned int depth)
    {
        double
          _12x = (  _1x +   _2x) * 0.5,   _12y = (  _1y +   _2y) * 0.5,
          _23x = (  _2x +   _3x) * 0.5,   _23y = (  _2y +   _3y) * 0.5,
          _34x = (  _3x +   _4x) * 0.5,   _34y = (  _3y +   _4y) * 0.5,
         _123x = ( _12x +  _23x) * 0.5,  _123y = ( _12y +  _23y) * 0.5,
         _234x = ( _23x +  _34x) * 0.5,  _234y = ( _23y +  _34y) * 0.5,
        _1234x = (_123x + _234x) * 0.5, _1234y = (_123y + _234y) * 0.5;

        if (depth <= 0
            || (   check_angle   (_1x, _1y, _2x, _2y, _3x, _3y, _4x, _4y)
                && check_distance(_1x, _1y, _2x, _2y, _3x, _3y, _4x, _4y)))
            result_.append(_1234x, _1234y);
        else
        {
            --depth;

            build(_1x, _1y, _12x, _12y, _123x, _123y, _1234x, _1234y, depth);
            build(_1234x, _1234y, _234x, _234y, _34x, _34y, _4x, _4y, depth);
        }
    }

private:
    inline bool check_angle(double _1x, double _1y,
                            double _2x, double _2y,
                            double _3x, double _3y)
    {
        double _1 = math::atan2(_3x - _2x, _3y - _2y);
        double _2 = math::abs(_1 - math::atan2(_2x - _1x, _2y - _1y));

        if (_2 >= math::pi)
            _2 = 2 * math::pi - _2;

        return _2 < angle_tolerance_;
    }

    inline bool check_angle(double _1x, double _1y,
                            double _2x, double _2y,
                            double _3x, double _3y,
                            double _4x, double _4y)
    {
        double _1 = math::atan2(_3x - _2x, _3y - _2y);
        double _2 = math::abs(_1 - math::atan2(_2x - _1x, _2y - _1y));
        double _3 = math::abs(math::atan2(_4x - _3x, _4y - _3y) - _1);

        if (_2 >= math::pi)
            _2 = 2 * math::pi - _2;

        if (_3 >= math::pi)
            _3 = 2 * math::pi - _3;

        return _2 + _3 < angle_tolerance_;
    }

    inline bool check_distance(double _1x, double _1y,
                               double _2x, double _2y,
                               double _3x, double _3y)
    {
        double _1 = _3x - _1x;
        double _2 = _3y - _1y;
        double _3 = math::abs((_2x - _3x) * _2 - (_2y - _3y) * _1);

        return _3 * _3 < distance_tolerance_ * (_1 * _1 + _2 * _2);
    }

    inline bool check_distance(double _1x, double _1y,
                               double _2x, double _2y,
                               double _3x, double _3y,
                               double _4x, double _4y)
    {
        double _1 = _4x - _1x;
        double _2 = _4y - _1y;
        double _3 = math::abs((_2x - _4x) * _2 - (_2y - _4y) * _1);
        double _4 = math::abs((_2x - _4x) * _2 - (_2y - _4y) * _1);

        return _3 * _3 + _4 * _4 < distance_tolerance_ * (_1 * _1 + _2 * _2);
    }

private:
    class vertex_set result_;
    unsigned int depth_;
    double angle_tolerance_, distance_tolerance_;
};

class bezier::implement
{
    friend class bezier;

private:
    bezier_algorithm ba_;
};

bezier::bezier(void)
    : impl_(new implement)
{}

bezier::bezier(const bezier& _bezier)
    : bezier()
{
    impl_->ba_ = _bezier.impl_->ba_;
}

bezier::~bezier(void)
{
    delete impl_;
}

bezier& bezier::operator=(const bezier& bezier)
{
    if (this != &bezier)
        impl_->ba_ = bezier.impl_->ba_;

    return *this;
}

bezier& bezier::depth(unsigned int depth)
{
    impl_->ba_.depth(depth);

    return *this;
}

bezier& bezier::angle_tolerance(double angle_tolerance)
{
    impl_->ba_.angle_tolerance(angle_tolerance);

    return *this;
}

bezier& bezier::distance_tolerance(double distance_tolerance)
{
    impl_->ba_.distance_tolerance(distance_tolerance);

    return *this;
}

unsigned int bezier::depth(void) const
{
    return impl_->ba_.depth();
}

double bezier::angle_tolerance(void) const
{
    return impl_->ba_.angle_tolerance();
}

double bezier::distance_tolerance(void) const
{
    return impl_->ba_.distance_tolerance();
}

const path& bezier::build(const path::vertex_type& _1,
                          const path::vertex_type& _2,
                          const path::vertex_type& _3)
{
    return build(_1.x, _1.y, _2.x, _2.y, _3.x, _3.y);
}

const path& bezier::build(path::vertex_type::axis_type _1x,
                          path::vertex_type::axis_type _1y,
                          path::vertex_type::axis_type _2x,
                          path::vertex_type::axis_type _2y,
                          path::vertex_type::axis_type _3x,
                          path::vertex_type::axis_type _3y)
{
    impl_->ba_.vertex_set().clear();

    impl_->ba_.build(_1x, _1y, _2x, _2y, _3x, _3y);
    impl_->ba_.vertex_set().append(_3x, _3y);

    return impl_->ba_.vertex_set();
}

const path& bezier::build(const path::vertex_type& _1,
                          const path::vertex_type& _2,
                          const path::vertex_type& _3,
                          const path::vertex_type& _4)
{
    return build(_1.x, _1.y, _2.x, _2.y, _3.x, _3.y, _4.x, _4.y);
}

const path& bezier::build(path::vertex_type::axis_type _1x,
                          path::vertex_type::axis_type _1y,
                          path::vertex_type::axis_type _2x,
                          path::vertex_type::axis_type _2y,
                          path::vertex_type::axis_type _3x,
                          path::vertex_type::axis_type _3y,
                          path::vertex_type::axis_type _4x,
                          path::vertex_type::axis_type _4y)
{
    impl_->ba_.vertex_set().clear();

    impl_->ba_.build(_1x, _1y, _2x, _2y, _3x, _3y, _4x, _4y);
    impl_->ba_.vertex_set().append(_4x, _4y);

    return impl_->ba_.vertex_set();
}

class bezier_fitting_curve::implement
{
    friend class bezier_fitting_curve;

    struct control_vertex
    {
        path::vertex_type _1;
        path::vertex_type _2;
    };

public:
    implement(double k, bool closed)
        : k_(k), closed_(closed)
    {}

public:
    const path& build(const path& path)
    {
        ba_.vertex_set().clear();

        if (path.size() < 3)
            return path;
        else
        {
            int from_index = 0, to_index = 0;
            vertex_fetcher vf(path);

            if (closed_)
                from_index = 0, to_index = path.size() - 1;
            else
                from_index = 1, to_index = path.size() - 3;

            if (!closed_)
                build_3p_v1(vf.fetch_vertex(+0),
                            vf.fetch_vertex(+1),
                            vf.fetch_vertex(+2));

            for (int i = from_index; i <= to_index; ++i)
                build_4p(vf.fetch_vertex(i - 1), vf.fetch_vertex(i + 0),
                         vf.fetch_vertex(i + 1), vf.fetch_vertex(i + 2));

            if (!closed_)
            {
                build_3p_v2(vf.fetch_vertex(-3),
                            vf.fetch_vertex(-2),
                            vf.fetch_vertex(-1));
                ba_.vertex_set().append(vf.fetch_vertex(-1));
            }

            return ba_.vertex_set();
        }
    }

private:
    inline void build_3p_v1(const path::vertex_type& _1,
                            const path::vertex_type& _2,
                            const path::vertex_type& _3)
    {
        ba_.build(_1,
                  calculate_control_vertex(_1, _2, _3)._1,
                  _2);
    }

    inline void build_4p(const path::vertex_type& _1,
                         const path::vertex_type& _2,
                         const path::vertex_type& _3,
                         const path::vertex_type& _4)
    {
        ba_.build(_2,
                  calculate_control_vertex(_1, _2, _3)._2,
                  calculate_control_vertex(_2, _3, _4)._1,
                  _3);
    }

    inline void build_3p_v2(const path::vertex_type& _1,
                            const path::vertex_type& _2,
                            const path::vertex_type& _3)
    {
        ba_.build(_2,
                  calculate_control_vertex(_1, _2, _3)._2,
                  _3);
    }

    inline control_vertex calculate_control_vertex(const path::vertex_type& _1,
                                                   const path::vertex_type& _2,
                                                   const path::vertex_type& _3)
    {
        path::vertex_type foobar = (_3 - _1) * k_ * 0.25;

        return control_vertex{._1 = _2 - foobar, ._2 = _2 + foobar};
    }

private:
    double k_;
    bool closed_;
    bezier_algorithm ba_;
};

bezier_fitting_curve::bezier_fitting_curve(void)
    : bezier_fitting_curve(1, false)
{}

bezier_fitting_curve::bezier_fitting_curve(double k, bool closed)
    : impl_(new implement(k, closed))
{}

bezier_fitting_curve::bezier_fitting_curve(const bezier_fitting_curve& bfc)
    : bezier_fitting_curve(bfc.impl_->k_, bfc.impl_->closed_)
{
    impl_->ba_ = bfc.impl_->ba_;
}

bezier_fitting_curve::~bezier_fitting_curve(void)
{
    delete impl_;
}

bezier_fitting_curve& bezier_fitting_curve::operator=(
    const bezier_fitting_curve& bfc)
{
    if (this != & bfc)
    {
        impl_->k_      = bfc.impl_->k_;
        impl_->closed_ = bfc.impl_->closed_;
        impl_->ba_     = bfc.impl_->ba_;
    }

    return *this;
}

bezier_fitting_curve& bezier_fitting_curve::depth(unsigned int depth)
{
    impl_->ba_.depth(depth);

    return *this;
}

bezier_fitting_curve& bezier_fitting_curve::angle_tolerance(
    double angle_tolerance)
{
    impl_->ba_.angle_tolerance(angle_tolerance);

    return *this;
}

bezier_fitting_curve& bezier_fitting_curve::distance_tolerance(
    double distance_tolerance)
{
    impl_->ba_.distance_tolerance(distance_tolerance);

    return *this;
}

bezier_fitting_curve& bezier_fitting_curve::k(double k)
{
    impl_->k_ = k;

    return *this;
}

bezier_fitting_curve& bezier_fitting_curve::closed(bool closed)
{
    impl_->closed_ = closed;

    return *this;
}

unsigned int bezier_fitting_curve::depth(void) const
{
    return impl_->ba_.depth();
}

double bezier_fitting_curve::angle_tolerance(void) const
{
    return impl_->ba_.angle_tolerance();
}

double bezier_fitting_curve::distance_tolerance(void) const
{
    return impl_->ba_.distance_tolerance();
}

double bezier_fitting_curve::k(void) const
{
    return impl_->k_;
}

bool bezier_fitting_curve::closed(void) const
{
    return impl_->closed_;
}

const path& bezier_fitting_curve::build(const path& path)
{
    return impl_->build(path);
}

const path& bezier_fitting_curve::build(const path::vertex_type* vertices,
                                        unsigned int size)
{
    return build(vertices_wrapper(vertices, size));
}

