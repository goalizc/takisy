#ifndef graphics_implement_hpp_20150715
#define graphics_implement_hpp_20150715

#include <takisy/cgl/basic/raster.h>

struct device_context
{
    typedef std::shared_ptr<device_context> pointer;

    virtual ~device_context(void) {}
    virtual void refresh(const rect& rect, canvas_bgra8& canvas) = 0;
};

class graphics::implement
{
    friend class cross_platform_window;
    friend class graphics;

    typedef canvas_bgra8 canvas_type;
    typedef std::shared_ptr<canvas_type> canvas_sptr;

public:
    implement(void)
        : canvas_(std::make_shared<canvas_type>()), dc_(nullptr)
    {}

    ~implement(void)
    {
        paint();
    }

public:
    inline void initialize(const device_context::pointer& dc, const rect& pr)
    {
        dc_      =  dc;
        prect_   =  rect_ = pr.normalize();
        poffset_ = -prect_.left_top();
        canvas_  =  std::make_shared<canvas_type>(pr.width(), pr.height());
    }

    inline void paint_area(const point& offset, const rect& rect)
    {
        poffset_ += offset;
        prect_    = rect.normalize();
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
        x = x <  prect_.left   ? prect_.left
          : x >= prect_.right  ? prect_.right  - 1 : x;
        y = y <  prect_.top    ? prect_.top
          : y >= prect_.bottom ? prect_.bottom - 1 : y;

        return canvas_->unsafe_pixel(x + poffset_.x, y + poffset_.y);
    }

public:
    inline void unsafe_pixel(int x, int y, const color& color)
    {
        canvas_->unsafe_pixel(x + poffset_.x, y + poffset_.y).blend(color);
    }

    inline void pixel(int x, int y, const color& color)
    {
        if (prect_.inside(x, y))
            unsafe_pixel(x, y, color);
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
        {
            int begin = span.x, end = span.x + span.length;

            if (begin < prect_.left)  begin = prect_.left;
            if (end   > prect_.right) end   = prect_.right;

            for (int x = begin; x < end; ++x)
                unsafe_pixel(x, y, brush(x, y) * span.coverage);
        }
    }

private:
    rect  prect_, rect_;
    point poffset_;
    canvas_sptr canvas_;
    device_context::pointer dc_;
};

#endif // graphics_implement_hpp_20150715
