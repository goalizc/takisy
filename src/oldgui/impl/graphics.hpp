#ifndef graphics_implement_hpp_20140120
#define graphics_implement_hpp_20140120

#include <takisy/cgl/basic/raster.h>
#include <takisy/cgl/basic/canvas.h>
#include <takisy/oldgui/basic/graphics.h>

class Graphics::Implement
{
    friend class Window;
    friend class Graphics;

    typedef canvas_bgra8 canvas_type;

public:
    Implement(HWND hwnd, Rect rect)
        : hwnd_(hwnd), hdc_(nullptr), canvas_(nullptr)
    {
        if (hwnd_)
            hdc_ = GetDC(hwnd_);

        rect_         = rect = rect.normalize();
        paint_offset_ = -rect.left_top();
        paint_rect_   = rect;

        canvas_.reset(new canvas_type(rect.width(), rect.height()));
    }

    ~Implement(void)
    {
        paint();
    }

public:
    inline void paintArea(Point offset, Rect rect)
    {
        paint_offset_ += offset;
        paint_rect_    = rect.normalize();
    }

    inline Color pixel(int x, int y)
    {
        if (valid_xy(x, y))
            return canvas_->pixel(x + paint_offset_.x, y + paint_offset_.y);
        else
            return Color::black(0);
    }

    inline void pixel(int x, int y, const Color& color)
    {
        if (valid_xy(x, y))
            canvas_->pixel(x + paint_offset_.x, y + paint_offset_.y, color);
    }

    inline void pixel(int x, int y, const Color& color, unsigned char coverage)
    {
        if (valid_xy(x, y))
            canvas_->pixel(x + paint_offset_.x,
                           y + paint_offset_.y, color, coverage);
    }

    template <typename Path>
    void renderPath(const Path& path, const Brush& brush)
    {
        raster raster;
        raster.rasterize(path);
        rect bbox = raster.bbox();

        if (bbox.top    < paint_rect_.top)
            bbox.top    = paint_rect_.top;
        if (bbox.bottom > paint_rect_.bottom)
            bbox.bottom = paint_rect_.bottom;

        for (int y = bbox.top; y < bbox.bottom; ++y)
        for (const raster::span& span : raster.fetch_scanline(y))
        for (int x = span.x; x < span.x + int(span.length); ++x)
            pixel(x, y, brush(x, y), span.coverage);
    }

    void premultiply(void)
    {
        typedef canvas_type::pixel_format pixel_format;

        for (pixel_format& pixel : canvas_->pixels())
        {
            unsigned int inc_alpha = pixel.a() + 1;

            pixel.r((pixel.r() * inc_alpha) >> pixel_format::channel_bits());
            pixel.g((pixel.g() * inc_alpha) >> pixel_format::channel_bits());
            pixel.b((pixel.b() * inc_alpha) >> pixel_format::channel_bits());
        }
    }

    inline void paint(void)
    {
        if (hdc_)
        {
            BITMAPINFO bitmap_info = {
                .bmiHeader = BITMAPINFOHEADER {
                    .biSize          = sizeof(BITMAPINFOHEADER),
                    .biWidth         = +static_cast<LONG>(canvas_->width()),
                    .biHeight        = -static_cast<LONG>(canvas_->height()),
                    .biPlanes        = 1,
                    .biBitCount      = 32,
                    .biCompression   = BI_RGB,
                    .biSizeImage     = 0,
                    .biXPelsPerMeter = 0,
                    .biYPelsPerMeter = 0,
                    .biClrUsed       = 0,
                    .biClrImportant  = 0, },
                .bmiColors = {0, },
            };

            SetDIBitsToDevice(
                hdc_, rect_.left, rect_.top, rect_.width(), rect_.height(),
                0, 0, 0, canvas_->height(),
                canvas_->pixels().data(), &bitmap_info, DIB_RGB_COLORS);

            if (hwnd_)
            {
                ReleaseDC(hwnd_, hdc_);
                hwnd_ = nullptr;
            }

            hdc_ = nullptr;
        }
    }

public:
    static inline Rect clientRect(HWND hwnd)
    {
        RECT rect;

        return GetClientRect(hwnd, &rect)
                ? Rect(rect.left, rect.top, rect.right, rect.bottom)
                : Rect(0, 0, 0, 0);
    }

private:
    inline bool valid_xy(int x, int y)
    {
        return paint_rect_.left <= x && x < paint_rect_.right
            && paint_rect_.top  <= y && y < paint_rect_.bottom;
    }

private:
    HWND  hwnd_;
    HDC   hdc_;
    Rect  rect_;
    Point paint_offset_;
    Rect  paint_rect_;
    std::shared_ptr<canvas_type> canvas_;
};

#endif // graphics_implement_hpp_20140120
