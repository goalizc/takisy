#include <vector>
#include <takisy/core/math.h>
#include <takisy/cgl/component/interpolator.h>

class interpolator::implement
{
    friend class interpolator;

    struct newton_polynomial
    {
        double dq; // difference quotient; 差商
        interpolator::knot_type knot;
    };

public:
    implement(void) : lower_fy_(-1e100), upper_fy_(+1e100) {}

public:
    void add_knot(double x, double y)
    {
        newton_polynomial_.push_back(newton_polynomial {
            .dq   = y,
            .knot = interpolator::knot_type {.x = x, .y = y},
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

            return fy < lower_fy_ ? lower_fy_ : fy > upper_fy_ ? upper_fy_ : fy;
        }
    }

private:
    double lower_fy_;
    double upper_fy_;
    std::vector<newton_polynomial> newton_polynomial_;
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
        impl_->lower_fy_          = interpolator.impl_->lower_fy_;
        impl_->upper_fy_          = interpolator.impl_->upper_fy_;
        impl_->newton_polynomial_ = interpolator.impl_->newton_polynomial_;
    }

    return *this;
}

interpolator& interpolator::clear(void)
{
    impl_->newton_polynomial_.clear();

    return *this;
}

double interpolator::lower_fy(void) const
{
    return impl_->lower_fy_;
}

double interpolator::upper_fy(void) const
{
    return impl_->upper_fy_;
}

unsigned int interpolator::knots_size(void) const
{
    return impl_->newton_polynomial_.size();
}

interpolator::knot_type interpolator::knot(unsigned int index) const
{
    return impl_->newton_polynomial_[index].knot;
}

interpolator& interpolator::clamp_fy(double lower, double upper)
{
    impl_->lower_fy_ = lower;
    impl_->upper_fy_ = upper;

    return *this;
}

interpolator& interpolator::lower_fy(double lower_fy)
{
    impl_->lower_fy_ = lower_fy;

    return *this;
}

interpolator& interpolator::upper_fy(double upper_fy)
{
    impl_->upper_fy_ = upper_fy;

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

double interpolator::fy(double x) const
{
    return impl_->fy(x);
}
