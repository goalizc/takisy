#ifndef painter_h_20141030
#define painter_h_20141030

#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/basic/rect.h>
#include <takisy/cgl/basic/canvas.h>
#include <takisy/cgl/basic/raster.h>
#include <takisy/cgl/font/tfont_simple.h>

template <typename Canvas>
class painter
{
public:
    typedef Canvas canvas_type;

public:
    painter(canvas_type& canvas);
    painter(const painter& painter);

    painter& operator=(const painter& painter) = delete;

public:
    inline canvas_type& canvas(void);
    inline const canvas_type canvas(void) const;

public:
    template <typename Brush>
    inline void pixel(point xy, const Brush& brush);
    template <typename Brush>
    inline void pixel(int x, int y, const Brush& brush);

    template <typename Path, typename Brush>
    inline void render(const Path& path, const Brush& brush);

    template <typename PixelMatrix>
    inline void draw(point xy, const PixelMatrix& pixel_matrix);
    template <typename PixelMatrix>
    inline void draw(int x, int y, const PixelMatrix& pixel_matrix);

    template <typename Font, typename Brush>
    inline void text(const wchar_t* string,
                     const Font& font, const Brush& brush);
    template <typename Font, typename Brush>
    inline void text(point xy, const wchar_t* string,
                     const Font& font, const Brush& brush);
    template <typename Font, typename Brush>
    inline void text(int x, int y, const wchar_t* string,
                     const Font& font, const Brush& brush);
    template <typename Font, typename Brush>
    inline void text(rect area, const wchar_t* string,
                     const Font& font, const Brush& brush);
    template <typename Font, typename Brush>
    inline void text(int left, int top, int right, int bottom,
                     const wchar_t* string,
                     const Font& font, const Brush& brush);

private:
    canvas_type& canvas_;
};

/// Implementation

template <typename Canvas>
painter<Canvas>::painter(canvas_type& canvas)
    : canvas_(canvas)
{}

template <typename Canvas>
painter<Canvas>::painter(const painter& painter)
    : canvas_(painter.canvas_)
{}

template <typename Canvas>
typename painter<Canvas>::canvas_type& painter<Canvas>::canvas(void)
{
    return canvas_;
}

template <typename Canvas>
const typename painter<Canvas>::canvas_type painter<Canvas>::canvas(void) const
{
    return canvas_;
}

template <typename Canvas>
template <typename Brush>
void painter<Canvas>::pixel(point xy, const Brush& brush)
{
    canvas_.pixel(xy.x, xy.y, brush);
}

template <typename Canvas>
template <typename Brush>
void painter<Canvas>::pixel(int x, int y, const Brush& brush)
{
    canvas_.pixel(x, y, brush);
}

template <typename Canvas>
template <typename Path, typename Brush>
void painter<Canvas>::render(const Path& path, const Brush& brush)
{
    raster raster;
    raster.rasterize(path);

    for (int y = raster.bbox().top; y < raster.bbox().bottom; ++y)
    for (const raster::span& span : raster.fetch_scanline(y))
    for (int x = span.x, end_x = span.x + int(span.length); x < end_x; ++x)
        canvas_.pixel(x, y, brush, span.coverage);
}

template <typename Canvas>
template <typename PixelMatrix>
void painter<Canvas>::draw(point xy, const PixelMatrix& pixel_matrix)
{
    draw(xy.x, xy.y, pixel_matrix);
}

template <typename Canvas>
template <typename PixelMatrix>
void painter<Canvas>::draw(int x, int y, const PixelMatrix& pixel_matrix)
{
    for (unsigned int yy = 0; yy < pixel_matrix.height(); ++yy)
    for (unsigned int xx = 0; xx < pixel_matrix.width(); ++xx)
        canvas_.pixel(x + xx, y + yy, pixel_matrix.pixel(xx, yy));
}

template <typename Canvas>
template <typename Font, typename Brush>
void painter<Canvas>::text(const wchar_t* string,
                           const Font& font, const Brush& brush)
{
    text(0, 0, string, font, brush);
}

template <typename Canvas>
template <typename Font, typename Brush>
void painter<Canvas>::text(point xy, const wchar_t* string,
                           const Font& font, const Brush& brush)
{
    text(xy.x, xy.y, string, font, brush);
}

template <typename Canvas>
template <typename Font, typename Brush>
void painter<Canvas>::text(int x, int y, const wchar_t* string,
                           const Font& font, const Brush& brush)
{
    text(x, y, 1 << 24, 1 << 24, string, font, brush);
}

template <typename Canvas>
template <typename Font, typename Brush>
void painter<Canvas>::text(rect area, const wchar_t* string,
                           const Font& font, const Brush& brush)
{
    text(area.left, area.top, area.right, area.bottom, string, font, brush);
}

template <typename Canvas>
template <typename Font, typename Brush>
void painter<Canvas>::text(int left, int top, int right, int bottom,
                           const wchar_t* string,
                           const Font& font, const Brush& brush)
{
    int x = left, y = top;
    int c = 0;
    auto next_line = [&](void) -> bool {
        return x = left, (y += font.height()) < bottom;
    };

    while ((c = *string++))
    {
        switch (c)
        {
        case '\n':
            if (!next_line())
                return;
            break;
        default:
            {
                const bitmap* bitmap = font.get_bitmap(c);
                if (static_cast<int>(x + bitmap->left + bitmap->width) >= right)
                    if (!next_line())
                        return;
                x = bitmap->render(canvas_, x, y, right, bottom, brush);
            }
            break;
        }
    }
}

#endif // painter_h_20141030
