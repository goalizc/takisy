#ifndef canvas_h_20131115
#define canvas_h_20131115

#include <algorithm>
#include <takisy/core/memory.h>
#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/basic/color.h>
#include <takisy/cgl/basic/gamma.h>
#include <takisy/cgl/basic/pixel_format.h>

template <typename PixelFormat, typename GammaType = gamma_null>
class canvas {
public:
    typedef PixelFormat pixel_format;
    typedef GammaType   gamma_type;
    typedef stretchy_buffer<pixel_format, false, true> pixels_type;

public:
    canvas(void) : width_(0), height_(0) {}
    canvas(unsigned int width, unsigned int height)
        : width_(width), height_(height), pixels_(width * height) {}
    canvas(const canvas& canvas) { operator=(canvas); }
    canvas(canvas&& canvas) { operator=(std::move(canvas)); }
    template <typename PixelMatrix>
    canvas(const PixelMatrix& pixel_matrix)
        : canvas() { operator=(pixel_matrix); }
   ~canvas(void) {}

    canvas& operator=(const canvas& canvas) {
        if (this != &canvas) {
            width_ = canvas.width_; height_ = canvas.height_;
            gamma_ = canvas.gamma_; pixels_ = canvas.pixels_.copy();
        }
        return *this;
    }

    canvas& operator=(canvas&& canvas) {
        width_ = canvas.width_; height_ = canvas.height_;
        gamma_ = canvas.gamma_; pixels_ = canvas.pixels_;
        return *this;
    }

    template <typename PixelMatrix>
    canvas& operator=(const PixelMatrix& pixel_matrix) {
        resize(pixel_matrix.width(), pixel_matrix.height());
        clear([&pixel_matrix](unsigned int x, unsigned int y) {
            return pixel_matrix.pixel(x, y);
        });
        return *this;
    }

public:
    void clear(void) {
        memset(pixels_.data(), 0, pixels_.size() * pixel_format::pixel_bytes());
    }

    template <typename Brush>
    void clear(const Brush& brush) {
        for (unsigned int y = 0; y < height_; ++y)
        for (unsigned int x = 0; x < width_;  ++x)
            unsafe_pixel(x, y) = brush(x, y);
    }

    void clear(const color& color) {
        typedef memory::std memstd;
        memstd::memset<pixel_format>(pixels_.data(), color, pixels_.size());
    }

    void resize(unsigned int width, unsigned int height) {
        if (width_ == width) {
            height_ = height;
            pixels_.resize(width * height);
        } else {
            pixels_type pixels(width * height);
            unsigned int xlimit = width  < width_  ? width  : width_;
            unsigned int ylimit = height < height_ ? height : height_;
            for (unsigned int y = 0; y < ylimit; ++y)
                memcpy(&pixels[y * width], row(y),
                       xlimit * pixel_format::pixel_bytes());
            width_ = width; height_ = height;
            pixels_.swap(pixels);
        }
    }

    void swap(canvas& canvas) {
        std::swap(width_,  canvas.width_);
        std::swap(height_, canvas.height_);
        std::swap(gamma_,  canvas.gamma_);
        pixels_.swap(canvas.pixels_);
    }

    template <typename GammaFunction>
    void gamma(const GammaFunction& gamma_function) {
        return gamma_.refresh_gamma_lut(gamma_function);
    }

public:
    unsigned int width(void)  const { return width_;  }
    unsigned int height(void) const { return height_; }

    pixels_type& pixels(void)
        { return pixels_; }
    const pixels_type& pixels(void) const
        { return pixels_; }
    pixel_format* row(int y)
        { return pixels_.data() + y * width_; }
    const pixel_format* row(int y) const
        { return pixels_.data() + y * width_; }
    pixel_format& unsafe_pixel(int x, int y)
        { return row(y)[x]; }
    const pixel_format& unsafe_pixel(int x, int y) const
        { return row(y)[x]; }
    const pixel_format& pixel(int x, int y) const {
        x = x < 0 ? 0 : x >= (int)width_  ? (int)width_  - 1 : x;
        y = y < 0 ? 0 : y >= (int)height_ ? (int)height_ - 1 : y;
        return row(y)[x];
    }

public:
    template <typename Brush>
    void pixel(unsigned int x, unsigned int y, const Brush& brush)
        { pixel(x, y, brush(x, y)); }
    void pixel(unsigned int x, unsigned int y, const color& color)
        { if (is_validxy(x, y)) unsafe_pixel(x, y).blend(color); }
    void pixel(unsigned int x, unsigned int y, const absolute_color& color)
        { if (is_validxy(x, y)) unsafe_pixel(x, y) = color; }
    template <typename Brush>
    void pixel(unsigned int x, unsigned int y,
               const Brush& brush, color::channel_type covarage)
        { pixel(x, y, brush(x, y), covarage); }
    void pixel(unsigned int x, unsigned int y,
               const color& color, color::channel_type covarage)
        { if (is_validxy(x, y))
            unsafe_pixel(x, y).blend(color, gamma_.gamma_value(covarage)); }
    void pixel(unsigned int x, unsigned int y,
               const absolute_color& color, color::channel_type covarage)
        { if (is_validxy(x, y))
            unsafe_pixel(x, y) = color * gamma_.gamma_value(covarage); }

private:
    bool is_validxy(unsigned int x, unsigned int y) const {
        return x < width_ && y < height_;
    }

private:
    unsigned int width_, height_;
    gamma_type   gamma_;
    pixels_type  pixels_;
};

typedef canvas<pixfmt_G8>      canvas_G8;
typedef canvas<pixfmt_Ga8>     canvas_Ga8;
typedef canvas<pixfmt_aG8>     canvas_aG8;
typedef canvas<pixfmt_rgb8>    canvas_rgb8;
typedef canvas<pixfmt_bgr8>    canvas_bgr8;
typedef canvas<pixfmt_rgba8>   canvas_rgba8;
typedef canvas<pixfmt_bgra8>   canvas_bgra8;
typedef canvas<pixfmt_argb8>   canvas_argb8;
typedef canvas<pixfmt_abgr8>   canvas_abgr8;
typedef canvas<pixfmt_mask8>   canvas_a8;
typedef canvas_a8              canvas_mask8;

typedef canvas<pixfmt_G8be>    canvas_G8be;
typedef canvas<pixfmt_Ga8be>   canvas_Ga8be;
typedef canvas<pixfmt_aG8be>   canvas_aG8be;
typedef canvas<pixfmt_rgb8be>  canvas_rgb8be;
typedef canvas<pixfmt_bgr8be>  canvas_bgr8be;
typedef canvas<pixfmt_rgba8be> canvas_rgba8be;
typedef canvas<pixfmt_bgra8be> canvas_bgra8be;
typedef canvas<pixfmt_argb8be> canvas_argb8be;
typedef canvas<pixfmt_abgr8be> canvas_abgr8be;
typedef canvas<pixfmt_mask8be> canvas_a8be;
typedef canvas_a8be            canvas_mask8be;

typedef canvas<pixfmt_G8le>    canvas_G8le;
typedef canvas<pixfmt_Ga8le>   canvas_Ga8le;
typedef canvas<pixfmt_aG8le>   canvas_aG8le;
typedef canvas<pixfmt_rgb8le>  canvas_rgb8le;
typedef canvas<pixfmt_bgr8le>  canvas_bgr8le;
typedef canvas<pixfmt_rgba8le> canvas_rgba8le;
typedef canvas<pixfmt_bgra8le> canvas_bgra8le;
typedef canvas<pixfmt_argb8le> canvas_argb8le;
typedef canvas<pixfmt_abgr8le> canvas_abgr8le;
typedef canvas<pixfmt_mask8le> canvas_a8le;
typedef canvas_a8le            canvas_mask8le;

typedef canvas<pixfmt_G16>      canvas_G16;
typedef canvas<pixfmt_Ga16>     canvas_Ga16;
typedef canvas<pixfmt_aG16>     canvas_aG16;
typedef canvas<pixfmt_rgb16>    canvas_rgb16;
typedef canvas<pixfmt_bgr16>    canvas_bgr16;
typedef canvas<pixfmt_rgba16>   canvas_rgba16;
typedef canvas<pixfmt_bgra16>   canvas_bgra16;
typedef canvas<pixfmt_argb16>   canvas_argb16;
typedef canvas<pixfmt_abgr16>   canvas_abgr16;
typedef canvas<pixfmt_mask16>   canvas_a16;
typedef canvas_a16              canvas_mask16;

typedef canvas<pixfmt_G16be>    canvas_G16be;
typedef canvas<pixfmt_Ga16be>   canvas_Ga16be;
typedef canvas<pixfmt_aG16be>   canvas_aG16be;
typedef canvas<pixfmt_rgb16be>  canvas_rgb16be;
typedef canvas<pixfmt_bgr16be>  canvas_bgr16be;
typedef canvas<pixfmt_rgba16be> canvas_rgba16be;
typedef canvas<pixfmt_bgra16be> canvas_bgra16be;
typedef canvas<pixfmt_argb16be> canvas_argb16be;
typedef canvas<pixfmt_abgr16be> canvas_abgr16be;
typedef canvas<pixfmt_mask16be> canvas_a16be;
typedef canvas_a16be            canvas_mask16be;

typedef canvas<pixfmt_G16le>    canvas_G16le;
typedef canvas<pixfmt_Ga16le>   canvas_Ga16le;
typedef canvas<pixfmt_aG16le>   canvas_aG16le;
typedef canvas<pixfmt_rgb16le>  canvas_rgb16le;
typedef canvas<pixfmt_bgr16le>  canvas_bgr16le;
typedef canvas<pixfmt_rgba16le> canvas_rgba16le;
typedef canvas<pixfmt_bgra16le> canvas_bgra16le;
typedef canvas<pixfmt_argb16le> canvas_argb16le;
typedef canvas<pixfmt_abgr16le> canvas_abgr16le;
typedef canvas<pixfmt_mask16le> canvas_a16le;
typedef canvas_a16le            canvas_mask16le;

#endif //canvas_h_20131115
