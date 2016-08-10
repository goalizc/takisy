#ifndef binary_image_h_20140917
#define binary_image_h_20140917

#include <takisy/core/algorithm.h>
#include <takisy/cgl/tr/filter/blur.h>

class binary_image
{
public:
    binary_image(int radius)
        : radius(radius)
    {}

public:
    template <typename Canvas>
    void filter(const Canvas& src, Canvas& dst) const
    {
        if (&src == &dst)
        {
            Canvas canvas_temporary;
            filter(src, canvas_temporary);
            dst.swap(canvas_temporary);
        }
        else
        {
            typedef typename Canvas::pixel_format pixel_format;

            box_blur(radius < 1 ? 1 : radius).filter(src, dst);

            for (unsigned int y = 0; y < src.height(); ++y)
            {
                const pixel_format* srow = src.row(y);
                      pixel_format* drow = dst.row(y);

                for (unsigned int x = 0; x < src.width();  ++x)
                    drow[x] = color(srow[x].grayscale() < drow[x].grayscale()
                                        ? 0 : color::channel_mask,
                                    srow[x].a());
            }
        }
    }

private:
    int radius;
};

#endif // binary_image_h_20140917
