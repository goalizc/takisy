#ifndef releivo_h_20140110
#define releivo_h_20140110

#include <takisy/core/algorithm.h>

class releivo
{
public:
    releivo(void)
        : releivo(1)
    {}

    releivo(double ratio)
        : releivo(ratio, 1, 1)
    {}

    releivo(double ratio, int offset_x, int offset_y)
        : releivo(ratio, offset_x, offset_y, 127)
    {}

    releivo(double ratio, int offset_x, int offset_y, unsigned char surface)
        : ratio(ratio), offset_x(offset_x), offset_y(offset_y), surface(surface)
    {}

public:
    template <typename Canvas>
    inline void filter(const Canvas& src, Canvas& dst) const
    {
        if (&src == &dst)
        {
            Canvas canvas_temporary;
            filter(src, canvas_temporary);
            dst.swap(canvas_temporary);
        }
        else
        {
            dst.resize(src.width(), src.height());

            for (unsigned int y = 0; y < dst.height(); ++y)
            for (unsigned int x = 0; x < dst.width();  ++x)
                dst.unsafe_pixel(x, y).grayscale(
                    algorithm::clamp<int>(
                            (src.unsafe_pixel(x, y).grayscale() -
                             src.pixel(x + offset_x, y + offset_y).grayscale())
                                * ratio + surface, 0, color::channel_mask));
        }
    }

private:
    double ratio;
    int offset_x, offset_y;
    unsigned char surface;
};

#endif // releivo_h_20140110
