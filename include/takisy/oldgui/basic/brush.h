#ifndef brush_h_20140118
#define brush_h_20140118

#include <memory>
#include <takisy/oldgui/basic/typedef.h>

class Brush
{
public:
    virtual Color operator()(int x, int y) const = 0;
};

/* ===============================================================
 *
 *             Color Brush
 *
 * ============================================================ */

class ColorBrush : public Brush
{
public:
    inline ColorBrush(const Color& color)
        : color_(color)
    {}

public:
    inline Color operator()(int, int) const override
    {
        return color_;
    }

private:
    Color color_;
};

inline ColorBrush makeColorBrush(Color color)
{
    return ColorBrush(color);
}

inline std::shared_ptr<Brush> makeColorBrushSP(Color color)
{
    return std::shared_ptr<Brush>(new ColorBrush(color));
}

/* ===============================================================
 *
 *             Lambda Brush
 *
 * ============================================================ */

template <typename Lambda>
class LambdaBrush : public Brush
{
public:
    inline LambdaBrush(Lambda lambda)
        : lambda_(lambda)
    {}

public:
    inline Color operator()(int x, int y) const override
    {
        return lambda_(x, y);
    }

private:
    Lambda lambda_;
};

template <typename Lambda>
inline LambdaBrush<Lambda> makeLambdaBrush(Lambda lambda)
{
    return LambdaBrush<Lambda>(lambda);
}

template <typename Lambda>
inline std::shared_ptr<Brush> makeLambdaBrushSP(Lambda lambda)
{
    return std::shared_ptr<Brush>(new LambdaBrush<Lambda>(lambda));
}

/* ===============================================================
 *
 *             Pattern Brush
 *
 * ============================================================ */

template <typename PixelMatrix>
class PatternBrush : public Brush
{
public:
    PatternBrush(const PixelMatrix& pixel_matrix)
        : offset_(0, 0), pixel_matrix_(pixel_matrix)
    {}

    PatternBrush(int offset_x, int offset_y, const PixelMatrix& pixel_matrix)
        : offset_(offset_x, offset_y), pixel_matrix_(pixel_matrix)
    {}

    PatternBrush(const Point& offset, const PixelMatrix& pixel_matrix)
        : offset_(offset), pixel_matrix_(pixel_matrix)
    {}

public:
    inline Color operator()(int x, int y) const override
    {
        register int w = pixel_matrix_.width();
        register int h = pixel_matrix_.height();

        return pixel_matrix_.pixel((w + (x + offset_.x) % w) % w,
                                   (h + (y + offset_.y) % h) % h);
    }

private:
    Point offset_;
    PixelMatrix pixel_matrix_;
};

template <typename PixelMatrix>
inline PatternBrush<PixelMatrix> makePatternBrush(PixelMatrix pixel_matrix)
{
    return PatternBrush<PixelMatrix>(pixel_matrix);
}

template <typename PixelMatrix>
inline PatternBrush<PixelMatrix>
    makePatternBrush(int offset_x, int offset_y, PixelMatrix pixel_matrix)
{
    return PatternBrush<PixelMatrix>(offset_x, offset_y, pixel_matrix);
}

template <typename PixelMatrix>
inline PatternBrush<PixelMatrix>
    makePatternBrush(const Point& offset, PixelMatrix pixel_matrix)
{
    return PatternBrush<PixelMatrix>(offset, pixel_matrix);
}

template <typename PixelMatrix>
inline std::shared_ptr<Brush> makePatternBrushSP(PixelMatrix pixel_matrix)
{
    return std::shared_ptr<Brush>(new PatternBrush<PixelMatrix>(pixel_matrix));
}

template <typename PixelMatrix>
inline std::shared_ptr<Brush>
    makePatternBrushSP(int offset_x, int offset_y, PixelMatrix pixel_matrix)
{
    return std::shared_ptr<Brush>(
            new PatternBrush<PixelMatrix>(offset_x, offset_y, pixel_matrix));
}

template <typename PixelMatrix>
inline std::shared_ptr<Brush>
    makePatternBrushSP(const Point& offset, PixelMatrix pixel_matrix)
{
    return std::shared_ptr<Brush>(
            new PatternBrush<PixelMatrix>(offset, pixel_matrix));
}

/* ===============================================================
 *
 *             Gradient Brush
 *
 * ============================================================ */

class GradientBrush;

#endif // brush_h_20140118
