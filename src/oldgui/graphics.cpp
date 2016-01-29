#include <memory>
#include <takisy/core/algorithm.h>
#include <takisy/cgl/path/vertex_set.h>
#include <takisy/cgl/path/builder/rectangle.h>
#include <takisy/cgl/path/builder/ellipse.h>
#include <takisy/cgl/path/builder/circle.h>
#include <takisy/oldgui/basic/graphics.h>
#include "impl/pen.hpp"
#include "impl/graphics.hpp"

Graphics::Graphics(HWND hwnd)
    : Graphics(hwnd, Implement::clientRect(hwnd))
{}

Graphics::Graphics(HWND hwnd, Rect rect)
    : impl_(new Implement(hwnd, rect))
{}

Graphics::Graphics(HWND hwnd, Point left_top, Point right_bottom)
    : Graphics(hwnd, Rect(left_top, right_bottom))
{}

Graphics::Graphics(HWND hwnd, Point point, Size size)
    : Graphics(hwnd, Rect(point, size))
{}

Graphics::Graphics(HWND hwnd, int x, int y,
                   unsigned int width, unsigned int height)
    : Graphics(hwnd, Rect(x, y, x + width, y + height))
{}

Graphics::Graphics(HDC hdc, Rect rect)
    : Graphics(static_cast<HWND>(nullptr), rect)
{
    impl_->hdc_ = hdc;
}

Graphics::Graphics(HDC hdc, Point left_top, Point right_bottom)
    : Graphics(hdc, Rect(left_top, right_bottom))
{}

Graphics::Graphics(HDC hdc, Point point, Size size)
    : Graphics(hdc, Rect(point, size))
{}

Graphics::Graphics(HDC hdc, int x, int y,
                   unsigned int width, unsigned int height)
    : Graphics(hdc, Rect(x, y, x + width, y + height))
{}

Graphics::Graphics(const PAINTSTRUCT& ps)
    : Graphics(ps.hdc, ps.rcPaint)
{}

Graphics::Graphics(const Graphics& graphics)
    : Graphics(static_cast<HWND>(nullptr), Rect(0, 0, 0, 0))
{
    operator=(graphics);
}

Graphics::~Graphics(void)
{
    delete impl_;
}

Graphics& Graphics::operator=(const Graphics& graphics)
{
    if (this != &graphics)
    {
        impl_->paint();

        impl_->rect_         = graphics.impl_->rect_;
        impl_->paint_offset_ = graphics.impl_->paint_offset_;
        impl_->paint_rect_   = graphics.impl_->paint_rect_;
        impl_->canvas_       = graphics.impl_->canvas_;
    }

    return *this;
}

Rect Graphics::paintRect(void) const
{
    return impl_->paint_rect_;
}

void Graphics::clear(Color color)
{
    clear(ColorBrush(color));
}

void Graphics::clear(const Brush& brush)
{
    if (impl_->hdc_)
        impl_->canvas_->clear(brush);
    else
        fillRectangle(impl_->paint_rect_, brush);
}

Color Graphics::pixel(Point point) const
{
    return impl_->pixel(point.x, point.y);
}

void Graphics::pixel(Point point, Color color)
{
    impl_->pixel(point.x, point.y, color);
}

void Graphics::pixel(Point point, Color color, unsigned int coverage)
{
    impl_->pixel(point.x, point.y, color, coverage);
}

void Graphics::pixel(Point point, const Brush& brush)
{
    pixel(point.x, point.y, brush(point.x, point.y));
}

void Graphics::pixel(Point point, const Brush& brush, unsigned int coverage)
{
    pixel(point.x, point.y, brush(point.x, point.y), coverage);
}

Color Graphics::pixel(int x, int y) const
{
    return impl_->pixel(x, y);
}

void Graphics::pixel(int x, int y, Color color)
{
    impl_->pixel(x, y, color);
}

void Graphics::pixel(int x, int y, Color color, unsigned int coverage)
{
    impl_->pixel(x, y, color, coverage);
}

void Graphics::pixel(int x, int y, const Brush& brush)
{
    pixel(x, y, brush(x, y));
}

void Graphics::pixel(int x, int y, const Brush& brush, unsigned int coverage)
{
    pixel(x, y, brush(x, y), coverage);
}

void Graphics::drawLine(Point from, Point to, const Color& color)
{
    drawLine(from, to, ColorBrush(color));
}

void Graphics::drawLine(int from_x, int from_y, int to_x, int to_y,
                        const Color& color)
{
    drawLine(from_x, from_y, to_x, to_y, ColorBrush(color));
}

void Graphics::drawLine(Point from, Point to, const Brush& brush)
{
    drawLine(from.x, from.y, to.x, to.y, brush);
}

void Graphics::drawLine(int from_x, int from_y, int to_x, int to_y,
                        const Brush& brush)
{
    if (to_x == from_x)
    {
        int x    = from_x;
        int step = from_y < to_y ? 1 : -1;

        for (int y = from_y; y != to_y; y += step)
            impl_->pixel(x, y, brush(x, y));
    }

    double slope = static_cast<double>(to_y - from_y) / (to_x - from_x);

    if (math::abs(slope) < 1)
    {
        int step = from_x < to_x ? 1 : -1;
        double y = from_y + 0.5;

        slope *= step;

        for (int x = from_x; x != to_x; x += step)
        {
            impl_->pixel(x, y, brush(x, y));

            y += slope;
        }
    }
    else
    {
        int step = from_y < to_y ? 1 : -1;
        double x = from_x + 0.5;

        slope = step / slope;

        for (int y = from_y; y != to_y; y += step)
        {
            impl_->pixel(x, y, brush(x, y));

            x += slope;
        }
    }
}

void Graphics::drawLine(PointF from, PointF to, const Pen& pen)
{
    drawLine(from.x, from.y, to.x, to.y, pen);
}

void Graphics::drawLine(double from_x, double from_y, double to_x, double to_y,
                        const Pen& pen)
{
    vertex_set line;

    line.append(from_x, from_y);
    line.append(to_x, to_y);

    drawPath(line, pen);
}

void Graphics::drawRectangle(Rect rect, const Color& color)
{
    drawRectangle(rect, ColorBrush(color));
}

void Graphics::drawRectangle(Point from, Point to, const Color& color)
{
    drawRectangle(from, to, ColorBrush(color));
}

void Graphics::drawRectangle(Point point, Size size, const Color& color)
{
    drawRectangle(point, size, ColorBrush(color));
}

void Graphics::drawRectangle(int from_x, int from_y, int to_x, int to_y,
                             const Color& color)
{
    drawRectangle(from_x, from_y, to_x, to_y, ColorBrush(color));
}

void Graphics::drawRectangle(Rect rect, const Brush& brush)
{
    drawRectangle(rect.left, rect.top, rect.right, rect.bottom, brush);
}

void Graphics::drawRectangle(Point from, Point to, const Brush& brush)
{
    drawRectangle(from.x, from.y, to.x, to.y, brush);
}

void Graphics::drawRectangle(Point point, Size size, const Brush& brush)
{
    drawRectangle(point.x, point.y,
                  point.x + size.width, point.y + size.height, brush);
}

void Graphics::drawRectangle(int from_x, int from_y, int to_x, int to_y,
                             const Brush& brush)
{
    if (from_x > to_x) algorithm::swap(from_x, to_x);
    if (from_y > to_y) algorithm::swap(from_y, to_y);

    for (int x = from_x; x <= to_x; ++x)
    {
        impl_->pixel(x, from_y, brush(x, from_y));
        impl_->pixel(x, to_y,   brush(x, to_y));
    }

    for (int y = from_y + 1; y < to_y; ++y)
    {
        impl_->pixel(from_x, y, brush(from_x, y));
        impl_->pixel(to_x,   y, brush(to_x, y));
    }
}

void Graphics::drawRectangle(RectF rect, const Pen& pen)
{
    drawRectangle(rect.left, rect.top, rect.right, rect.bottom, pen);
}

void Graphics::drawRectangle(PointF from, PointF to, const Pen& pen)
{
    drawRectangle(from.x, from.y, to.x, to.y, pen);
}

void Graphics::drawRectangle(PointF point, SizeF size, const Pen& pen)
{
    drawRectangle(point.x, point.y,
                  point.x + size.width, point.y + size.height, pen);
}

void Graphics::drawRectangle(double from_x, double from_y,
                             double to_x, double to_y, const Pen& pen)
{
    drawPath(rectangle().build(from_x, from_y, to_x, to_y), true, pen);
}

void Graphics::drawRoundRectangle(RectF rect, double radius, const Pen& pen)
{
    drawRoundRectangle(rect, radius, radius, pen);
}

void Graphics::drawRoundRectangle(PointF from, PointF to, double radius,
                                  const Pen& pen)
{
    drawRoundRectangle(from, to, radius, radius, pen);
}

void Graphics::drawRoundRectangle(PointF point, SizeF size, double radius,
                                  const Pen& pen)
{
    drawRoundRectangle(point, size, radius, radius, pen);
}

void Graphics::drawRoundRectangle(double from_x, double from_y,
                                  double to_x, double to_y, double radius,
                                  const Pen& pen)
{
    drawRoundRectangle(from_x, from_y, to_x, to_y, radius, radius, pen);
}

void Graphics::drawRoundRectangle(RectF rect, double width, double height,
                                  const Pen& pen)
{
    drawRoundRectangle(rect.left, rect.top, rect.right, rect.bottom,
                       width, height, pen);
}

void Graphics::drawRoundRectangle(PointF from, PointF to,
                                  double width, double height, const Pen& pen)
{
    drawRoundRectangle(from.x, from.y, to.x, to.y, width, height, pen);
}

void Graphics::drawRoundRectangle(PointF point, SizeF size,
                                  double width, double height, const Pen& pen)
{
    drawRoundRectangle(point.x, point.y,
                       point.x + size.width, point.y + size.height,
                       width, height, pen);
}

void Graphics::drawRoundRectangle(double from_x, double from_y,
                                  double to_x, double to_y,
                                  double width, double height, const Pen& pen)
{
    drawPath(rectangle().build(from_x, from_y, to_x, to_y, width, height),
             true, pen);
}

void Graphics::drawEllipse(PointF center, double width, double height,
                           const Pen& pen)
{
    drawEllipse(center.x, center.y, width, height, pen);
}

void Graphics::drawEllipse(double x, double y, double width, double height,
                           const Pen& pen)
{
    drawPath(ellipse(x, y, width, height).build(), true, pen);
}

void Graphics::drawCircle(PointF center, double radius, const Pen& pen)
{
    drawCircle(center.x, center.y, radius, pen);
}

void Graphics::drawCircle(double x, double y, double radius, const Pen& pen)
{
    drawPath(circle(x, y, radius).build(), true, pen);
}

void Graphics::drawPath(const Path& path, const Pen& pen)
{
    drawPath(path, false, pen);
}

void Graphics::drawPath(const Path& path, bool closed, const Pen& pen)
{
    if (pen.impl_->brush_)
        impl_->renderPath(pen.impl_->build(path, closed), *pen.impl_->brush_);
}

void Graphics::drawPath(const Paths& paths, const Pen& pen)
{
    drawPath(paths, false, pen);
}

void Graphics::drawPath(const Paths& paths, bool closed, const Pen& pen)
{
    if (pen.impl_->brush_)
        impl_->renderPath(pen.impl_->build(paths, closed), *pen.impl_->brush_);
}

void Graphics::drawText(Point point, const wchar_t* string,
                        const Font& font, const Color& color)
{
    drawText(point, string, font, ColorBrush(color));
}

void Graphics::drawText(Rect rect, const wchar_t* string,
                        const Font& font, const Color& color)
{
    drawText(rect, string, font, ColorBrush(color));
}

void Graphics::drawText(Point from, Point to, const wchar_t* string,
                        const Font& font, const Color& color)
{
    drawText(from, to, string, font, ColorBrush(color));
}

void Graphics::drawText(Point point, Size size, const wchar_t* string,
                        const Font& font, const Color& color)
{
    drawText(point, size, string, font, ColorBrush(color));
}

void Graphics::drawText(int x, int y, const wchar_t* string,
                        const Font& font, const Color& color)
{
    drawText(x, y, string, font, ColorBrush(color));
}

void Graphics::drawText(int from_x, int from_y, int to_x, int to_y,
                        const wchar_t* string,
                        const Font& font, const Color& color)
{
    drawText(from_x, from_y, to_x, to_y, string, font, ColorBrush(color));
}

void Graphics::drawText(Point point, const wchar_t* string,
                        const Font& font, const Brush& brush)
{
    drawText(point.x, point.y, string, font, brush);
}

void Graphics::drawText(Rect rect, const wchar_t* string,
                        const Font& font, const Brush& brush)
{
    drawText(rect.left, rect.top, rect.right, rect.bottom,
             string, font, brush);
}

void Graphics::drawText(Point from, Point to, const wchar_t* string,
                        const Font& font, const Brush& brush)
{
    drawText(from.x, from.y, to.x, to.y, string, font, brush);
}

void Graphics::drawText(Point point, Size size, const wchar_t* string,
                        const Font& font, const Brush& brush)
{
    drawText(point.x, point.y, point.x + size.width, point.y + size.height,
             string, font, brush);
}

void Graphics::drawText(int x, int y, const wchar_t* string,
                        const Font& font, const Brush& brush)
{
    drawText(x, y, 1 << 24, 1 << 24, string, font, brush);
}

void Graphics::drawText(int from_x, int from_y, int to_x, int to_y,
                        const wchar_t* string,
                        const Font& font, const Brush& brush)
{
    int x = from_x, y = from_y;
    int char_code = 0;
    auto next_line = [&](void) -> bool {
        return x = from_x, (y += font.height()) < to_y;
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

                if (static_cast<int>(x + bitmap->left + bitmap->width) > to_x)
                    if (!next_line())
                        return;

                x = bitmap->render(*impl_, x, y, to_x, to_y, brush(x, y));
            }
        }
    }
}

void Graphics::fillRectangle(Rect rect, const Color& color)
{
    fillRectangle(rect, ColorBrush(color));
}

void Graphics::fillRectangle(Point from, Point to, const Color& color)
{
    fillRectangle(from, to, ColorBrush(color));
}

void Graphics::fillRectangle(Point point, Size size, const Color& color)
{
    fillRectangle(point, size, ColorBrush(color));
}

void Graphics::fillRectangle(int from_x, int from_y, int to_x, int to_y,
                             const Color& color)
{
    fillRectangle(from_x, from_y, to_x, to_y, ColorBrush(color));
}

void Graphics::fillRectangle(Rect rect, const Brush& brush)
{
    rect = rect.normalize();

    fillRectangle(rect.left, rect.top, rect.right, rect.bottom, brush);
}

void Graphics::fillRectangle(Point from, Point to, const Brush& brush)
{
    fillRectangle(Rect(from, to), brush);
}

void Graphics::fillRectangle(Point point, Size size, const Brush& brush)
{
    fillRectangle(Rect(point, size), brush);
}

void Graphics::fillRectangle
    (int from_x, int from_y, int to_x, int to_y, const Brush& brush)
{
    if (from_x > to_x)
        from_x ^= to_x ^= from_x ^= to_x;
    if (from_y > to_y)
        from_y ^= to_y ^= from_y ^= to_y;

    for (int y = from_y; y < to_y; ++y)
    for (int x = from_x; x < to_x; ++x)
        impl_->pixel(x, y, brush(x, y));
}

void Graphics::fillRectangle(RectF rect, const Brush& brush)
{
    fillRectangle(rect.left, rect.top, rect.right, rect.bottom, brush);
}

void Graphics::fillRectangle(PointF from, PointF to, const Brush& brush)
{
    fillRectangle(from.x, from.y, to.x, to.y, brush);
}

void Graphics::fillRectangle(PointF point, SizeF size, const Brush& brush)
{
    fillRectangle(point.x, point.x + size.width,
                  point.y, point.y + size.height, brush);
}

void Graphics::fillRectangle(double from_x, double from_y,
                             double to_x, double to_y, const Brush& brush)
{
    fillPath(rectangle().build(from_x, from_y, to_x, to_y), brush);
}

void Graphics::fillRoundRectangle(RectF rect, double radius, const Color& color)
{
    fillRoundRectangle(rect, radius, ColorBrush(color));
}

void Graphics::fillRoundRectangle(PointF from, PointF to, double radius,
                                  const Color& color)
{
    fillRoundRectangle(from, to, radius, ColorBrush(color));
}

void Graphics::fillRoundRectangle(PointF point, SizeF size, double radius,
                                  const Color& color)
{
    fillRoundRectangle(point, size, radius, ColorBrush(color));
}

void Graphics::fillRoundRectangle(double from_x, double from_y,
                                  double to_x, double to_y, double radius,
                                  const Color& color)
{
    fillRoundRectangle(from_x, from_y, to_x, to_y, radius, ColorBrush(color));
}

void Graphics::fillRoundRectangle(RectF rect, double width, double height,
                                  const Color& color)
{
    fillRoundRectangle(rect, width, height, ColorBrush(color));
}

void Graphics::fillRoundRectangle(PointF from, PointF to,
                                  double width, double height,
                                  const Color& color)
{
    fillRoundRectangle(from, to, width, height, ColorBrush(color));
}

void Graphics::fillRoundRectangle(PointF point, SizeF size,
                                  double width, double height,
                                  const Color& color)
{
    fillRoundRectangle(point, size, width, height, ColorBrush(color));
}

void Graphics::fillRoundRectangle(double from_x, double from_y,
                                  double to_x, double to_y,
                                  double width, double height,
                                  const Color& color)
{
    fillRoundRectangle(from_x, from_y, to_x, to_y, width, height,
                       ColorBrush(color));
}

void Graphics::fillRoundRectangle(RectF rect, double radius, const Brush& brush)
{
    fillRoundRectangle(rect, radius, radius, brush);
}

void Graphics::fillRoundRectangle(PointF from, PointF to, double radius,
                                  const Brush& brush)
{
    fillRoundRectangle(from, to, radius, radius, brush);
}

void Graphics::fillRoundRectangle(PointF point, SizeF size, double radius,
                                  const Brush& brush)
{
    fillRoundRectangle(point, size, radius, radius, brush);
}

void Graphics::fillRoundRectangle(double from_x, double from_y,
                                  double to_x, double to_y, double radius,
                                  const Brush& brush)
{
    fillRoundRectangle(from_x, from_y, to_x, to_y, radius, radius, brush);
}

void Graphics::fillRoundRectangle(RectF rect, double width, double height,
                                  const Brush& brush)
{
    fillRoundRectangle(rect.left, rect.top, rect.right, rect.bottom,
                       width, height, brush);
}

void Graphics::fillRoundRectangle(PointF from, PointF to,
                                  double width, double height,
                                  const Brush& brush)
{
    fillRoundRectangle(from.x, from.y, to.x, to.y, width, height, brush);
}

void Graphics::fillRoundRectangle(PointF point, SizeF size,
                                  double width, double height,
                                  const Brush& brush)
{
    fillRoundRectangle(point.x, point.y,
                       point.x + size.width, point.y + size.height,
                       width, height, brush);
}

void Graphics::fillRoundRectangle(double from_x, double from_y,
                                  double to_x, double to_y,
                                  double width, double height,
                                  const Brush& brush)
{
    fillPath(rectangle().build(from_x, from_y, to_x, to_y, width, height),
             brush);
}

void Graphics::fillEllipse(PointF center, double width, double height,
                           const Color& color)
{
    fillEllipse(center, width, height, ColorBrush(color));
}

void Graphics::fillEllipse(double x, double y, double width, double height,
                           const Color& color)
{
    fillEllipse(x, y, width, height, ColorBrush(color));
}

void Graphics::fillEllipse(PointF center, double width, double height,
                           const Brush& brush)
{
    fillEllipse(center.x, center.y, width, height, brush);
}

void Graphics::fillEllipse(double x, double y, double width, double height,
                           const Brush& brush)
{
    fillPath(ellipse(x, y, width, height).build(), brush);
}

void Graphics::fillCircle(PointF center, double radius, const Color& color)
{
    fillCircle(center, radius, ColorBrush(color));
}

void Graphics::fillCircle(double x, double y, double radius, const Color& color)
{
    fillCircle(x, y, radius, ColorBrush(color));
}

void Graphics::fillCircle(PointF center, double radius, const Brush& brush)
{
    fillCircle(center.x, center.y, radius, brush);
}

void Graphics::fillCircle(double x, double y, double radius, const Brush& brush)
{
    fillPath(circle(x, y, radius).build(), brush);
}

void Graphics::fillPath(const Path& path, const Color& color)
{
    fillPath(path, ColorBrush(color));
}

void Graphics::fillPath(const Paths& paths, const Color& color)
{
    fillPath(paths, ColorBrush(color));
}

void Graphics::fillPath(const Path& path, const Brush& brush)
{
    impl_->renderPath(path, brush);
}

void Graphics::fillPath(const Paths& paths, const Brush& brush)
{
    impl_->renderPath(paths, brush);
}

void Graphics::endpaint(void)
{
    endpaint(false);
}

void Graphics::endpaint(bool premultiply)
{
    if (premultiply)
        impl_->premultiply();
    impl_->paint();
}
