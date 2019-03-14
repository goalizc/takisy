#ifndef graphics_h_20131115
#define graphics_h_20131115

#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/basic/color.h>
#include <takisy/cgl/basic/canvas.h>

template <typename Canvas>
class graphics
{
public:
    typedef Canvas canvas_type;

public:
    graphics(void);
    graphics(unsigned int width, unsigned int height);
    template <typename PixelMatrix>
    graphics(const PixelMatrix& pixel_matrix);
    graphics(const graphics& graphics);
   ~graphics(void);

    template <typename PixelMatrix>
    graphics& operator=(const PixelMatrix& pixel_matrix);
    graphics& operator=(const graphics& graphics);

public:
    inline void clear(void);
    template <typename Brush>
    inline void clear(const Brush& brush);

    inline void resize(unsigned int width, unsigned int height);

    template <typename GammaFunction>
    inline void gamma(const GammaFunction& gf);

public:
    inline unsigned int width(void) const;
    inline unsigned int height(void) const;

    inline color pixel(const point& point) const;
    inline color pixel(int x, int y) const;

    inline canvas_type& canvas(void);
    inline typename canvas_type::pixel_format* pixels(void);
    inline const typename canvas_type::pixel_format* pixels(void) const;

public:
    template <typename PixelMatrix>
    inline PixelMatrix& copy(PixelMatrix& pixel_matrix) const;
    template <typename PixelMatrix>
    inline PixelMatrix& copy(const point& lt, const point& rb,
                             PixelMatrix& pixel_matrix) const;
    template <typename PixelMatrix>
    inline PixelMatrix& copy(int lt_x, int lt_y, int rb_x, int rb_y,
                             PixelMatrix& pixel_matrix) const;

public:
    template <typename Brush>
    inline void pixel(const point& point, const Brush& brush);
    template <typename Brush>
    inline void pixel(int x, int y, const Brush& brush);

    template <typename Path, typename Brush>
    inline void render(const Path& path, const Brush& brush);

    template <typename PixelMatrix>
    inline void paint(const PixelMatrix& pixel_matrix);
    template <typename PixelMatrix>
    inline void paint(const PixelMatrix& pixel_matrix,
                      const point& lt, const point& rb);
    template <typename PixelMatrix>
    inline void paint(const point& position, const PixelMatrix& pixel_matrix);
    template <typename PixelMatrix>
    inline void paint(const point& position, const PixelMatrix& pixel_matrix,
                      const point& lt, const point& rb);
    template <typename PixelMatrix>
    inline void paint(int x, int y, const PixelMatrix& pixel_matrix);
    template <typename PixelMatrix>
    inline void paint(const PixelMatrix& pixel_matrix,
                      int lt_x, int lt_y, int rb_x, int rb_y);
    template <typename PixelMatrix>
    inline void paint(int x, int y, const PixelMatrix& pixel_matrix,
                      int lt_x, int lt_y, int rb_x, int rb_y);

    template <typename Brush>
    inline void text(const char* string, const Brush& brush);
    template <typename Brush>
    inline void text(const point& lt, const char* string, const Brush& brush);
    template <typename Brush>
    inline void text(const point& lt,
                     const point& rb, const char* string, const Brush& brush);
    template <typename Brush>
    inline void text(int lt_x, int lt_y,
                     const char* string, const Brush& brush);
    template <typename Brush>
    inline void text(int lt_x, int lt_y,
                     int rb_x, int rb_y,
                     const char* string, const Brush& brush);

public:
    inline bool save(const char* filename);

public:
    template <typename DeviceContext>
    inline void play(DeviceContext dc,
                     bool up2down = true, int x = 0, int y = 0,
                     unsigned int width = 0, unsigned int height = 0);

private:
    canvas_type canvas_;
};

#include <takisy/core/algo.h>
#include <takisy/core/osdet.h>
#include <takisy/cgl/basic/raster.h>
#include <takisy/cgl/font/tfont_simple.h>
#include <takisy/cgl/image/image.h>

template <typename Canvas>
graphics<Canvas>::graphics(void)
{}

template <typename Canvas>
graphics<Canvas>::graphics(unsigned int width, unsigned int height)
    : canvas_(width, height)
{}

template <typename Canvas>
template <typename PixelMatrix>
graphics<Canvas>::graphics(const PixelMatrix& pixel_matrix)
    : canvas_(pixel_matrix)
{}

template <typename Canvas>
graphics<Canvas>::graphics(const graphics& graphics)
    : canvas_(graphics.canvas_)
{}

template <typename Canvas>
graphics<Canvas>::~graphics(void)
{}

template <typename Canvas>
template <typename PixelMatrix>
graphics<Canvas>& graphics<Canvas>::operator=(const PixelMatrix& pixel_matrix)
{
    canvas_ = pixel_matrix;

    return *this;
}

template <typename Canvas>
graphics<Canvas>& graphics<Canvas>::operator=(const graphics& graphics)
{
    if (this != &graphics)
        canvas_ = graphics.canvas_;

    return *this;
}

template <typename Canvas>
void graphics<Canvas>::clear(void)
{
    canvas_.clear();
}

template <typename Canvas>
template <typename Brush>
void graphics<Canvas>::clear(const Brush& brush)
{
    canvas_.clear(brush);
}

template <typename Canvas>
void graphics<Canvas>::resize(unsigned int width, unsigned int height)
{
    canvas_.resize(width, height);
}

template <typename Canvas>
template <typename GammaFunction>
void graphics<Canvas>::gamma(const GammaFunction& gf)
{
    canvas_.gamma(gf);
}

template <typename Canvas>
unsigned int graphics<Canvas>::width(void) const
{
    return canvas_.width();
}

template <typename Canvas>
unsigned int graphics<Canvas>::height(void) const
{
    return canvas_.height();
}

template <typename Canvas>
color graphics<Canvas>::pixel(const point& point) const
{
    return canvas_.pixel(point.x, point.y);
}

template <typename Canvas>
color graphics<Canvas>::pixel(int x, int y) const
{
    return canvas_.pixel(x, y);
}

template <typename Canvas>
typename graphics<Canvas>::canvas_type& graphics<Canvas>::canvas(void)
{
    return canvas_;
}

template <typename Canvas>
typename graphics<Canvas>::canvas_type::pixel_format*
    graphics<Canvas>::pixels(void)
{
    return canvas_.pixels().data();
}

template <typename Canvas>
const typename graphics<Canvas>::canvas_type::pixel_format*
    graphics<Canvas>::pixels(void) const
{
    return canvas_.pixels().data();
}

template <typename Canvas>
template <typename PixelMatrix>
PixelMatrix& graphics<Canvas>::copy(PixelMatrix& pixel_matrix) const
{
    return copy(0, 0, width(), height(), pixel_matrix);
}

template <typename Canvas>
template <typename PixelMatrix>
PixelMatrix& graphics<Canvas>::copy(const point& lt, const point& rb,
                                    PixelMatrix& pixel_matrix) const
{
    return copy(lt.x, lt.y, rb.x, rb.y, pixel_matrix);
}

template <typename Canvas>
template <typename PixelMatrix>
PixelMatrix& graphics<Canvas>::copy(int lt_x, int lt_y, int rb_x, int rb_y,
                                    PixelMatrix& pixel_matrix) const
{
    if (lt_x > rb_x) algo::swap(lt_x, rb_x);
    if (lt_y > rb_y) algo::swap(lt_y, rb_y);

    pixel_matrix.resize(rb_x - lt_x, rb_y - lt_y);

    for (unsigned int y = 0; y < pixel_matrix.height(); ++y)
    for (unsigned int x = 0; x < pixel_matrix.width(); ++x)
        pixel_matrix.pixel(x, y, pixel(x + lt_x, y + lt_y));

    return pixel_matrix;
}

template <typename Canvas>
template <typename Brush>
void graphics<Canvas>::pixel(const point& point, const Brush& brush)
{
    canvas_.pixel(point.x, point.y, brush);
}

template <typename Canvas>
template <typename Brush>
void graphics<Canvas>::pixel(int x, int y, const Brush& brush)
{
    canvas_.pixel(x, y, brush);
}

template <typename Canvas>
template <typename Path, typename Brush>
void graphics<Canvas>::render(const Path& path, const Brush& brush)
{
    static raster raster;

    raster.rasterize(path);

    for (int y = raster.bbox().top; y < raster.bbox().bottom; ++y)
    for (const raster::span& span : raster.fetch_scanline(y))
    for (int x = span.x, end_x = span.x + int(span.length); x < end_x; ++x)
        canvas_.pixel(x, y, brush, span.coverage);
}

template <typename Canvas>
template <typename PixelMatrix>
void graphics<Canvas>::paint(const PixelMatrix& pixel_matrix)
{
    paint(0, 0, pixel_matrix);
}

template <typename Canvas>
template <typename PixelMatrix>
void graphics<Canvas>::paint(const PixelMatrix& pixel_matrix,
                             const point& lt, const point& rb)
{
    paint(0, 0, pixel_matrix, lt, rb);
}

template <typename Canvas>
template <typename PixelMatrix>
void graphics<Canvas>::paint(const point& position,
                             const PixelMatrix& pixel_matrix)
{
    paint(position.x, position.y, pixel_matrix);
}

template <typename Canvas>
template <typename PixelMatrix>
void graphics<Canvas>::paint(const point& position,
                             const PixelMatrix& pixel_matrix,
                             const point& lt, const point& rb)
{
    paint(position.x, position.y, pixel_matrix, lt, rb);
}

template <typename Canvas>
template <typename PixelMatrix>
void graphics<Canvas>::paint(int x, int y, const PixelMatrix& pixel_matrix)
{
    paint(x, y,
          pixel_matrix, 0, 0, pixel_matrix.width(), pixel_matrix.height());
}

template <typename Canvas>
template <typename PixelMatrix>
void graphics<Canvas>::paint(const PixelMatrix& pixel_matrix,
                             int lt_x, int lt_y, int rb_x, int rb_y)
{
    paint(0, 0, pixel_matrix, lt_x, lt_y, rb_x, rb_y);
}

template <typename Canvas>
template <typename PixelMatrix>
void graphics<Canvas>::paint(int x, int y,
                             const PixelMatrix& pixel_matrix,
                             int lt_x, int lt_y, int rb_x, int rb_y)
{
    if (lt_x > rb_x) algo::swap(lt_x, rb_x);
    if (lt_y > rb_y) algo::swap(lt_y, rb_y);

    int xbegin = x < 0 ? -x : 0;
    int ybegin = y < 0 ? -y : 0;
    int xend   = algo::min<int>(width()  - x, rb_x - lt_x);
    int yend   = algo::min<int>(height() - y, rb_y - lt_y);

    for (int j = ybegin; j < yend; ++j)
    for (int i = xbegin; i < xend; ++i)
        canvas_.pixel(x + i, y + j, pixel_matrix.pixel(lt_x + i, lt_y + j));
}

template <typename Canvas>
template <typename Brush>
void graphics<Canvas>::text(const char* string, const Brush& brush)
{
    text(0, 0, width(), height(), string, brush);
}

template <typename Canvas>
template <typename Brush>
void graphics<Canvas>::text(const point& lt, const point& rb,
                            const char* string, const Brush& brush)
{
    text(lt.x, lt.y, rb.x, rb.y, string, brush);
}

template <typename Canvas>
template <typename Brush>
void graphics<Canvas>::text(const point& lt,
                            const char* string, const Brush& brush)
{
    text(lt.x, lt.y, string, brush);
}

template <typename Canvas>
template <typename Brush>
void graphics<Canvas>::text(int lt_x, int lt_y,
                            const char* string, const Brush& brush)
{
    text(lt_x, lt_y, lt_x + 1e9, lt_y + 1e9, string, brush);
}

template <typename Canvas>
template <typename Brush>
void graphics<Canvas>::text(int lt_x, int lt_y, int rb_x, int rb_y,
                            const char* string, const Brush& brush)
{
    static tfont_simple font;
    int x = lt_x, y = lt_y;
    int c = 0;

    while ((c = *string++))
    {
        switch (c)
        {
        case '\n':
            x  = lt_x;
            y += font.height();
            break;

        default:
            {
                const bitmap* bitmap = font.get_bitmap(c);

                if (static_cast<int>(x + bitmap->left + bitmap->width) >= rb_x)
                    x  = lt_x,
                    y += font.height();

                bitmap->render(canvas_, x, y, rb_x, rb_y, brush);

                x += bitmap->advance;
            }
            break;
        }
    }
}

template <typename Canvas>
bool graphics<Canvas>::save(const char* filename)
{
    image image;

    image.new_frame(*this);

    return image.dump_uri(filename);
}

#include <Windows.h>

template <>
template <typename HDC>
void graphics<canvas_bgr8>::play(HDC dc,
                                 bool up2down, int x, int y,
                                 unsigned int width, unsigned int height)
{
    BITMAPINFO bmi = {
        .bmiHeader = BITMAPINFOHEADER {
            .biSize          = sizeof(BITMAPINFOHEADER),
            .biWidth         = (LONG)(canvas_.width()),
            .biHeight        = (LONG)(canvas_.height()) * (up2down ? -1 : 1),
            .biPlanes        = 1,
            .biBitCount      = canvas_type::pixel_format::pixel_bits(),
            .biCompression   = BI_RGB,
            .biSizeImage     = 0,
            .biXPelsPerMeter = 0,
            .biYPelsPerMeter = 0,
            .biClrUsed       = 0,
            .biClrImportant  = 0, },
        .bmiColors = {0, },
    };

    typedef typename canvas_type::pixel_format pixel_format;
    unsigned char* pixels_data = (unsigned char*)canvas_.pixels().data();
    unsigned int row_size = canvas_.width() * pixel_format::pixel_bytes();
    bool need_free = false;

    if (row_size % 4 != 0)
    {
        unsigned int   fix_row_size    = math::ceil(row_size / 4.0) * 4;
        unsigned char* fix_pixels_data =
            memory::std::malloc<unsigned char>(fix_row_size * canvas_.height());

        for (unsigned int y = 0; y < canvas_.height(); ++y)
            memcpy(fix_pixels_data + y * fix_row_size,
                   pixels_data + y * row_size, row_size);

        pixels_data = fix_pixels_data;
        need_free   = true;
    }

    SetDIBitsToDevice(dc, x, y,
            width  == 0 ? canvas_.width()  : width,
            height == 0 ? canvas_.height() : height,
            0, 0, 0, canvas_.height(), pixels_data, &bmi, DIB_RGB_COLORS);

    if (need_free)
        memory::std::free(pixels_data);
}

template <typename Canvas>
template <typename HDC>
inline void graphics<Canvas>::play(HDC dc,
                                   bool up2down, int x, int y,
                                   unsigned int width, unsigned int height)
{
    int w = math::ceil(canvas_.width() / 4.0) * 4, h = canvas_.height();
    graphics<canvas_bgr8> fixed_graphics(w, h);

    fixed_graphics.clear(color::white());
    for (unsigned int y = 0; y < fixed_graphics.height(); ++y)
    for (unsigned int x = 0; x < fixed_graphics.width(); ++x)
        if ((x % 10 < 5) ^ (y % 10 < 5))
            fixed_graphics.pixel(x, y, color::light_gray());

    fixed_graphics.paint(*this);

    return fixed_graphics.play(dc, up2down, x, y, width, height);
}

#elif defined(unix)

    // add unix play method here.

#elif defined(linux)

    // add linux play method here.

#endif

typedef graphics<canvas_G8>      graphics_G8;
typedef graphics<canvas_Ga8>     graphics_Ga8;
typedef graphics<canvas_aG8>     graphics_aG8;
typedef graphics<canvas_rgb8>    graphics_rgb8;
typedef graphics<canvas_bgr8>    graphics_bgr8;
typedef graphics<canvas_rgba8>   graphics_rgba8;
typedef graphics<canvas_bgra8>   graphics_bgra8;
typedef graphics<canvas_argb8>   graphics_argb8;
typedef graphics<canvas_abgr8>   graphics_abgr8;
typedef graphics<canvas_mask8>   graphics_a8;
typedef graphics_a8              graphics_mask8;

typedef graphics<canvas_G8be>    graphics_G8be;
typedef graphics<canvas_Ga8be>   graphics_Ga8be;
typedef graphics<canvas_aG8be>   graphics_aG8be;
typedef graphics<canvas_rgb8be>  graphics_rgb8be;
typedef graphics<canvas_bgr8be>  graphics_bgr8be;
typedef graphics<canvas_rgba8be> graphics_rgba8be;
typedef graphics<canvas_bgra8be> graphics_bgra8be;
typedef graphics<canvas_argb8be> graphics_argb8be;
typedef graphics<canvas_abgr8be> graphics_abgr8be;
typedef graphics<canvas_mask8be> graphics_a8be;
typedef graphics_a8be            graphics_mask8be;

typedef graphics<canvas_G8le>    graphics_G8le;
typedef graphics<canvas_Ga8le>   graphics_Ga8le;
typedef graphics<canvas_aG8le>   graphics_aG8le;
typedef graphics<canvas_rgb8le>  graphics_rgb8le;
typedef graphics<canvas_bgr8le>  graphics_bgr8le;
typedef graphics<canvas_rgba8le> graphics_rgba8le;
typedef graphics<canvas_bgra8le> graphics_bgra8le;
typedef graphics<canvas_argb8le> graphics_argb8le;
typedef graphics<canvas_abgr8le> graphics_abgr8le;
typedef graphics<canvas_mask8le> graphics_a8le;
typedef graphics_a8le            graphics_mask8le;

typedef graphics<canvas_G16>      graphics_G16;
typedef graphics<canvas_Ga16>     graphics_Ga16;
typedef graphics<canvas_aG16>     graphics_aG16;
typedef graphics<canvas_rgb16>    graphics_rgb16;
typedef graphics<canvas_bgr16>    graphics_bgr16;
typedef graphics<canvas_rgba16>   graphics_rgba16;
typedef graphics<canvas_bgra16>   graphics_bgra16;
typedef graphics<canvas_argb16>   graphics_argb16;
typedef graphics<canvas_abgr16>   graphics_abgr16;
typedef graphics<canvas_mask16>   graphics_a16;
typedef graphics_a16              graphics_mask16;

typedef graphics<canvas_G16be>    graphics_G16be;
typedef graphics<canvas_Ga16be>   graphics_Ga16be;
typedef graphics<canvas_aG16be>   graphics_aG16be;
typedef graphics<canvas_rgb16be>  graphics_rgb16be;
typedef graphics<canvas_bgr16be>  graphics_bgr16be;
typedef graphics<canvas_rgba16be> graphics_rgba16be;
typedef graphics<canvas_bgra16be> graphics_bgra16be;
typedef graphics<canvas_argb16be> graphics_argb16be;
typedef graphics<canvas_abgr16be> graphics_abgr16be;
typedef graphics<canvas_mask16be> graphics_a16be;
typedef graphics_a16be            graphics_mask16be;

typedef graphics<canvas_G16le>    graphics_G16le;
typedef graphics<canvas_Ga16le>   graphics_Ga16le;
typedef graphics<canvas_aG16le>   graphics_aG16le;
typedef graphics<canvas_rgb16le>  graphics_rgb16le;
typedef graphics<canvas_bgr16le>  graphics_bgr16le;
typedef graphics<canvas_rgba16le> graphics_rgba16le;
typedef graphics<canvas_bgra16le> graphics_bgra16le;
typedef graphics<canvas_argb16le> graphics_argb16le;
typedef graphics<canvas_abgr16le> graphics_abgr16le;
typedef graphics<canvas_mask16le> graphics_a16le;
typedef graphics_a16le            graphics_mask16le;
