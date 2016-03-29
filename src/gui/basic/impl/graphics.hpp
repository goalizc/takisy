#ifndef graphics_implement_hpp_20150715
#define graphics_implement_hpp_20150715

#include <takisy/core/algorithm.h>
#include <takisy/cgl/basic/raster.h>
#include <takisy/cgl/basic/canvas.h>
#include "../device_context.h"

class graphics::implement
{
    friend class Window;
    friend class cross_platform_window;
    friend class graphics;

    typedef canvas_bgra8 canvas_type;
    typedef std::shared_ptr<canvas_type> canvas_sptr;

public:
    implement(void)
        : mask_(nullptr), canvas_(std::make_shared<canvas_type>()), dc_(nullptr)
    {}

    ~implement(void)
    {
        paint();
    }

public:
    inline void initialize(device_context* dc, rect pr)
    {
        dc_       =  dc;
        rect_     =  pr = pr.normalize();
        poffset_  = -pr.left_top();
        prect_    =  pr;
        mask_     =  nullptr;
        canvas_   =  std::make_shared<canvas_type>(pr.width(), pr.height());
    }

    inline void paint_area(const point& offset, rect rect)
    {
        poffset_ += offset;
        prect_    = rect.normalize();
        mask_     = nullptr;
    }

public:
    inline void paint(void)
    {
        if (dc_)
        {
            dc_ ->refresh(rect_, *canvas_);
            dc_ = nullptr;
        }
    }

public:
    inline const canvas_type::pixel_format& pixel(int x, int y) const
    {
        x = algorithm::clamp(x, prect_.left, prect_.right  - 1);
        y = algorithm::clamp(y, prect_.top,  prect_.bottom - 1);

        return canvas_->unsafe_pixel(x + poffset_.x, y + poffset_.y);
    }

public:
    inline void pixel(int x, int y, color color)
    {
        if (prect_.outside(x, y))
            return;

        if (mask_)
            color *= mask_->pixel(x, y).a();

        canvas_->pixel(x + poffset_.x, y + poffset_.y, color);
    }

    inline void pixel(int x, int y, color color, unsigned char coverage)
    {
        return pixel(x, y, color * coverage);
    }

    template <typename Path>
    void render(const Path& path, const brush& brush)
    {
        raster raster;
        raster.rasterize(path);
        rect bbox = raster.bbox();

        if (bbox.top    < prect_.top)    bbox.top    = prect_.top;
        if (bbox.bottom > prect_.bottom) bbox.bottom = prect_.bottom;

        for (int y = bbox.top; y < bbox.bottom; ++y)
        for (const raster::span& span : raster.fetch_scanline(y))
        for (int x = span.x; x < span.x + int(span.length); ++x)
            pixel(x, y, brush(x, y), span.coverage);
    }

private:
    rect  rect_;
    point poffset_;
    rect  prect_;
    const mask_type* mask_;
    canvas_sptr canvas_;
    device_context* dc_;
};

#endif // graphics_implement_hpp_20150715
