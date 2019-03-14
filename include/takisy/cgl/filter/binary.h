#ifndef binary_h_20140917
#define binary_h_20140917

#include <takisy/cgl/filter/blur.h>

class binary
{
public:
    binary(int radius)
        : radius(radius)
    {}

public:
    template <typename Canvas>
    void filter(const Canvas& src, Canvas& dst) const
    {
        if (&src == &dst) {
            Canvas canvas_temporary;
            filter(src, canvas_temporary);
            dst.swap(canvas_temporary);
        } else {
            typedef typename Canvas::pixel_format pixel_format;

            box_blur(radius < 1 ? 1 : radius).filter(src, dst);

            for (unsigned int y = 0; y < src.height(); ++y) {
                const pixel_format* srow = src.row(y);
                      pixel_format* drow = dst.row(y);

                for (unsigned int x = 0; x < src.width();  ++x)
                    drow[x].grayscale(srow[x].grayscale() < drow[x].grayscale()
                                      ? 0 : pixel_format::channel_mask());
            }
        }
    }

private:
    int radius;
};

#endif // binary_h_20140917
