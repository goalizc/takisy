#ifndef gamma_h_20131119
#define gamma_h_20131119

#include <takisy/core/math.h>

class gamma_null
{
public:
    template <typename GammeFunction>
    inline void refresh_gamma_lut(const GammeFunction&) {}
    inline unsigned int gamma_value(unsigned int x) { return x; }
};

template <unsigned int GammaLutSize>
class gamma_lut
{
public:
    gamma_lut(void)
    {
        refresh_gamma_lut(1);
    }

    template <typename GammeFunction>
    gamma_lut(const GammeFunction& gamma_function)
    {
        refresh_gamma_lut(gamma_function);
    }

public:
    inline void refresh_gamma_lut(int gamma_seed)
    {
        refresh_gamma_lut(static_cast<double>(gamma_seed));
    }

    inline void refresh_gamma_lut(double gamma_seed)
    {
        refresh_gamma_lut(
            [&](double x) -> double {
                return math::pow(x, gamma_seed);
            });
    }

    template <typename GammeFunction>
    void refresh_gamma_lut(const GammeFunction& gamma_function)
    {
        constexpr double foobar = static_cast<double>(GammaLutSize - 1);

        for (unsigned int i = 0; i < GammaLutSize; ++i)
            gamma_lut_[i] = gamma_function(i / foobar) * foobar;
    }

    inline unsigned int gamma_value(unsigned int x)
    {
        return gamma_lut_[x];
    }

private:
    unsigned int gamma_lut_[GammaLutSize];
};

typedef gamma_lut<256> gamma_256, gamma_uint8, gamma_u8, gamma_byte;

#endif //gamma_h_20131119
