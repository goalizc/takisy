#include <takisy/core/algorithm.h>
#include <takisy/cgl/path/vertex_set.h>
#include <takisy/cgl/path/builder/rectangle.h>
#include <takisy/cgl/path/builder/ellipse.h>
#include <takisy/cgl/path/builder/circle.h>
#include <takisy/gui/basic/graphics.h>
#include "impl/pen.hpp"
#include "impl/graphics.hpp"

graphics::graphics(void)
    : impl_(new implement)
{}

graphics::graphics(const graphics& _graphics)
    : graphics()
{
    operator=(_graphics);
}

graphics::~graphics(void)
{
    delete impl_;
}

graphics& graphics::operator=(const graphics& graphics)
{
    if (this != &graphics)
    {
        impl_->paint();
        impl_->rect_    = graphics.impl_->rect_;
        impl_->poffset_ = graphics.impl_->poffset_;
        impl_->prect_   = graphics.impl_->prect_;
        impl_->mask_    = graphics.impl_->mask_;
        impl_->canvas_  = graphics.impl_->canvas_;
    }

    return *this;
}

color graphics::pixel(int x, int y) const
{
    return impl_->pixel(x, y);
}

color graphics::pixel(const point& point) const
{
    return impl_->pixel(point.x, point.y);
}

void graphics::mask(const mask_type* mask)
{
    impl_->mask_ = mask;
}

void graphics::clear(const color& color)
{
    if (impl_->dc_)
        impl_->canvas_->clear(color);
    else
        fill_rectangle(impl_->prect_, color);
}

void graphics::clear(const brush& brush)
{
    if (impl_->dc_)
        impl_->canvas_->clear(brush);
    else
        fill_rectangle(impl_->prect_, brush);
}

void graphics::pixel(int x, int y, const color& color)
{
    impl_->pixel(x, y, color);
}

void graphics::pixel(int x, int y, const color& color, unsigned char coverage)
{
    impl_->pixel(x, y, color, coverage);
}

void graphics::pixel(const point& point, const color& color)
{
    impl_->pixel(point.x, point.y, color);
}

void graphics::pixel(const point& point, const color& color,
                     unsigned char coverage)
{
    impl_->pixel(point.x, point.y, color, coverage);
}

void graphics::pixel(int x, int y, const brush& brush)
{
    impl_->pixel(x, y, brush(x, y));
}

void graphics::pixel(int x, int y, const brush& brush, unsigned char coverage)
{
    impl_->pixel(x, y, brush(x, y), coverage);
}

void graphics::pixel(const point& point, const brush& brush)
{
    impl_->pixel(point.x, point.y, brush(point.x, point.y));
}

void graphics::pixel(const point& point, const brush& brush,
                     unsigned char coverage)
{
    impl_->pixel(point.x, point.y, brush(point.x, point.y), coverage);
}

void graphics::draw_line(const point& p1, const point& p2, const color& color)
{
    draw_line(p1, p2, color_brush(color));
}

void graphics::draw_line(int p1x, int p1y, int p2x, int p2y, const color& color)
{
    draw_line(p1x, p1y, p2x, p2y, color_brush(color));
}

void graphics::draw_line(const point& p1, const point& p2, const brush& brush)
{
    draw_line(p1.x, p1.y, p2.x, p2.y, brush);
}

void graphics::draw_line(int p1x, int p1y, int p2x, int p2y, const brush& brush)
{
    if (p2x == p1x)
    {
        int x    = p1x;
        int step = p1y < p2y ? 1 : -1;

        for (int y = p1y; y != p2y; y += step)
            impl_->pixel(x, y, brush(x, y));
    }
    else
    {
        double slope = static_cast<double>(p2y - p1y) / (p2x - p1x);

        if (math::abs(slope) < 1)
        {
            int step = p1x < p2x ? 1 : -1;
            double y = p1y + 0.5;

            slope *= step;

            for (int x = p1x; x != p2x; x += step)
            {
                impl_->pixel(x, y, brush(x, y));

                y += slope;
            }
        }
        else
        {
            int step = p1y < p2y ? 1 : -1;
            double x = p1x + 0.5;

            slope = step / slope;

            for (int y = p1y; y != p2y; y += step)
            {
                impl_->pixel(x, y, brush(x, y));

                x += slope;
            }
        }
    }
}

void graphics::draw_line(const pointf& p1, const pointf& p2, const pen& pen)
{
    draw_line(p1.x, p1.y, p2.x, p2.y, pen);
}

void graphics::draw_line(double p1x, double p1y, double p2x, double p2y,
                         const pen& pen)
{
    vertex_set line;

    line.append(p1x, p1y);
    line.append(p2x, p2y);

    draw_path(line, pen);
}

void graphics::draw_rectangle(const rect& rect, const color& color)
{
    draw_rectangle(rect, color_brush(color));
}

void graphics::draw_rectangle(const point& p1, const point& p2,
                              const color& color)
{
    draw_rectangle(p1, p2, color_brush(color));
}

void graphics::draw_rectangle(const point& point, const size& size,
                              const color& color)
{
    draw_rectangle(point, size, color_brush(color));
}

void graphics::draw_rectangle(int p1x, int p1y, int p2x, int p2y,
                              const color& color)
{
    draw_rectangle(p1x, p1y, p2x, p2y, color_brush(color));
}

void graphics::draw_rectangle(const rect& rect, const brush& brush)
{
    draw_rectangle(rect.left, rect.top, rect.right, rect.bottom, brush);
}

void graphics::draw_rectangle(const point& p1, const point& p2,
                              const brush& brush)
{
    draw_rectangle(p1.x, p1.y, p2.x, p2.y, brush);
}

void graphics::draw_rectangle(const point& point, const size& size,
                              const brush& brush)
{
    draw_rectangle(point.x, point.y,
                   point.x + size.width, point.y + size.height,
                   brush);
}

void graphics::draw_rectangle(int p1x, int p1y, int p2x, int p2y,
                              const brush& brush)
{
    if (p1x > p2x) algorithm::swap(p1x, p2x);
    if (p1y > p2y) algorithm::swap(p1y, p2y);

    for (int x = p1x; x <= p2x; ++x)
    {
        impl_->pixel(x, p1y, brush(x, p1y));
        impl_->pixel(x, p2y, brush(x, p2y));
    }

    for (int y = p1y + 1; y < p2y; ++y)
    {
        impl_->pixel(p1x, y, brush(p1x, y));
        impl_->pixel(p2x, y, brush(p2x, y));
    }
}

void graphics::draw_rectangle(const rectf& rect, const pen& pen)
{
    draw_rectangle(rect.left, rect.top, rect.right, rect.bottom, pen);
}

void graphics::draw_rectangle(const pointf& p1, const pointf& p2,
                              const pen& pen)
{
    draw_rectangle(p1.x, p1.y, p2.x, p2.y, pen);
}

void graphics::draw_rectangle(const pointf& point, const sizef& size,
                              const pen& pen)
{
    draw_rectangle(point.x, point.y,
                   point.x + size.width, point.y + size.height,
                   pen);
}

void graphics::draw_rectangle(double p1x, double p1y, double p2x, double p2y,
                              const pen& pen)
{
    draw_path(rectangle().build(p1x, p1y, p2x, p2y), true, pen);
}

void graphics::draw_round_rectangle(const rectf& rect, double radius,
                                    const pen& pen)
{
    draw_round_rectangle(rect, radius, radius, pen);
}

void graphics::draw_round_rectangle(const pointf& p1, const pointf& p2,
                                    double radius, const pen& pen)
{
    draw_round_rectangle(p1, p2, radius, radius, pen);
}

void graphics::draw_round_rectangle(const pointf& point, const sizef& size,
                                    double radius, const pen& pen)
{
    draw_round_rectangle(point, size, radius, radius, pen);
}

void graphics::draw_round_rectangle(double p1x, double p1y,
                                    double p2x, double p2y, double radius,
                                    const pen& pen)
{
    draw_round_rectangle(p1x, p1y, p2x, p2y, radius, radius, pen);
}

void graphics::draw_round_rectangle(const rectf& rect,
                                    double width, double height,
                                    const pen& pen)
{
    draw_round_rectangle(rect.left, rect.top, rect.right, rect.bottom,
                         width, height,
                         pen);
}

void graphics::draw_round_rectangle(const pointf& p1, const pointf& p2,
                                    double width, double height,
                                    const pen& pen)
{
    draw_round_rectangle(p1.x, p1.y, p2.x, p2.y, width, height, pen);
}

void graphics::draw_round_rectangle(const pointf& point, const sizef& size,
                                    double width, double height,
                                    const pen& pen)
{
    draw_round_rectangle(point.x, point.y,
                         point.x + size.width, point.y + size.height,
                         width, height,
                         pen);
}

void graphics::draw_round_rectangle(double p1x, double p1y,
                                    double p2x, double p2y,
                                    double width, double height,
                                    const pen& pen)
{
    draw_path(rectangle().build(p1x, p1y, p2x, p2y, width, height), true, pen);
}

void graphics::draw_ellipse(const pointf& center, double width, double height,
                            const pen& pen)
{
    draw_ellipse(center.x, center.y, width, height, pen);
}

void graphics::draw_ellipse(double x, double y, double width, double height,
                            const pen& pen)
{
    draw_path(ellipse(x, y, width, height).build(), true, pen);
}

void graphics::draw_circle(const pointf& center, double radius, const pen& pen)
{
    draw_circle(center.x, center.y, radius, pen);
}

void graphics::draw_circle(double x, double y, double radius, const pen& pen)
{
    draw_path(circle(x, y, radius).build(), true, pen);
}

void graphics::draw_path(const path& path, const pen& pen)
{
    draw_path(path, false, pen);
}

void graphics::draw_path(const path& path, bool closed, const pen& pen)
{
    if (pen.impl_->brush_)
        impl_->render(pen.impl_->build(path, closed), *pen.impl_->brush_);
}

void graphics::draw_path(const paths& paths, const pen& pen)
{
    draw_path(paths, false, pen);
}

void graphics::draw_path(const paths& paths, bool closed, const pen& pen)
{
    if (pen.impl_->brush_)
        impl_->render(pen.impl_->build(paths, closed), *pen.impl_->brush_);
}

void graphics::draw_text(const point& point,
                         const wchar_t* string, const font& font,
                         const color& color)
{
    draw_text(point, string, font, color_brush(color));
}

void graphics::draw_text(const rect& rect,
                         const wchar_t* string, const font& font,
                         const color& color)
{
    draw_text(rect, string, font, color_brush(color));
}

void graphics::draw_text(const point& p1, const point& p2,
                         const wchar_t* string, const font& font,
                         const color& color)
{
    draw_text(p1, p2, string, font, color_brush(color));
}

void graphics::draw_text(const point& point, const size& size,
                         const wchar_t* string, const font& font,
                         const color& color)
{
    draw_text(point, size, string, font, color_brush(color));
}

void graphics::draw_text(int x, int y,
                         const wchar_t* string, const font& font,
                         const color& color)
{
    draw_text(x, y, string, font, color_brush(color));
}

void graphics::draw_text(int p1x, int p1y, int p2x, int p2y,
                         const wchar_t* string, const font& font,
                         const color& color)
{
    draw_text(p1x, p1y, p2x, p2y, string, font, color_brush(color));
}

void graphics::draw_text(const point& point,
                         const wchar_t* string, const font& font,
                         const brush& brush)
{
    draw_text(point.x, point.y, string, font, brush);
}

void graphics::draw_text(const rect& rect,
                         const wchar_t* string, const font& font,
                         const brush& brush)
{
    draw_text(rect.left, rect.top, rect.right, rect.bottom,
              string, font,
              brush);
}

void graphics::draw_text(const point& p1, const point& p2,
                         const wchar_t* string, const font& font,
                         const brush& brush)
{
    draw_text(p1.x, p1.y, p2.x, p2.y, string, font, brush);
}

void graphics::draw_text(const point& point, const size& size,
                         const wchar_t* string, const font& font,
                         const brush& brush)
{
    draw_text(point.x, point.y, point.x + size.width, point.y + size.height,
              string, font,
              brush);
}

void graphics::draw_text(int x, int y,
                         const wchar_t* string, const font& font,
                         const brush& brush)
{
    draw_text(x, y, 1 << 24, 1 << 24, string, font, brush);
}

void graphics::draw_text(int p1x, int p1y, int p2x, int p2y,
                         const wchar_t* string, const font& font,
                         const brush& brush)
{
    int x = p1x, y = p1y;
    int char_code = 0;
    auto next_line = [&](void) -> bool {
        return x = p1x, (y += font.height()) < p2y;
    };

    while ((char_code = *string++))
    {
        switch (char_code)
        {
        case '\n':
            if (!next_line())
                return;
            break;
        default:
            {
                const bitmap* bitmap = font.get_bitmap(char_code);
                if (static_cast<int>(x + bitmap->left + bitmap->width) >= p2x)
                    if (!next_line())
                        return;
                x = bitmap->render(*impl_, x, y, p2x, p2y, brush(x, y));
            }
            break;
        }
    }
}

void graphics::draw_image(const canvas_adapter& canvas)
{
    draw_image(0, 0, canvas);
}

void graphics::draw_image(const canvas_adapter& canvas,
                          int x, int y, unsigned int width, unsigned int height)
{
    draw_image(0, 0, canvas, x, y, width, height);
}

void graphics::draw_image(const canvas_adapter& canvas,
                          const point& p1, const point& p2)
{
    draw_image(0, 0, canvas, p1, p2);
}

void graphics::draw_image(const canvas_adapter& canvas,
                          const point& point, const size& size)
{
    draw_image(0, 0, canvas, point, size);
}

void graphics::draw_image(const canvas_adapter& canvas, const rect& rect)
{
    draw_image(0, 0, canvas, rect);
}

void graphics::draw_image(int x, int y, const canvas_adapter& canvas)
{
    draw_image(x, y, canvas, 0, 0, canvas.width(), canvas.height());
}

void graphics::draw_image(int x, int y, const canvas_adapter& canvas,
                          int cx, int cy,
                          unsigned int width, unsigned int height)
{
    const rect& r = impl_->prect_;

    if (cx + (int)width < 0 || cy + (int)height < 0
        || cx > (int)canvas.width() || cy > (int)canvas.height())
        return;

    if (cx < 0) x -= cx, width += cx, cx = 0;
    if (cx + (int)width > (int)canvas.width()) width = canvas.width() - cx;
    if (cy < 0) y -= cy, height += cy, cy = 0;
    if (cy + (int)height > (int)canvas.height()) height = canvas.height() - cy;

    if (x + (int)width < r.left || y + (int)height < r.top
        || x > r.right || y > r.bottom)
        return;

    if (x < r.left) cx += r.left - x, width -= r.left - x, x = r.left;
    if (x + (int)width > r.right) width = r.right - x;
    if (y < r.top) cy += r.top - y, height -= r.top - y, y = r.top;
    if (y + (int)height > r.bottom) height = r.bottom - y;

    for (unsigned int i = 0; i < height; ++i)
    for (unsigned int j = 0; j < width;  ++j)
        impl_->pixel(x + j, y + i, canvas.pixel(cx + j, cy + i));
}

void graphics::draw_image(int x, int y, const canvas_adapter& canvas,
                          const point& p1, const point& p2)
{
    draw_image(x, y, canvas, rect(p1, p2));
}

void graphics::draw_image(int x, int y, const canvas_adapter& canvas,
                          const point& point, const size& size)
{
    draw_image(x, y, canvas, rect(point, size));
}

void graphics::draw_image(int x, int y, const canvas_adapter& canvas,
                          const rect& rect)
{
    draw_image(x, y, canvas, rect.left, rect.top, rect.width(), rect.height());
}

void graphics::draw_image(const point& xy, const canvas_adapter& canvas)
{
    draw_image(xy.x, xy.y, canvas);
}

void graphics::draw_image(const point& xy, const canvas_adapter& canvas,
                          int x, int y, unsigned int width, unsigned int height)
{
    draw_image(xy.x, xy.y, canvas, x, y, width, height);
}

void graphics::draw_image(const point& xy, const canvas_adapter& canvas,
                          const point& p1, const point& p2)
{
    draw_image(xy.x, xy.y, canvas, p1, p2);
}

void graphics::draw_image(const point& xy, const canvas_adapter& canvas,
                          const point& point, const size& size)
{
    draw_image(xy.x, xy.y, canvas, point, size);
}

void graphics::draw_image(const point& xy, const canvas_adapter& canvas,
                          const rect& rect)
{
    draw_image(xy.x, xy.y, canvas, rect);
}

void graphics::fill_rectangle(const rect& rect, const color& color)
{
    fill_rectangle(rect, color_brush(color));
}

void graphics::fill_rectangle(const point& p1, const point& p2,
                              const color& color)
{
    fill_rectangle(p1, p2, color_brush(color));
}

void graphics::fill_rectangle(const point& point, const size& size,
                              const color& color)
{
    fill_rectangle(point, size, color_brush(color));
}

void graphics::fill_rectangle(int p1x, int p1y, int p2x, int p2y,
                              const color& color)
{
    fill_rectangle(p1x, p1y, p2x, p2y, color_brush(color));
}

void graphics::fill_rectangle(const rect& rect, const brush& brush)
{
    ::rect r = rect.normalize();

    fill_rectangle(r.left, r.top, r.right, r.bottom, brush);
}

void graphics::fill_rectangle(const point& p1, const point& p2,
                              const brush& brush)
{
    fill_rectangle(rect(p1, p2), brush);
}

void graphics::fill_rectangle(const point& point, const size& size,
                              const brush& brush)
{
    fill_rectangle(rect(point, size), brush);
}

void graphics::fill_rectangle(int p1x, int p1y, int p2x, int p2y,
                              const brush& brush)
{
    if (p1x > p2x) algorithm::swap(p1x, p2x);
    if (p1y > p2y) algorithm::swap(p1y, p2y);

    for (int y = p1y; y < p2y; ++y)
    for (int x = p1x; x < p2x; ++x)
        impl_->pixel(x, y, brush(x, y));
}

void graphics::fill_rectangle(const rectf& rect, const brush& brush)
{
    fill_rectangle(rect.left, rect.top, rect.right, rect.bottom, brush);
}

void graphics::fill_rectangle(const pointf& p1, const pointf& p2,
                              const brush& brush)
{
    fill_rectangle(p1.x, p1.y, p2.x, p2.y, brush);
}

void graphics::fill_rectangle(const pointf& point, const sizef& size,
                              const brush& brush)
{
    fill_rectangle(point.x, point.x + size.width,
                   point.y, point.y + size.height, brush);
}

void graphics::fill_rectangle(double p1x, double p1y, double p2x, double p2y,
                              const brush& brush)
{
    fill_path(rectangle().build(p1x, p1y, p2x, p2y), brush);
}

void graphics::fill_round_rectangle(const rectf& rect, double radius,
                                    const color& color)
{
    fill_round_rectangle(rect, radius, color_brush(color));
}

void graphics::fill_round_rectangle(const pointf& p1, const pointf& p2,
                                    double radius,
                                    const color& color)
{
    fill_round_rectangle(p1, p2, radius, color_brush(color));
}

void graphics::fill_round_rectangle(const pointf& point, const sizef& size,
                                    double radius,
                                    const color& color)
{
    fill_round_rectangle(point, size, radius, color_brush(color));
}

void graphics::fill_round_rectangle(double p1x, double p1y,
                                    double p2x, double p2y,
                                    double radius,
                                    const color& color)
{
    fill_round_rectangle(p1x, p1y, p2x, p2y, radius, color_brush(color));
}

void graphics::fill_round_rectangle(const rectf& rect,
                                    double width, double height,
                                    const color& color)
{
    fill_round_rectangle(rect, width, height, color_brush(color));
}

void graphics::fill_round_rectangle(const pointf& p1, const pointf& p2,
                                    double width, double height,
                                    const color& color)
{
    fill_round_rectangle(p1, p2, width, height, color_brush(color));
}

void graphics::fill_round_rectangle(const pointf& point, const sizef& size,
                                    double width, double height,
                                    const color& color)
{
    fill_round_rectangle(point, size, width, height, color_brush(color));
}

void graphics::fill_round_rectangle(double p1x, double p1y,
                                    double p2x, double p2y,
                                    double width, double height,
                                    const color& color)
{
    fill_round_rectangle(p1x, p1y, p2x, p2y, width, height, color_brush(color));
}

void graphics::fill_round_rectangle(const rectf& rect, double radius,
                                    const brush& brush)
{
    fill_round_rectangle(rect, radius, radius, brush);
}

void graphics::fill_round_rectangle(const pointf& p1, const pointf& p2,
                                    double radius,
                                    const brush& brush)
{
    fill_round_rectangle(p1, p2, radius, radius, brush);
}

void graphics::fill_round_rectangle(const pointf& point, const sizef& size,
                                    double radius,
                                    const brush& brush)
{
    fill_round_rectangle(point, size, radius, radius, brush);
}

void graphics::fill_round_rectangle(double p1x, double p1y,
                                    double p2x, double p2y, double radius,
                                    const brush& brush)
{
    fill_round_rectangle(p1x, p1y, p2x, p2y, radius, radius, brush);
}

void graphics::fill_round_rectangle(const rectf& rect,
                                    double width, double height,
                                    const brush& brush)
{
    fill_round_rectangle(rect.left, rect.top, rect.right, rect.bottom,
                         width, height,
                         brush);
}

void graphics::fill_round_rectangle(const pointf& p1, const pointf& p2,
                                    double width, double height,
                                    const brush& brush)
{
    fill_round_rectangle(p1.x, p1.y, p2.x, p2.y, width, height, brush);
}

void graphics::fill_round_rectangle(const pointf& point, const sizef& size,
                                    double width, double height,
                                    const brush& brush)
{
    fill_round_rectangle(point.x, point.y,
                         point.x + size.width, point.y + size.height,
                         width, height,
                         brush);
}

void graphics::fill_round_rectangle(double p1x, double p1y,
                                    double p2x, double p2y,
                                    double width, double height,
                                    const brush& brush)
{
    fill_path(rectangle().build(p1x, p1y, p2x, p2y, width, height), brush);
}

void graphics::fill_ellipse(const pointf& center, double width, double height,
                            const color& color)
{
    fill_ellipse(center, width, height, color_brush(color));
}

void graphics::fill_ellipse(double x, double y, double width, double height,
                            const color& color)
{
    fill_ellipse(x, y, width, height, color_brush(color));
}

void graphics::fill_ellipse(const pointf& center, double width, double height,
                            const brush& brush)
{
    fill_ellipse(center.x, center.y, width, height, brush);
}

void graphics::fill_ellipse(double x, double y, double width, double height,
                            const brush& brush)
{
    fill_path(ellipse(x, y, width, height).build(), brush);
}

void graphics::fill_circle(const pointf& center, double radius,
                           const color& color)
{
    fill_circle(center, radius, color_brush(color));
}

void graphics::fill_circle(double x, double y, double radius,
                           const color& color)
{
    fill_circle(x, y, radius, color_brush(color));
}

void graphics::fill_circle(const pointf& center, double radius,
                           const brush& brush)
{
    fill_circle(center.x, center.y, radius, brush);
}

void graphics::fill_circle(double x, double y, double radius,
                           const brush& brush)
{
    fill_path(circle(x, y, radius).build(), brush);
}

void graphics::fill_path(const path& path, const color& color)
{
    fill_path(path, color_brush(color));
}

void graphics::fill_path(const paths& paths, const color& color)
{
    fill_path(paths, color_brush(color));
}

void graphics::fill_path(const path& path, const brush& brush)
{
    impl_->render(path, brush);
}

void graphics::fill_path(const paths& paths, const brush& brush)
{
    impl_->render(paths, brush);
}
