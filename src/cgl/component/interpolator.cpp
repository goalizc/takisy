#include <takisy/core/math.h>
#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/component/interpolator.h>

class interpolator::implement
{
    friend class interpolator;

    struct newton_polynomial
    {
        interpolator::knot_type knot;
        double dq; // difference quotient
    };

public:
    implement(void) : min_fy_(-1e100), max_fy_(+1e100) {}

public:
    void add_knot(double x, double y)
    {
        newton_polynomial_.append(newton_polynomial {
            .knot = interpolator::knot_type {.x = x, .y = y},
            .dq   = y
        });

        for (int i = newton_polynomial_.size() - 1; i > 0; --i)
            newton_polynomial_[i - 1].dq =
                (newton_polynomial_[i].dq - newton_polynomial_[i - 1].dq) /
                (newton_polynomial_.back().knot.x -
                 newton_polynomial_[i - 1].knot.x);
    }

    double fy(double x) const
    {
        if (newton_polynomial_.size() == 0)
            return 0;
        else
        {
            const unsigned int last_index = newton_polynomial_.size() - 1;
            double fy     = newton_polynomial_[last_index].dq;
            double foobar = 1;

            for (unsigned int i = last_index; i > 0; --i)
            {
                foobar *= x - newton_polynomial_[i].knot.x;
                fy     += foobar * newton_polynomial_[i - 1].dq;
            }

            static_cast<void>(math::isnan(fy) ? fy = 0.0 : 0);

            return fy < min_fy_ ? min_fy_ : fy > max_fy_ ? max_fy_ : fy;
        }
    }

private:
    double min_fy_;
    double max_fy_;
    stretchy_buffer<newton_polynomial> newton_polynomial_;
};

interpolator::interpolator(void)
    : impl_(new implement)
{}

interpolator::interpolator(const interpolator& _interpolator)
    : interpolator()
{
    operator=(_interpolator);
}

interpolator::~interpolator(void)
{
    delete impl_;
}

interpolator& interpolator::operator=(const interpolator& interpolator)
{
    if (this != &interpolator)
    {
        impl_->min_fy_ = interpolator.impl_->min_fy_;
        impl_->max_fy_ = interpolator.impl_->max_fy_;
        impl_->newton_polynomial_
                       = interpolator.impl_->newton_polynomial_.clone();
    }

    return *this;
}

interpolator& interpolator::clear(void)
{
    impl_->newton_polynomial_.clear();

    return *this;
}

interpolator& interpolator::clamp_fy(double min, double max)
{
    impl_->min_fy_ = min;
    impl_->max_fy_ = max;

    return *this;
}

interpolator& interpolator::min_fy(double min_fy)
{
    impl_->min_fy_ = min_fy;

    return *this;
}

interpolator& interpolator::max_fy(double max_fy)
{
    impl_->max_fy_ = max_fy;

    return *this;
}

interpolator& interpolator::add_knot(const knot_type& knot)
{
    return add_knot(knot.x, knot.y);
}

interpolator& interpolator::add_knot(double x, double y)
{
    impl_->add_knot(x, y);

    return *this;
}

double interpolator::min_fy(void) const
{
    return impl_->min_fy_;
}

double interpolator::max_fy(void) const
{
    return impl_->max_fy_;
}

unsigned int interpolator::knots_size(void) const
{
    return impl_->newton_polynomial_.size();
}

interpolator::knot_type interpolator::fetch_knot(unsigned int index) const
{
    return impl_->newton_polynomial_[index].knot;
}

double interpolator::fy(double x) const
{
    return impl_->fy(x);
}
