#ifndef canvas_h_20131115
#define canvas_h_20131115

#include <takisy/core/memory.h>
#include <takisy/core/algorithm.h>
#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/basic/size.h>
#include <takisy/cgl/basic/color.h>
#include <takisy/cgl/basic/gamma.h>
#include <takisy/cgl/basic/pixel_format.h>

template <typename PixelFormat,
          typename GammaType = gamma_null>
class canvas
{
public:
    typedef PixelFormat pixel_format;
    typedef GammaType   gamma_type;
    typedef stretchy_buffer<pixel_format> pixels_type;

public:
    canvas(void)
        : size_(0, 0)
    {}

    canvas(unsigned int width, unsigned int height)
        : size_(width, height), pixels_(width * height)
    {}

    canvas(const canvas& canvas)
        : size_  (canvas.size_)
        , gamma_ (canvas.gamma_)
        , pixels_(canvas.pixels_.clone())
    {}

    template <typename PixalMatrix>
    canvas(const PixalMatrix& pixel_matrix)
    {
        operator=(pixel_matrix);
    }

    ~canvas(void)
    {}

    canvas& operator=(const canvas& canvas)
    {
        if (this != &canvas)
        {
            size_   = canvas.size_;
            gamma_  = canvas.gamma_;
            pixels_ = canvas.pixels_.clone();
        }

        return *this;
    }

    template <typename PixalMatrix>
    canvas& operator=(const PixalMatrix& pixel_matrix)
    {
        size_.width  = pixel_matrix.width();
        size_.height = pixel_matrix.height();

        pixels_.resize(size_.width * size_.height);

        clear(
            [&pixel_matrix](int x, int y)
            {
                return pixel_matrix.pixel(x, y);
            });

        return *this;
    }

public:
    inline void clear(void)
    {
        clear(color::black(0));
    }

    template <typename Brush>
    void clear(const Brush& brush)
    {
        for (unsigned int y = 0; y < size_.height; ++y)
        for (unsigned int x = 0; x < size_.width;  ++x)
            unsafe_pixel(x, y) = brush(x, y);
    }

    inline void clear(const color& color)
    {
        memory::memset<pixel_format>(pixels_.data(), color, pixels_.size());
    }

    void resize(unsigned int width, unsigned int height)
    {
        if (size_.width == width)
        {
            size_.height = height;
            pixels_.resize(size_.width * size_.height);
        }
        else
        {
            pixels_type pixels(width * height);

            unsigned int xlimit = width  < size_.width  ? width  : size_.width;
            unsigned int ylimit = height < size_.height ? height : size_.height;

            for (unsigned int y = 0; y < ylimit; ++y)
                memcpy(&pixels [y * width],
                       &pixels_[y * size_.width],
                       xlimit * pixel_format::pixel_bytes());

            size_.width  = width;
            size_.height = height;

            pixels_.swap(pixels);
        }
    }

    inline void swap(canvas& canvas)
    {
        algorithm::swap(size_ ,  canvas.size_);
        algorithm::swap(gamma_,  canvas.gamma_);
        algorithm::swap(pixels_, canvas.pixels_);
    }

    template <typename GammaFunction>
    inline void gamma(const GammaFunction& gamma_function)
    {
        return gamma_.refresh_gamma_lut(gamma_function);
    }

public:
    inline unsigned int width(void) const
    {
        return size_.width;
    }

    inline unsigned int height(void) const
    {
        return size_.height;
    }

    inline const pixel_format& pixel(register int x, register int y) const
    {
        x = algorithm::clamp(x, 0, (int)size_.width  - 1);
        y = algorithm::clamp(y, 0, (int)size_.height - 1);

        return row(y)[x];
    }

    inline pixel_format& unsafe_pixel(int x, int y)
    {
        return row(y)[x];
    }

    inline const pixel_format& unsafe_pixel(int x, int y) const
    {
        return row(y)[x];
    }

    inline pixel_format* row(int y)
    {
        return pixels_.data() + y * size_.width;
    }

    inline const pixel_format* row(int y) const
    {
        return pixels_.data() + y * size_.width;
    }

    inline pixels_type& pixels(void)
    {
        return pixels_;
    }

    inline const pixels_type& pixels(void) const
    {
        return pixels_;
    }

public:
    template <typename Brush>
    inline void pixel(unsigned int x, unsigned int y, const Brush& brush)
    {
        pixel(x, y, brush(x, y));
    }

    inline void pixel(unsigned int x, unsigned int y, const color& color)
    {
        if (valid_xy(x, y))
            unsafe_pixel(x, y).blend(color);
    }

    template <typename Brush>
    inline void pixel(unsigned int x, unsigned int y,
                      const Brush& brush, color::channel_type covarage)
    {
        pixel(x, y, brush(x, y), covarage);
    }

    inline void pixel(unsigned int x, unsigned int y,
                      const color& color, color::channel_type covarage)
    {
        if (valid_xy(x, y))
            unsafe_pixel(x, y).blend(color, gamma_.gamma_value(covarage));
    }

private:
    inline bool valid_xy(unsigned int x, unsigned int y) const
    {
        return x < size_.width && y < size_.height;
    }

private:
    sizeu size_;
    gamma_type gamma_;
    pixels_type pixels_;
};

typedef canvas<pf_G8>      canvas_G8;
typedef canvas<pf_Ga8>     canvas_Ga8;
typedef canvas<pf_aG8>     canvas_aG8;
typedef canvas<pf_rgb8>    canvas_rgb8;
typedef canvas<pf_bgr8>    canvas_bgr8;
typedef canvas<pf_rgba8>   canvas_rgba8;
typedef canvas<pf_bgra8>   canvas_bgra8;
typedef canvas<pf_argb8>   canvas_argb8;
typedef canvas<pf_abgr8>   canvas_abgr8;
typedef canvas<pf_mask8>   canvas_a8;
typedef canvas_a8          canvas_mask8;

typedef canvas<pf_G8be>    canvas_G8be;
typedef canvas<pf_Ga8be>   canvas_Ga8be;
typedef canvas<pf_aG8be>   canvas_aG8be;
typedef canvas<pf_rgb8be>  canvas_rgb8be;
typedef canvas<pf_bgr8be>  canvas_bgr8be;
typedef canvas<pf_rgba8be> canvas_rgba8be;
typedef canvas<pf_bgra8be> canvas_bgra8be;
typedef canvas<pf_argb8be> canvas_argb8be;
typedef canvas<pf_abgr8be> canvas_abgr8be;
typedef canvas<pf_mask8be> canvas_a8be;
typedef canvas_a8be        canvas_mask8be;

typedef canvas<pf_G8le>    canvas_G8le;
typedef canvas<pf_Ga8le>   canvas_Ga8le;
typedef canvas<pf_aG8le>   canvas_aG8le;
typedef canvas<pf_rgb8le>  canvas_rgb8le;
typedef canvas<pf_bgr8le>  canvas_bgr8le;
typedef canvas<pf_rgba8le> canvas_rgba8le;
typedef canvas<pf_bgra8le> canvas_bgra8le;
typedef canvas<pf_argb8le> canvas_argb8le;
typedef canvas<pf_abgr8le> canvas_abgr8le;
typedef canvas<pf_mask8le> canvas_a8le;
typedef canvas_a8le        canvas_mask8le;

typedef canvas<pf_G16>      canvas_G16;
typedef canvas<pf_Ga16>     canvas_Ga16;
typedef canvas<pf_aG16>     canvas_aG16;
typedef canvas<pf_rgb16>    canvas_rgb16;
typedef canvas<pf_bgr16>    canvas_bgr16;
typedef canvas<pf_rgba16>   canvas_rgba16;
typedef canvas<pf_bgra16>   canvas_bgra16;
typedef canvas<pf_argb16>   canvas_argb16;
typedef canvas<pf_abgr16>   canvas_abgr16;
typedef canvas<pf_mask16>   canvas_a16;
typedef canvas_a16          canvas_mask16;

typedef canvas<pf_G16be>    canvas_G16be;
typedef canvas<pf_Ga16be>   canvas_Ga16be;
typedef canvas<pf_aG16be>   canvas_aG16be;
typedef canvas<pf_rgb16be>  canvas_rgb16be;
typedef canvas<pf_bgr16be>  canvas_bgr16be;
typedef canvas<pf_rgba16be> canvas_rgba16be;
typedef canvas<pf_bgra16be> canvas_bgra16be;
typedef canvas<pf_argb16be> canvas_argb16be;
typedef canvas<pf_abgr16be> canvas_abgr16be;
typedef canvas<pf_mask16be> canvas_a16be;
typedef canvas_a16be        canvas_mask16be;

typedef canvas<pf_G16le>    canvas_G16le;
typedef canvas<pf_Ga16le>   canvas_Ga16le;
typedef canvas<pf_aG16le>   canvas_aG16le;
typedef canvas<pf_rgb16le>  canvas_rgb16le;
typedef canvas<pf_bgr16le>  canvas_bgr16le;
typedef canvas<pf_rgba16le> canvas_rgba16le;
typedef canvas<pf_bgra16le> canvas_bgra16le;
typedef canvas<pf_argb16le> canvas_argb16le;
typedef canvas<pf_abgr16le> canvas_abgr16le;
typedef canvas<pf_mask16le> canvas_a16le;
typedef canvas_a16le        canvas_mask16le;

#endif //canvas_h_20131115
