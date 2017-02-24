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

template <typename Brush, typename... ConstructorArgs>
inline Brush make_brush(ConstructorArgs&&... args)
{
    return Brush(std::forward<ConstructorArgs>(args)...);
}

template <template <class...> class TemplateBrush, typename... TemplateArgs>
inline TemplateBrush<TemplateArgs...> make_brush(void)
{
    return TemplateBrush<TemplateArgs...>();
}

template <template <class...> class TemplateBrush, typename... TemplateArgs>
inline TemplateBrush<TemplateArgs...> make_brush(TemplateArgs&&... args)
{
    typedef TemplateBrush<TemplateArgs...> brush_type;

    return brush_type(std::forward<TemplateArgs>(args)...);
}

/// make brush pointer

template <typename Brush, typename... ConstructorArgs>
inline Brush* make_brushptr(ConstructorArgs&&... args)
{
    return new Brush(std::forward<ConstructorArgs>(args)...);
}

template <template <class...> class TemplateBrush, typename... TemplateArgs>
inline TemplateBrush<TemplateArgs...>* make_brushptr(void)
{
    return new TemplateBrush<TemplateArgs...>();
}

template <template <class...> class TemplateBrush, typename... TemplateArgs>
inline TemplateBrush<TemplateArgs...>* make_brushptr(TemplateArgs&&... args)
{
    typedef TemplateBrush<TemplateArgs...> brush_type;

    return new brush_type(std::forward<TemplateArgs>(args)...);
}

/// make brush shared pointer

template <typename Brush, typename... ConstructorArgs>
inline brush_sptr make_brushsptr(ConstructorArgs&&... args)
{
    return brush_sptr(new Brush(std::forward<ConstructorArgs>(args)...));
}

template <template <class...> class TemplateBrush, typename... TemplateArgs>
inline brush_sptr make_brushsptr(void)
{
    return brush_sptr(new TemplateBrush<TemplateArgs...>());
}

template <template <class...> class TemplateBrush, typename... TemplateArgs>
inline brush_sptr make_brushsptr(TemplateArgs&&... args)
{
    typedef TemplateBrush<TemplateArgs...> brush_type;

    return brush_sptr(new brush_type(std::forward<TemplateArgs>(args)...));
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
    static inline void set(brush& brush, const point& offset)
    {
        set(&brush, offset);
    }

    static inline void set(brush* brushptr, const point& offset)
    {
        offset_brush* obptr = dynamic_cast<offset_brush*>(brushptr);
        if (obptr)
            obptr->offset = offset;
    }

    static inline void set(const brush_sptr& brushsptr, const point& offset)
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

/* ===============================================================
 *
 *  lambda brush
 *
 * ============================================================ */

template <typename Lambda>
class lambda_brush : public offset_brush
{
public:
    inline lambda_brush(Lambda lambda)
        : lambda_(lambda)
    {}

public:
    inline color operator()(int x, int y) const override
    {
        return lambda_(offset.x + x, offset.y + y);
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
    inline color operator()(int x, int y) const override
    {
        int width = pixel_matrix_.width();
        x = (offset.x + x) % width;
        if (x < 0)
            x += width;

        int height = pixel_matrix_.height();
        y = (offset.y + y) % height;
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

template <typename Calculate,
          typename Decorate  = decorate::null,
          typename Transform = transform::null>
struct gradient_brush : public offset_brush
{
    typedef gradient<Calculate, Decorate, Transform> gradient_type;

    gradient_type gradient;
    interpolator  r, g, b, a;

public:
    gradient_brush(void)
    {
        clamp();
    }

    gradient_brush(const typename gradient_type::calculate_type& calculate)
        : gradient(calculate)
    {
        clamp();
    }

    gradient_brush(const typename gradient_type::calculate_type& calculate,
                   const typename gradient_type::decorate_type&  decorate,
                   const typename gradient_type::transform_type& transform)
        : gradient(calculate, decorate, transform)
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

    void clamp(void)
    {
        r.clamp(0, 255);
        g.clamp(0, 255);
        b.clamp(0, 255);
        a.clamp(0, 255);
    }

    void add_knot(double gradient, const color& color)
    {
        r.add_knot(gradient, color.r);
        g.add_knot(gradient, color.g);
        b.add_knot(gradient, color.b);
        a.add_knot(gradient, color.a);
    }

public:
    color operator()(int x, int y) const override
    {
        double gvalue = gradient.value(offset.x + x, offset.y + y);

        return color(r[gvalue], g[gvalue], b[gvalue], a[gvalue]);
    }
};

#endif // brush_h_20150715
