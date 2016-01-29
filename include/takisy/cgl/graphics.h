#ifndef graphics_h_20131115
#define graphics_h_20131115

#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/basic/color.h>
#include <takisy/cgl/basic/canvas.h>

template <typename Canvas>
class graphics
{
public:
    typedef Canvas canvas_type;

public:
    graphics(void);
    graphics(unsigned int width, unsigned int height);
    template <typename PixelMatrix>
    graphics(const PixelMatrix& pixel_matrix);
    graphics(const graphics& graphics);
   ~graphics(void);

    template <typename PixelMatrix>
    graphics& operator=(const PixelMatrix& pixel_matrix);
    graphics& operator=(const graphics& graphics);

public:
    inline void clear(void);
    template <typename Brush>
    inline void clear(const Brush& brush);

    inline void resize(unsigned int width, unsigned int height);

    template <typename GammaFunction>
    inline void gamma(const GammaFunction& gf);

public:
    inline unsigned int width(void) const;
    inline unsigned int height(void) const;

    inline color pixel(const point& point) const;
    inline color pixel(int x, int y) const;

    inline canvas_type& canvas(void);
    inline typename canvas_type::pixel_format* pixels(void);
    inline const typename canvas_type::pixel_format* pixels(void) const;

public:
    template <typename PixelMatrix>
    inline PixelMatrix& clone(PixelMatrix& pixel_matrix) const;
    template <typename PixelMatrix>
    inline PixelMatrix& clone(const point& lt,
                              const point& rb, PixelMatrix& pixel_matrix) const;
    template <typename PixelMatrix>
    inline PixelMatrix& clone(int lt_x, int lt_y,
                              int rb_x, int rb_y,
                              PixelMatrix& pixel_matrix) const;

public:
    template <typename Brush>
    inline void pixel(const point& point, const Brush& brush);
    template <typename Brush>
    inline void pixel(int x, int y, const Brush& brush);

    template <typename Path, typename Brush>
    inline void render(const Path& path, const Brush& brush);

    template <typename PixelMatrix>
    inline void paint(const PixelMatrix& pixel_matrix);
    template <typename PixelMatrix>
    inline void paint(const PixelMatrix& pixel_matrix,
                      const point& lt, const point& rb);
    template <typename PixelMatrix>
    inline void paint(const point& position, const PixelMatrix& pixel_matrix);
    template <typename PixelMatrix>
    inline void paint(const point& position, const PixelMatrix& pixel_matrix,
                      const point& lt, const point& rb);
    template <typename PixelMatrix>
    inline void paint(int x, int y, const PixelMatrix& pixel_matrix);
    template <typename PixelMatrix>
    inline void paint(const PixelMatrix& pixel_matrix,
                      int lt_x, int lt_y, int rb_x, int rb_y);
    template <typename PixelMatrix>
    inline void paint(int x, int y, const PixelMatrix& pixel_matrix,
                      int lt_x, int lt_y, int rb_x, int rb_y);

    template <typename Brush>
    inline void text(const char* string, const Brush& brush);
    template <typename Brush>
    inline void text(const point& lt, const char* string, const Brush& brush);
    template <typename Brush>
    inline void text(const point& lt,
                     const point& rb, const char* string, const Brush& brush);
    template <typename Brush>
    inline void text(int lt_x, int lt_y,
                     const char* string, const Brush& brush);
    template <typename Brush>
    inline void text(int lt_x, int lt_y,
                     int rb_x, int rb_y,
                     const char* string, const Brush& brush);

public:
    inline bool save(const char* filename);

public:
    template <typename DeviceContext>
    inline void play(DeviceContext dc,
                     bool up2down = true, int x = 0, int y = 0,
                     unsigned int width = 0, unsigned int height = 0);

private:
    canvas_type canvas_;
};

#include <takisy/cgl/graphics_implement.hpp>

typedef graphics<canvas_G8>      graphics_G8;
typedef graphics<canvas_Ga8>     graphics_Ga8;
typedef graphics<canvas_aG8>     graphics_aG8;
typedef graphics<canvas_rgb8>    graphics_rgb8;
typedef graphics<canvas_bgr8>    graphics_bgr8;
typedef graphics<canvas_rgba8>   graphics_rgba8;
typedef graphics<canvas_bgra8>   graphics_bgra8;
typedef graphics<canvas_argb8>   graphics_argb8;
typedef graphics<canvas_abgr8>   graphics_abgr8;
typedef graphics<canvas_mask8>   graphics_a8, graphics_mask8;

typedef graphics<canvas_G8be>    graphics_G8be;
typedef graphics<canvas_Ga8be>   graphics_Ga8be;
typedef graphics<canvas_aG8be>   graphics_aG8be;
typedef graphics<canvas_rgb8be>  graphics_rgb8be;
typedef graphics<canvas_bgr8be>  graphics_bgr8be;
typedef graphics<canvas_rgba8be> graphics_rgba8be;
typedef graphics<canvas_bgra8be> graphics_bgra8be;
typedef graphics<canvas_argb8be> graphics_argb8be;
typedef graphics<canvas_abgr8be> graphics_abgr8be;
typedef graphics<canvas_mask8be> graphics_a8be, graphics_mask8be;

typedef graphics<canvas_G8le>    graphics_G8le;
typedef graphics<canvas_Ga8le>   graphics_Ga8le;
typedef graphics<canvas_aG8le>   graphics_aG8le;
typedef graphics<canvas_rgb8le>  graphics_rgb8le;
typedef graphics<canvas_bgr8le>  graphics_bgr8le;
typedef graphics<canvas_rgba8le> graphics_rgba8le;
typedef graphics<canvas_bgra8le> graphics_bgra8le;
typedef graphics<canvas_argb8le> graphics_argb8le;
typedef graphics<canvas_abgr8le> graphics_abgr8le;
typedef graphics<canvas_mask8le> graphics_a8le, graphics_mask8le;

typedef graphics<canvas_G16>      graphics_G16;
typedef graphics<canvas_Ga16>     graphics_Ga16;
typedef graphics<canvas_aG16>     graphics_aG16;
typedef graphics<canvas_rgb16>    graphics_rgb16;
typedef graphics<canvas_bgr16>    graphics_bgr16;
typedef graphics<canvas_rgba16>   graphics_rgba16;
typedef graphics<canvas_bgra16>   graphics_bgra16;
typedef graphics<canvas_argb16>   graphics_argb16;
typedef graphics<canvas_abgr16>   graphics_abgr16;
typedef graphics<canvas_mask16>   graphics_a16, graphics_mask16;

typedef graphics<canvas_G16be>    graphics_G16be;
typedef graphics<canvas_Ga16be>   graphics_Ga16be;
typedef graphics<canvas_aG16be>   graphics_aG16be;
typedef graphics<canvas_rgb16be>  graphics_rgb16be;
typedef graphics<canvas_bgr16be>  graphics_bgr16be;
typedef graphics<canvas_rgba16be> graphics_rgba16be;
typedef graphics<canvas_bgra16be> graphics_bgra16be;
typedef graphics<canvas_argb16be> graphics_argb16be;
typedef graphics<canvas_abgr16be> graphics_abgr16be;
typedef graphics<canvas_mask16be> graphics_a16be, graphics_mask16be;

typedef graphics<canvas_G16le>    graphics_G16le;
typedef graphics<canvas_Ga16le>   graphics_Ga16le;
typedef graphics<canvas_aG16le>   graphics_aG16le;
typedef graphics<canvas_rgb16le>  graphics_rgb16le;
typedef graphics<canvas_bgr16le>  graphics_bgr16le;
typedef graphics<canvas_rgba16le> graphics_rgba16le;
typedef graphics<canvas_bgra16le> graphics_bgra16le;
typedef graphics<canvas_argb16le> graphics_argb16le;
typedef graphics<canvas_abgr16le> graphics_abgr16le;
typedef graphics<canvas_mask16le> graphics_a16le, graphics_mask16le;

#endif //graphics_h_20131115
