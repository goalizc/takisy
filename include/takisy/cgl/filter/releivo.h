#ifndef releivo_h_20140110
#define releivo_h_20140110

class releivo
{
public:
    releivo(void)
        : releivo(1)
    {}

    releivo(double ratio)
        : releivo(ratio, 1, 1)
    {}

    releivo(double ratio, int dx, int dy)
        : releivo(ratio, dx, dy, 127)
    {}

    releivo(double ratio, int dx, int dy, unsigned char surface)
        : ratio(ratio), dx(dx), dy(dy), surface(surface)
    {}

public:
    template <typename Canvas>
    inline void filter(const Canvas& src, Canvas& dst) const
    {
        if (&src == &dst) {
            Canvas canvas_temporary;
            filter(src, canvas_temporary);
            dst.swap(canvas_temporary);
        } else {
            typedef typename Canvas::pixel_format pixel_format;
            constexpr int channel_mask = pixel_format::channel_mask();

            dst.resize(src.width(), src.height());

            for (unsigned int y = 0; y < dst.height(); ++y)
            for (unsigned int x = 0; x < dst.width();  ++x) {
                int g1 = src.unsafe_pixel(x, y).grayscale();
                int g2 = src.pixel(x + dx, y + dy).grayscale();

                dst.unsafe_pixel(x, y).grayscale(
                    clamp((g1 - g2) * ratio + surface, 0, channel_mask));
            }
        }
    }

private:
    static inline int clamp(int n, int lower, int upper)
    {
        return n < lower ? lower : (n > upper ? upper : n);
    }

private:
    double ratio;
    int dx, dy;
    unsigned char surface;
};

#endif // releivo_h_20140110
