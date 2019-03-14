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
    implement(void) : lower_(-1e100), upper_(+1e100) {}

public:
    void add_knot(double x, double fy)
    {
        newton_polynomial_.push_back(newton_polynomial {
            .dq   = fy,
            .knot = interpolator::knot_type { x, fy },
        });

        for (int i = newton_polynomial_.size() - 1; i > 0; --i)
            newton_polynomial_[i - 1].dq =
                (newton_polynomial_[i].dq - newton_polynomial_[i - 1].dq) /
                (newton_polynomial_.back().knot.x -
                 newton_polynomial_[i - 1].knot.x);
    }

    double fy(double x) const
    {
        if (newton_polynomial_.empty())
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

            if (math::isnan(fy))
                return 0;
            else
                return fy < lower_ ? lower_ : fy > upper_ ? upper_ : fy;
        }
    }

private:
    double lower_, upper_;
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
        *impl_ = *interpolator.impl_;

    return *this;
}

interpolator& interpolator::clear(void)
{
    impl_->newton_polynomial_.clear();

    return *this;
}

double interpolator::lower(void) const
{
    return impl_->lower_;
}

double interpolator::upper(void) const
{
    return impl_->upper_;
}

unsigned int interpolator::knots_size(void) const
{
    return impl_->newton_polynomial_.size();
}

interpolator::knot_type interpolator::knot(unsigned int index) const
{
    return impl_->newton_polynomial_[index].knot;
}

interpolator::knots_type interpolator::knots(void) const
{
    knots_type knots;
    for (implement::newton_polynomial& element : impl_->newton_polynomial_)
        knots.push_back(element.knot);

    return knots;
}

interpolator& interpolator::clamp(double lower, double upper)
{
    impl_->lower_ = lower;
    impl_->upper_ = upper;

    return *this;
}

interpolator& interpolator::lower(double lower)
{
    impl_->lower_ = lower;

    return *this;
}

interpolator& interpolator::upper(double upper)
{
    impl_->upper_ = upper;

    return *this;
}

interpolator& interpolator::add_knot(const knot_type& knot)
{
    return add_knot(knot.x, knot.y);
}

interpolator& interpolator::add_knot(double x, double fy)
{
    impl_->add_knot(x, fy);

    return *this;
}

double interpolator::fy(double x) const
{
    return impl_->fy(x);
}

double interpolator::operator[](double x) const
{
    return impl_->fy(x);
}
