#include <takisy/cgl/basic/color.h>

color& color::blend(const color& c)
{
    constexpr double channel_mask = static_cast<double>(color::channel_mask);

    if (c.a == color::channel_mask)
        return operator=(c);
    else
    if (c.a == 0)
        return *this;

    double _1 = a / channel_mask;
    double _2 = c.a / channel_mask;
    double _3 = _1 - _1 * _2;
    double _4 = _2 + _3;

    r = (c.r * _2 + r * _3) / _4;
    g = (c.g * _2 + g * _3) / _4;
    b = (c.b * _2 + b * _3) / _4;
    a = _4 * channel_mask + 0.5;

    return *this;
}

color operator*(color::channel_type factor, const color& c)
{
    return c * factor;
}
