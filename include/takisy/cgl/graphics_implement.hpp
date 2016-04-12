#ifdef graphics_h_20131115

#ifndef graphics_implement_h_20131115
#define graphics_implement_h_20131115

#include <takisy/core/macro.h>
#include <takisy/core/algorithm.h>
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
PixelMatrix& graphics<Canvas>::clone(PixelMatrix& pixel_matrix) const
{
    return clone(0, 0, width(), height(), pixel_matrix);
}

template <typename Canvas>
template <typename PixelMatrix>
PixelMatrix& graphics<Canvas>::clone(const point& lt, const point& rb,
                                     PixelMatrix& pixel_matrix) const
{
    return clone(lt.x, lt.y, rb.x, rb.y, pixel_matrix);
}

template <typename Canvas>
template <typename PixelMatrix>
PixelMatrix& graphics<Canvas>::clone(int lt_x, int lt_y, int rb_x, int rb_y,
                                     PixelMatrix& pixel_matrix) const
{
    if (lt_x > rb_x) algorithm::swap(lt_x, rb_x);
    if (lt_y > rb_y) algorithm::swap(lt_y, rb_y);

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
    if (lt_x > rb_x) algorithm::swap(lt_x, rb_x);
    if (lt_y > rb_y) algorithm::swap(lt_y, rb_y);

    int xbegin = x < 0 ? -x : 0;
    int ybegin = y < 0 ? -y : 0;
    int xend   = algorithm::min<int>(width()  - x, rb_x - lt_x);
    int yend   = algorithm::min<int>(height() - y, rb_y - lt_y);

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

    return image.dump_file(filename);
}

#ifdef OS_WIN

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
            memory::std::memcpy(fix_pixels_data + y * fix_row_size,
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

    // TODO：add unix play method here.

#elif defined(linux)

    // TODO：add linux play method here.

#endif

#endif //graphics_implement_h_20131115

#endif //graphics_h_20131115
