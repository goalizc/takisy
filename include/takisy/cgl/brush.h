#ifndef brush_h_20150715
#define brush_h_20150715

#include <memory>
#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/basic/color.h>
#include <takisy/cgl/component/gradient.h>
#include <takisy/cgl/component/interpolator.h>

class brush
{
public:
    virtual ~brush(void) {}
    virtual color operator()(int x, int y) const = 0;
};

typedef std::shared_ptr<class brush> brush_sptr;

/* ===============================================================
 *
 *  function for make brush
 *
 * ============================================================ */

/// make object

template <typename Brush, typename... Args>
inline Brush make_brush(Args&&... args)
{
    return Brush(std::forward<Args>(args)...);
}

template <template <class...> class Brush, typename... Args>
inline Brush<Args...> make_brush(void)
{
    return Brush<Args...>();
}

template <template <class...> class Brush, typename... Args>
inline Brush<Args...> make_brush(Args&&... args)
{
    return Brush<Args...>(std::forward<Args>(args)...);
}

/// make brush pointer

template <typename Brush, typename... Args>
inline Brush* make_pbrush(Args&&... args)
{
    return new Brush(std::forward<Args>(args)...);
}

template <template <class...> class Brush, typename... Args>
inline Brush<Args...>* make_pbrush(void)
{
    return new Brush<Args...>();
}

template <template <class...> class Brush, typename... Args>
inline Brush<Args...>* make_pbrush(Args&&... args)
{
    return new Brush<Args...>(std::forward<Args>(args)...);
}

/// make brush shared pointer

template <typename Brush, typename... Args>
inline brush_sptr make_spbrush(Args&&... args)
{
    return brush_sptr(new Brush(std::forward<Args>(args)...));
}

template <template <class...> class Brush, typename... Args>
inline brush_sptr make_spbrush(void)
{
    return brush_sptr(new Brush<Args...>());
}

template <template <class...> class Brush, typename... Args>
inline brush_sptr make_spbrush(Args&&... args)
{
    return brush_sptr(new Brush<Args...>(std::forward<Args>(args)...));
}

/* ===============================================================
 *
 *  offset brush
 *
 * ============================================================ */

struct offset_brush : public brush
{
    point offset;

public:
    static void set(brush& brush, const point& offset)
    {
        set(&brush, offset);
    }

    static void set(brush* brushptr, const point& offset)
    {
        offset_brush* obptr = dynamic_cast<offset_brush*>(brushptr);

        if (obptr)
            obptr->offset = offset;
    }

    static void set(const brush_sptr& brushsptr, const point& offset)
    {
        set(brushsptr.get(), offset);
    }
};

/* ===============================================================
 *
 *  color brush
 *
 * ============================================================ */

class color_brush : public brush
{
public:
    color_brush(const color& c)
        : color_(c)
    {}

public:
    color operator()(int, int) const override
    {
        return color_;
    }

private:
    color color_;
};

/* ===============================================================
 *
 *  lambda brush
 *
 * ============================================================ */

template <typename Lambda>
class lambda_brush : public offset_brush
{
public:
    lambda_brush(Lambda lambda)
        : lambda_(lambda)
    {}

public:
    color operator()(int x, int y) const override
    {
        return lambda_(x + offset.x, y + offset.y);
    }

private:
    Lambda lambda_;
};

/* ===============================================================
 *
 *  pattern brush
 *
 * ============================================================ */

template <typename PixelMatrix>
class pattern_brush : public offset_brush
{
public:
    pattern_brush(const PixelMatrix& pixel_matrix)
        : pixel_matrix_(pixel_matrix)
    {}

public:
    color operator()(int x, int y) const override
    {
        int width = pixel_matrix_.width();
        x = (x + offset.x) % width;
        if (x < 0)
            x += width;

        int height = pixel_matrix_.height();
        y = (y + offset.y) % height;
        if (y < 0)
            y += height;

        return pixel_matrix_.pixel(x, y);
    }

private:
    const PixelMatrix& pixel_matrix_;
};

/* ===============================================================
 *
 *  gradient brush
 *
 * ============================================================ */

template <typename C,
          typename T = transform::null,
          typename D = decorate::null>
struct gradient_brush : public offset_brush
{
    gradient<C, T, D>
        G;
    interpolator
        r, g, b, a;

public:
    gradient_brush(void)
    {
        clamp();
    }

    template <typename... Args>
    gradient_brush(Args&&... args)
        : G(args...)
    {
        clamp();
    }

public:
    void clear(void)
    {
        r.clear();
        g.clear();
        b.clear();
        a.clear();
    }

    void add_knot(double x, const color& c)
    {
        r.add_knot(x, c.r);
        g.add_knot(x, c.g);
        b.add_knot(x, c.b);
        a.add_knot(x, c.a);
    }

public:
    color operator()(int x, int y) const override
    {
        double ix = G.value(x + offset.x, y + offset.y);

        return color(r[ix], g[ix], b[ix], a[ix]);
    }

private:
    void clamp(void)
    {
        r.clamp(0, 255);
        g.clamp(0, 255);
        b.clamp(0, 255);
        a.clamp(0, 255);
    }
};

#endif // brush_h_20150715
