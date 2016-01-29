#ifndef brush_h_20150715
#define brush_h_20150715

#include <memory>
#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/basic/color.h>

class brush
{
public:
    virtual ~brush(void) {}
    virtual void  offset(int x, int y) {};
    virtual color operator()(int x, int y) const = 0;
};

typedef std::shared_ptr<class brush> brush_sptr;

/* ===============================================================
 *
 *             brush wrapper
 *
 * ============================================================ */

class brush_wrapper : public brush
{
public:
    inline brush_wrapper(const brush& brush)
        : brush_(brush)
    {}

public:
    inline color operator()(int x, int y) const override
    {
        return brush_(x, y);
    }

private:
    const brush& brush_;
};

inline brush_sptr make_brush_wrapper_sptr(const brush& brush)
{
    return brush_sptr(new brush_wrapper(brush));
}

/* ===============================================================
 *
 *             color brush
 *
 * ============================================================ */

class color_brush : public brush
{
public:
    inline color_brush(const color& color)
        : color_(color)
    {}

public:
    inline color operator()(int, int) const override
    {
        return color_;
    }

private:
    color color_;
};

inline brush_sptr make_color_brush_sptr(color color)
{
    return brush_sptr(new color_brush(color));
}

/* ===============================================================
 *
 *             lambda brush
 *
 * ============================================================ */

template <typename Lambda>
class lambda_brush : public brush
{
public:
    inline lambda_brush(Lambda lambda)
        : offset_(0, 0), lambda_(lambda)
    {}

public:
    inline void offset(int x, int y) override
    {
        offset_.x = x;
        offset_.y = y;
    }

    inline color operator()(int x, int y) const override
    {
        return lambda_(offset_.x + x, offset_.y + y);
    }

private:
    point  offset_;
    Lambda lambda_;
};

template <typename Lambda>
inline lambda_brush<Lambda> make_lambda_brush(Lambda lambda)
{
    return lambda_brush<Lambda>(lambda);
}

template <typename Lambda>
inline brush_sptr make_lambda_brush_sptr(Lambda lambda)
{
    return brush_sptr(new lambda_brush<Lambda>(lambda));
}

/* ===============================================================
 *
 *             pattern brush
 *
 * ============================================================ */

template <typename PixelMatrix>
class pattern_brush : public brush
{
public:
    pattern_brush(const PixelMatrix& pm)
        : pattern_brush(0, 0, pm)
    {}

    pattern_brush(int offset_x, int offset_y, const PixelMatrix& pm)
        : offset_(offset_x, offset_y), pixel_matrix_(pm)
    {}

    pattern_brush(const point& offset, const PixelMatrix& pm)
        : pattern_brush(offset.x, offset.y, pm)
    {}

public:
    inline void offset(int x, int y) override
    {
        offset_.x = x;
        offset_.y = y;
    }

    inline color operator()(int x, int y) const override
    {
        register int w = pixel_matrix_.width();
        register int h = pixel_matrix_.height();

        return pixel_matrix_.pixel(
                    (w + (x + offset_.x) % w) % w,
                    (h + (y + offset_.y) % h) % h
        );
    }

private:
    point offset_;
    PixelMatrix pixel_matrix_;
};

template <typename PixelMatrix>
inline brush_sptr make_pattern_brush_sptr(PixelMatrix pm)
{
    return brush_sptr(new pattern_brush<PixelMatrix>(pm));
}

template <typename PixelMatrix>
inline brush_sptr make_pattern_brush_sptr(int offset_x, int offset_y,
                                          PixelMatrix pm)
{
    return brush_sptr(new pattern_brush<PixelMatrix>(offset_x, offset_y, pm));
}

template <typename PixelMatrix>
inline brush_sptr make_pattern_brush_sptr(const point& offset, PixelMatrix pm)
{
    return brush_sptr(new pattern_brush<PixelMatrix>(offset, pm));
}

/* ===============================================================
 *
 *             gradient brush
 *
 * ============================================================ */

class gradient_brush;

#endif // brush_h_20150715
