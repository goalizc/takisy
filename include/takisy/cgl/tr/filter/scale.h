#ifndef scale_h_20131203
#define scale_h_20131203

#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/component/bilinear_interpolator.h>

class scale
{
public:
    typedef stretchy_buffer<int> table_t;

public:
    template <unsigned int Shift>
    static table_t table(unsigned int src, unsigned int dst)
    {
        double ratio = (src - 1.0) * (1 << Shift) / (dst - 1), foobar = 0;
        table_t table(dst);

        for (unsigned int i = 0; i < dst; ++i, foobar += ratio)
            table[i] = foobar + 0.5;

        return table;
    }
};

class nearest_neighbor_scale
{
public:
    nearest_neighbor_scale(unsigned int width, unsigned int height)
        : width(width), height(height)
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
        if (width == src.width() && height == src.height())
            dst = src;
        else
        {
            typedef typename Canvas::pixel_format pixel_format;
            scale::table_t xtable = scale::table<0>(src.width(),  width);
            scale::table_t ytable = scale::table<0>(src.height(), height);

            dst.resize(width, height);

            for (unsigned int y = 0; y < height; ++y)
            {
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

class bilinear_interpolation_scale
{
public:
    bilinear_interpolation_scale(unsigned int width, unsigned int height)
        : width(width), height(height)
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
        if (width == src.width() && height == src.height())
            dst = src;
        else
        {
            typedef bilinear_interpolator<Canvas> bilinear_interpolator;
            typedef typename Canvas::pixel_format pixel_format;
            typedef unsigned int uint32;
            static constexpr uint32 shift = bilinear_interpolator::shift();
            scale::table_t xtable = scale::table<shift>(src.width(),  width);
            scale::table_t ytable = scale::table<shift>(src.height(), height);
            bilinear_interpolator bi(src);

            dst.resize(width, height);

            for (uint32 y = 0; y < height; ++y)
            {
                pixel_format* dst_row = dst.row(y);

                for (uint32 x = 0; x < width; ++x)
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
