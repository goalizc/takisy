#ifndef blur_h_20131127
#define blur_h_20131127

#include <takisy/core/math.h>
#include <takisy/core/memory.h>
#include <takisy/core/algorithm.h>
#include <takisy/core/stretchy_buffer.h>

class box_blur
{
public:
    box_blur(int radius)
        : box_blur(radius, 1)
    {}

    box_blur(int radius, unsigned int iterations)
        : radius(radius), iterations(iterations)
    {}

public:
    template <typename Canvas>
    inline void filter(const Canvas& src, Canvas& dst) const
    {
        if (radius > 0)
        {
            int radius2 = math::ceil(static_cast<double>(radius) / iterations);
            Canvas canvas_temporary(src.height(), src.width());

            for (unsigned int i = 0; i < iterations; ++i)
            {
                blur(src, canvas_temporary, radius2);
                dst.resize(src.width(), src.height());
                blur(canvas_temporary, dst, radius2);
            }
        }
    }

private:
    template <typename Canvas>
    void blur(const Canvas& c1, Canvas& c2, int radius) const
    {
        typedef typename Canvas::pixel_format pixel_format;
        constexpr unsigned int channels = pixel_format::channels();
        constexpr auto min   = algorithm::min<int>;
        constexpr auto max   = algorithm::max<int>;
        constexpr auto clamp = algorithm::clamp<int>;

        const pixel_format* c1pxls = c1.pixels().data();
              pixel_format* c2pxls = c2.pixels().data();
        int rp1 = radius + 1;
        int ws1 = c1.width() - 1;
        int table_size = 2 * radius + 1;
        stretchy_buffer<unsigned char> lut(256 * table_size);

        for (int i = 0; i < 256; i++)
            memory::std::memset(lut.data() + i * table_size, i, table_size);

        for (unsigned int y = 0; y < c1.height(); y++)
        {
            int xbegin = y * c1.width();
            int sum[channels] = {0};

            for (int i = -radius; i <= radius; ++i)
                for (unsigned int j = 0; j < channels; ++j)
                    sum[j] += c1pxls[xbegin + clamp(i, 0, ws1)].data()[j];

            for (unsigned int x = 0; x < c1.width(); x++)
            {
                int c2_index = x * c2.width() + y;

                for (unsigned int i = 0; i < channels; ++i)
                    c2pxls[c2_index].data()[i] = lut[sum[i]];

                const pixel_format& ph = c1pxls[xbegin + min(x + rp1,  ws1)];
                const pixel_format& pt = c1pxls[xbegin + max(x - radius, 0)];

                for (unsigned int i = 0; i < channels; ++i)
                    sum[i] += ph.data()[i] - pt.data()[i];
            }
        }
    }

private:
    int radius;
    unsigned int iterations;
};

class stack_blur
{
public:
    stack_blur(int radius)
        : radius(radius)
    {}

public:
    template <typename Canvas>
    inline void filter(const Canvas& src, Canvas& dst) const
    {
        if (radius > 0)
        {
            Canvas canvas_temporary(src.height(), src.width());

            blur(src, canvas_temporary, radius);
            dst.resize(src.width(), src.height());
            blur(canvas_temporary, dst, radius);
        }
    }

private:
    template <typename Canvas>
    void blur(const Canvas& c1, Canvas& c2, int radius) const
    {
        typedef typename Canvas::pixel_format pixel_format;
        constexpr unsigned int channels = pixel_format::channels();
        constexpr auto min   = algorithm::min<int>;
        constexpr auto max   = algorithm::max<int>;
        constexpr auto clamp = algorithm::clamp<int>;

        const pixel_format* c1pxls = c1.pixels().data();
              pixel_format* c2pxls = c2.pixels().data();
        int rp1 = radius + 1;
        int ws1 = c1.width() - 1;
        int stack_size = math::sqr(rp1);

        for (unsigned int y = 0; y < c1.height(); ++y)
        {
            int xbegin = y * c1.width();
            struct { int all, l, r; } sum[channels] = {{0}};

            for (int i = -radius; i <= radius; ++i)
            {
                const pixel_format& p = c1pxls[xbegin + clamp(i, 0, ws1)];
                int height = rp1 - math::abs(i);

                for (unsigned int j = 0; j < channels; ++j)
                    sum[j].all += p.data()[j] * height;

                if (i > 0)
                    for (unsigned int j = 0; j < channels; ++j)
                        sum[j].r += p.data()[j];
                else
                    for (unsigned int j = 0; j < channels; ++j)
                        sum[j].l += p.data()[j];
            }

            for (unsigned int x = 0; x < c1.width(); ++x)
            {
                int c2_index = x * c2.width() + y;

                for (unsigned int i = 0; i < channels; ++i)
                    c2pxls[c2_index].data()[i] = sum[i].all / stack_size;

                const pixel_format& ph = c1pxls[xbegin + min(x + rp1,  ws1)];
                const pixel_format& pm = c1pxls[xbegin + min(x + 1,    ws1)];
                const pixel_format& pt = c1pxls[xbegin + max(x - radius, 0)];

                for (unsigned int i = 0; i < channels; ++i)
                {
                    sum[i].all += (sum[i].r += ph.data()[i]) - sum[i].l;
                    sum[i].l   += pm.data()[i] - pt.data()[i];
                    sum[i].r   -= pm.data()[i];
                }
            }
        }
    }

private:
    int radius;
};

class gaussion_blur;

#endif //blur_h_20131127
