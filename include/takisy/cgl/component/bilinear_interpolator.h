#ifndef bilinear_interpolator_h_20131203
#define bilinear_interpolator_h_20131203

#include <takisy/core/math.h>
#include <takisy/core/algorithm.h>
#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/basic/pixel_format.h>

template <typename Canvas>
class bilinear_interpolator
{
public:
    typedef typename Canvas::pixel_format pixel_format;

    class subpixel
    {
    public:
        enum
        {
            shift = 8,
            scale = 1 << shift,
            mask  = scale - 1,
        };

    public:
        static inline int i(int n) { return n >> shift; }
        static inline int f(int n) { return n &  mask;  }
    };

public:
    bilinear_interpolator(const Canvas& canvas)
        : canvas_(canvas)
    {}

public:
    static inline constexpr int shift(void) { return subpixel::shift; }
    static inline constexpr int scale(void) { return subpixel::scale; }
    static inline constexpr int mask (void) { return subpixel::mask ; }

public:
    inline pixel_format pixelf(const pointf& point) const
    {
        return pixelf(point.x, point.y);
    }

    inline pixel_format pixelf(double x, double y) const
    {
        return pixel(x * subpixel::scale, y * subpixel::scale);
    }

    inline pixel_format pixel(const point& shifted_point) const
    {
        return pixel(shifted_point.x, shifted_point.y);
    }

    pixel_format pixel(int shifted_x, int shifted_y) const
    {
        typedef typename pixel_format::data_type data_type;
        constexpr unsigned int shift_x2 = shift() << 1;
        pixel_format rtn;

        int xi = subpixel::i(shifted_x), xf = subpixel::f(shifted_x);
        int yi = subpixel::i(shifted_y), yf = subpixel::f(shifted_y);

        register const data_type* lt = canvas_.pixel(xi + 0, yi + 0).data();
        register const data_type* rt = canvas_.pixel(xi + 1, yi + 0).data();
        register const data_type* lb = canvas_.pixel(xi + 0, yi + 1).data();
        register const data_type* rb = canvas_.pixel(xi + 1, yi + 1).data();

        for (unsigned int i = 0; i < pixel_format::channels(); ++i)
        {
            register int t = (rt[i] - lt[i]) * xf + (lt[i] << shift());
            register int b = (rb[i] - lb[i]) * xf + (lb[i] << shift());

            rtn.data()[i] = ((b - t) * yf + (t << shift())) >> shift_x2;
        }

        return rtn;
    }

private:
    const Canvas& canvas_;
};

template <typename Canvas>
inline bilinear_interpolator<Canvas>
    make_bilinear_interpolator(const Canvas& canvas)
{
    return bilinear_interpolator<Canvas>(canvas);
}

#endif //bilinear_interpolator_h_20131203
