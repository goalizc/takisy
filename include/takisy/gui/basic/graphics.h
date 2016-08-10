#ifndef graphics_h_20150715
#define graphics_h_20150715

#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/basic/size.h>
#include <takisy/cgl/basic/rect.h>
#include <takisy/cgl/basic/color.h>
#include <takisy/cgl/basic/canvas.h>
#include <takisy/cgl/path/path.h>
#include <takisy/cgl/font/font.h>
#include <takisy/cgl/image/canvas_adapter.h>
#include <takisy/gui/basic/brush.h>
#include <takisy/gui/basic/pen.h>

class graphics
{
    class implement;

public:
    typedef canvas_mask8 mask_type;

public:
    graphics(void);
    graphics(const graphics& graphics);
   ~graphics(void);

    graphics& operator=(const graphics& graphics);

public:
    color pixel(int x, int y) const;
    color pixel(const point& point) const;

public:
    void mask(const mask_type* mask);

    void clear(const color& color);
    void clear(const brush& brush);

    void pixel(int x, int y, const color& color);
    void pixel(int x, int y, const color& color, unsigned char coverage);
    void pixel(const point& point, const color& color);
    void pixel(const point& point, const color& color, unsigned char coverage);
    void pixel(int x, int y, const brush& brush);
    void pixel(int x, int y, const brush& brush, unsigned char coverage);
    void pixel(const point& point, const brush& brush);
    void pixel(const point& point, const brush& brush, unsigned char coverage);

    void draw_line(const point& p1, const point& p2, const color& color);
    void draw_line(int p1x, int p1y, int p2x, int p2y, const color& color);
    void draw_line(const point& p1, const point& p2, const brush& brush);
    void draw_line(int p1x, int p1y, int p2x, int p2y, const brush& brush);
    void draw_line(const pointf& p1, const pointf& p2, const pen& pen);
    void draw_line(double p1x, double p1y, double p2x, double p2y, const pen& pen);

    void draw_rectangle(const rect& rect, const color& color);
    void draw_rectangle(const point& p1, const point& p2, const color& color);
    void draw_rectangle(const point& point, const size& size, const color& color);
    void draw_rectangle(int p1x, int p1y, int p2x, int p2y, const color& color);
    void draw_rectangle(const rect& rect, const brush& brush);
    void draw_rectangle(const point& p1, const point& p2, const brush& brush);
    void draw_rectangle(const point& point, const size& size, const brush& brush);
    void draw_rectangle(int p1x, int p1y, int p2x, int p2y, const brush& brush);
    void draw_rectangle(const rectf& rect, const pen& pen);
    void draw_rectangle(const pointf& p1, const pointf& p2, const pen& pen);
    void draw_rectangle(const pointf& point, const sizef& size, const pen& pen);
    void draw_rectangle(double p1x, double p1y, double p2x, double p2y, const pen& pen);

    void draw_round_rectangle(const rectf& rect, double radius, const pen& pen);
    void draw_round_rectangle(const pointf& p1, const pointf& p2, double radius, const pen& pen);
    void draw_round_rectangle(const pointf& point, const sizef& size, double radius, const pen& pen);
    void draw_round_rectangle(double p1x, double p1y, double p2x, double p2y, double radius, const pen& pen);
    void draw_round_rectangle(const rectf& rect, double width, double height, const pen& pen);
    void draw_round_rectangle(const pointf& p1, const pointf& p2, double width, double height, const pen& pen);
    void draw_round_rectangle(const pointf& point, const sizef& size, double width, double height, const pen& pen);
    void draw_round_rectangle(double p1x, double p1y, double p2x, double p2y, double width, double height, const pen& pen);

    void draw_ellipse(const pointf& center, double width, double height, const pen& pen);
    void draw_ellipse(double x, double y, double width, double height, const pen& pen);

    void draw_circle(const pointf& center, double radius, const pen& pen);
    void draw_circle(double x, double y, double radius, const pen& pen);

    void draw_path(const path& path, const pen& pen);
    void draw_path(const path& path, bool closed, const pen& pen);
    void draw_path(const paths& paths, const pen& pen);
    void draw_path(const paths& paths, bool closed, const pen& pen);

    void draw_text(const point& point, const char* string, const char* codec, const font& font, const color& color);
    void draw_text(const rect& rect, const char* string, const char* codec, const font& font, const color& color);
    void draw_text(const point& p1, const point& p2, const char* string, const char* codec, const font& font, const color& color);
    void draw_text(const point& point, const size& size, const char* string, const char* codec, const font& font, const color& color);
    void draw_text(int x, int y, const char* string, const char* codec, const font& font, const color& color);
    void draw_text(int p1x, int p1y, int p2x, int p2y, const char* string, const char* codec, const font& font, const color& color);
    void draw_text(const point& point, const char* string, const char* codec, const font& font, const brush& brush);
    void draw_text(const rect& rect, const char* string, const char* codec, const font& font, const brush& brush);
    void draw_text(const point& p1, const point& p2, const char* string, const char* codec, const font& font, const brush& brush);
    void draw_text(const point& point, const size& size, const char* string, const char* codec, const font& font, const brush& brush);
    void draw_text(int x, int y, const char* string, const char* codec, const font& font, const brush& brush);
    void draw_text(int p1x, int p1y, int p2x, int p2y, const char* string, const char* codec, const font& font, const brush& brush);
    void draw_text(const point& point, const char* string, const font& font, const color& color);
    void draw_text(const rect& rect, const char* string, const font& font, const color& color);
    void draw_text(const point& p1, const point& p2, const char* string, const font& font, const color& color);
    void draw_text(const point& point, const size& size, const char* string, const font& font, const color& color);
    void draw_text(int x, int y, const char* string, const font& font, const color& color);
    void draw_text(int p1x, int p1y, int p2x, int p2y, const char* string, const font& font, const color& color);
    void draw_text(const point& point, const char* string, const font& font, const brush& brush);
    void draw_text(const rect& rect, const char* string, const font& font, const brush& brush);
    void draw_text(const point& p1, const point& p2, const char* string, const font& font, const brush& brush);
    void draw_text(const point& point, const size& size, const char* string, const font& font, const brush& brush);
    void draw_text(int x, int y, const char* string, const font& font, const brush& brush);
    void draw_text(int p1x, int p1y, int p2x, int p2y, const char* string, const font& font, const brush& brush);
    void draw_text(const point& point, const wchar_t* string, const font& font, const color& color);
    void draw_text(const rect& rect, const wchar_t* string, const font& font, const color& color);
    void draw_text(const point& p1, const point& p2, const wchar_t* string, const font& font, const color& color);
    void draw_text(const point& point, const size& size, const wchar_t* string, const font& font, const color& color);
    void draw_text(int x, int y, const wchar_t* string, const font& font, const color& color);
    void draw_text(int p1x, int p1y, int p2x, int p2y, const wchar_t* string, const font& font, const color& color);
    void draw_text(const point& point, const wchar_t* string, const font& font, const brush& brush);
    void draw_text(const rect& rect, const wchar_t* string, const font& font, const brush& brush);
    void draw_text(const point& p1, const point& p2, const wchar_t* string, const font& font, const brush& brush);
    void draw_text(const point& point, const size& size, const wchar_t* string, const font& font, const brush& brush);
    void draw_text(int x, int y, const wchar_t* string, const font& font, const brush& brush);
    void draw_text(int p1x, int p1y, int p2x, int p2y, const wchar_t* string, const font& font, const brush& brush);

    void draw_image(const canvas_adapter& canvas);
    void draw_image(const canvas_adapter& canvas, int x, int y, unsigned int width, unsigned int height);
    void draw_image(const canvas_adapter& canvas, const point& p1, const point& p2);
    void draw_image(const canvas_adapter& canvas, const point& point, const size& size);
    void draw_image(const canvas_adapter& canvas, const rect& rect);
    void draw_image(int x, int y, const canvas_adapter& canvas);
    void draw_image(int x, int y, const canvas_adapter& canvas, int cx, int cy, unsigned int width, unsigned int height);
    void draw_image(int x, int y, const canvas_adapter& canvas, const point& p1, const point& p2);
    void draw_image(int x, int y, const canvas_adapter& canvas, const point& point, const size& size);
    void draw_image(int x, int y, const canvas_adapter& canvas, const rect& rect);
    void draw_image(const point& xy, const canvas_adapter& canvas);
    void draw_image(const point& xy, const canvas_adapter& canvas, int x, int y, unsigned int width, unsigned int height);
    void draw_image(const point& xy, const canvas_adapter& canvas, const point& p1, const point& p2);
    void draw_image(const point& xy, const canvas_adapter& canvas, const point& point, const size& size);
    void draw_image(const point& xy, const canvas_adapter& canvas, const rect& rect);

    void fill_rectangle(const rect& rect, const color& color);
    void fill_rectangle(const point& p1, const point& p2, const color& color);
    void fill_rectangle(const point& point, const size& size, const color& color);
    void fill_rectangle(int p1x, int p1y, int p2x, int p2y, const color& color);
    void fill_rectangle(const rect& rect, const brush& brush);
    void fill_rectangle(const point& p1, const point& p2, const brush& brush);
    void fill_rectangle(const point& point, const size& size, const brush& brush);
    void fill_rectangle(int p1x, int p1y, int p2x, int p2y, const brush& brush);
    void fill_rectangle(const rectf& rect, const brush& brush);
    void fill_rectangle(const pointf& p1, const pointf& p2, const brush& brush);
    void fill_rectangle(const pointf& point, const sizef& size, const brush& brush);
    void fill_rectangle(double p1x, double p1y, double p2x, double p2y, const brush& brush);

    void fill_round_rectangle(const rectf& rect, double radius, const color& color);
    void fill_round_rectangle(const pointf& p1, const pointf& p2, double radius, const color& color);
    void fill_round_rectangle(const pointf& point, const sizef& size, double radius, const color& color);
    void fill_round_rectangle(double p1x, double p1y, double p2x, double p2y, double radius, const color& color);
    void fill_round_rectangle(const rectf& rect, double width, double height, const color& color);
    void fill_round_rectangle(const pointf& p1, const pointf& p2, double width, double height, const color& color);
    void fill_round_rectangle(const pointf& point, const sizef& size, double width, double height, const color& color);
    void fill_round_rectangle(double p1x, double p1y, double p2x, double p2y, double width, double height, const color& color);
    void fill_round_rectangle(const rectf& rect, double radius, const brush& brush);
    void fill_round_rectangle(const pointf& p1, const pointf& p2, double radius, const brush& brush);
    void fill_round_rectangle(const pointf& point, const sizef& size, double radius, const brush& brush);
    void fill_round_rectangle(double p1x, double p1y, double p2x, double p2y, double radius, const brush& brush);
    void fill_round_rectangle(const rectf& rect, double width, double height, const brush& brush);
    void fill_round_rectangle(const pointf& p1, const pointf& p2, double width, double height, const brush& brush);
    void fill_round_rectangle(const pointf& point, const sizef& size, double width, double height, const brush& brush);
    void fill_round_rectangle(double p1x, double p1y, double p2x, double p2y, double width, double height, const brush& brush);

    void fill_ellipse(const pointf& center, double width, double height, const color& color);
    void fill_ellipse(double x, double y, double width, double height, const color& color);
    void fill_ellipse(const pointf& center, double width, double height, const brush& brush);
    void fill_ellipse(double x, double y, double width, double height, const brush& brush);

    void fill_circle(const pointf& center, double radius, const color& color);
    void fill_circle(double x, double y, double radius, const color& color);
    void fill_circle(const pointf& center, double radius, const brush& brush);
    void fill_circle(double x, double y, double radius, const brush& brush);

    void fill_path(const path& path, const color& color);
    void fill_path(const paths& paths, const color& color);
    void fill_path(const path& path, const brush& brush);
    void fill_path(const paths& paths, const brush& brush);

public:
    class implement* impl_;
};

#endif // graphics_h_20150715
