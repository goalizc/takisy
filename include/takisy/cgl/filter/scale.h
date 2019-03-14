#ifndef scale_h_20131203
#define scale_h_20131203

#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/component/bilinear_interpolator.h>

class scale
{
protected:
    typedef stretchy_buffer<int> table_type;

    template <unsigned int Shift>
    static table_type table(unsigned int src, unsigned int dst)
    {
        double ratio = (src - 1.0) * (1 << Shift) / (dst - 1), foobar = 0.5;
        table_type table(dst);

        for (unsigned int i = 0; i < dst; ++i, foobar += ratio)
            table[i] = foobar;

        return table;
    }
};

class nearest_neighbor_scale : private scale
{
public:
    nearest_neighbor_scale(unsigned int width, unsigned int height)
        : width(width), height(height)
    {}

public:
    template <typename Canvas>
    inline void filter(const Canvas& src, Canvas& dst) const
    {
        bool same_size = width == src.width() && height == src.height();

        if (&src == &dst) {
            if (!same_size) {
                Canvas canvas_temporary;
                filter(src, canvas_temporary);
                dst.swap(canvas_temporary);
            }
        } else if (same_size)
            dst = src;
        else {
            typedef typename Canvas::pixel_format pixel_format;
            table_type xtable = table<0>(src.width(),  width);
            table_type ytable = table<0>(src.height(), height);

            dst.resize(width, height);

            for (unsigned int y = 0; y < height; ++y) {
                const pixel_format* src_row = src.row(ytable[y]);
                      pixel_format* dst_row = dst.row(y);

                for (unsigned int x = 0; x < width; ++x)
                    dst_row[x] = src_row[xtable[x]];
            }
        }
    }

private:
    unsigned int width, height;
};

class bilinear_interpolation_scale : private scale
{
public:
    bilinear_interpolation_scale(unsigned int width, unsigned int height)
        : width(width), height(height)
    {}

public:
    template <typename Canvas>
    inline void filter(const Canvas& src, Canvas& dst) const
    {
        bool same_size = width == src.width() && height == src.height();

        if (&src == &dst) {
            if (!same_size) {
                Canvas canvas_temporary;
                filter(src, canvas_temporary);
                dst.swap(canvas_temporary);
            }
        } else if (same_size) {
            dst = src;
        } else {
            const unsigned int shift = bilinear_interpolator<Canvas>::shift();
            table_type xtable = table<shift>(src.width(), width);
            table_type ytable = table<shift>(src.height(), height);
            bilinear_interpolator<Canvas> bi(src);

            dst.resize(width, height);

            for (unsigned int y = 0; y < height; ++y) {
                typename Canvas::pixel_format* dst_row = dst.row(y);

                for (unsigned int x = 0; x < width; ++x)
                    dst_row[x] = bi.pixel(xtable[x], ytable[y]);
            }
        }
    }

private:
    unsigned int width, height;
};

// 三次卷积缩放
// class ...
// {
//
// };

#endif //scale_h_20131203
