#include <takisy/cgl/basic/color.h>

color& color::blend(const color& fg_color)
{
    const double channel_mask = static_cast<double>(color::channel_mask);

    if (fg_color.a == color::channel_mask)
        return operator=(fg_color);
    else
    if (fg_color.a == 0)
        return *this;

    double _1 = a / channel_mask;
    double _2 = fg_color.a / channel_mask;
    double _3 = _1 - _1 * _2;
    double _4 = _2 + _3;

    r = (fg_color.r * _2 + r * _3) / _4;
    g = (fg_color.g * _2 + g * _3) / _4;
    b = (fg_color.b * _2 + b * _3) / _4;
    a = _4 * channel_mask + 0.5;

    return *this;
}

color operator*(color::channel_type factor, const color& color)
{
    return color * factor;
}
