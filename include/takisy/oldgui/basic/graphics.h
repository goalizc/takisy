#ifndef graphics_h_20140118
#define graphics_h_20140118

#include <Windows.h>
#include <takisy/oldgui/basic/typedef.h>
#include <takisy/oldgui/basic/pen.h>
#include <takisy/oldgui/basic/brush.h>

class Graphics
{
    friend class Window;
    class Implement;

public:
    Graphics(HWND hwnd);
    Graphics(HWND hwnd, Rect rect);
    Graphics(HWND hwnd, Point left_top, Point right_bottom);
    Graphics(HWND hwnd, Point point, Size size);
    Graphics(HWND hwnd, int x, int y, unsigned int width, unsigned int height);
    Graphics(HDC hdc, Rect rect);
    Graphics(HDC hdc, Point left_top, Point right_bottom);
    Graphics(HDC hdc, Point point, Size size);
    Graphics(HDC hdc, int x, int y, unsigned int width, unsigned int height);
    Graphics(const PAINTSTRUCT& ps);
    Graphics(const Graphics& graphics);
   ~Graphics(void);

    Graphics& operator=(const Graphics& graphics);

public:
    Rect paintRect(void) const;

public:
    void  clear(Color color);
    void  clear(const Brush& brush);

    Color pixel(Point point) const;
    void  pixel(Point point, Color color);
    void  pixel(Point point, Color color, unsigned int coverage);
    void  pixel(Point point, const Brush& brush);
    void  pixel(Point point, const Brush& brush, unsigned int coverage);
    Color pixel(int x, int y) const;
    void  pixel(int x, int y, Color color);
    void  pixel(int x, int y, Color color, unsigned int coverage);
    void  pixel(int x, int y, const Brush& brush);
    void  pixel(int x, int y, const Brush& brush, unsigned int coverage);

public:
    void drawLine(Point from, Point to, const Color& color);
    void drawLine(int from_x, int from_y, int to_x, int to_y, const Color& color);
    void drawLine(Point from, Point to, const Brush& brush);
    void drawLine(int from_x, int from_y, int to_x, int to_y, const Brush& brush);
    void drawLine(PointF from, PointF to, const Pen& pen);
    void drawLine(double from_x, double from_y, double to_x, double to_y, const Pen& pen);

    void drawRectangle(Rect rect, const Color& color);
    void drawRectangle(Point from, Point to, const Color& color);
    void drawRectangle(Point point, Size size, const Color& color);
    void drawRectangle(int from_x, int from_y, int to_x, int to_y, const Color& color);
    void drawRectangle(Rect rect, const Brush& brush);
    void drawRectangle(Point from, Point to, const Brush& brush);
    void drawRectangle(Point point, Size size, const Brush& brush);
    void drawRectangle(int from_x, int from_y, int to_x, int to_y, const Brush& brush);
    void drawRectangle(RectF rect, const Pen& pen);
    void drawRectangle(PointF from, PointF to, const Pen& pen);
    void drawRectangle(PointF point, SizeF size, const Pen& pen);
    void drawRectangle(double from_x, double from_y, double to_x, double to_y, const Pen& pen);

    void drawRoundRectangle(RectF rect, double radius, const Pen& pen);
    void drawRoundRectangle(PointF from, PointF to, double radius, const Pen& pen);
    void drawRoundRectangle(PointF point, SizeF size, double radius, const Pen& pen);
    void drawRoundRectangle(double from_x, double from_y, double to_x, double to_y, double radius, const Pen& pen);
    void drawRoundRectangle(RectF rect, double width, double height, const Pen& pen);
    void drawRoundRectangle(PointF from, PointF to, double width, double height, const Pen& pen);
    void drawRoundRectangle(PointF point, SizeF size, double width, double height, const Pen& pen);
    void drawRoundRectangle(double from_x, double from_y, double to_x, double to_y, double width, double height, const Pen& pen);

    void drawEllipse(PointF center, double width, double height, const Pen& pen);
    void drawEllipse(double x, double y, double width, double height, const Pen& pen);

    void drawCircle(PointF center, double radius, const Pen& pen);
    void drawCircle(double x, double y, double radius, const Pen& pen);

    void drawPath(const Path& path, const Pen& pen);
    void drawPath(const Path& path, bool closed, const Pen& pen);
    void drawPath(const Paths& paths, const Pen& pen);
    void drawPath(const Paths& paths, bool closed, const Pen& pen);

    void drawText(Point point, const wchar_t* string, const Font& font, const Color& color);
    void drawText(Rect rect, const wchar_t* string, const Font& font, const Color& color);
    void drawText(Point from, Point to, const wchar_t* string, const Font& font, const Color& color);
    void drawText(Point point, Size size, const wchar_t* string, const Font& font, const Color& color);
    void drawText(int x, int y, const wchar_t* string, const Font& font, const Color& color);
    void drawText(int from_x, int from_y, int to_x, int to_y, const wchar_t* string, const Font& font, const Color& color);
    void drawText(Point point, const wchar_t* string, const Font& font, const Brush& brush);
    void drawText(Rect rect, const wchar_t* string, const Font& font, const Brush& brush);
    void drawText(Point from, Point to, const wchar_t* string, const Font& font, const Brush& brush);
    void drawText(Point point, Size size, const wchar_t* string, const Font& font, const Brush& brush);
    void drawText(int x, int y, const wchar_t* string, const Font& font, const Brush& brush);
    void drawText(int from_x, int from_y, int to_x, int to_y, const wchar_t* string, const Font& font, const Brush& brush);

    void fillRectangle(Rect rect, const Color& color);
    void fillRectangle(Point from, Point to, const Color& color);
    void fillRectangle(Point point, Size size, const Color& color);
    void fillRectangle(int from_x, int from_y, int to_x, int to_y, const Color& color);
    void fillRectangle(Rect rect, const Brush& brush);
    void fillRectangle(Point from, Point to, const Brush& brush);
    void fillRectangle(Point point, Size size, const Brush& brush);
    void fillRectangle(int from_x, int from_y, int to_x, int to_y, const Brush& brush);
    void fillRectangle(RectF rect, const Brush& brush);
    void fillRectangle(PointF from, PointF to, const Brush& brush);
    void fillRectangle(PointF point, SizeF size, const Brush& brush);
    void fillRectangle(double from_x, double from_y, double to_x, double to_y, const Brush& brush);

    void fillRoundRectangle(RectF rect, double radius, const Color& color);
    void fillRoundRectangle(PointF from, PointF to, double radius, const Color& color);
    void fillRoundRectangle(PointF point, SizeF size, double radius, const Color& color);
    void fillRoundRectangle(double from_x, double from_y, double to_x, double to_y, double radius, const Color& color);
    void fillRoundRectangle(RectF rect, double width, double height, const Color& color);
    void fillRoundRectangle(PointF from, PointF to, double width, double height, const Color& color);
    void fillRoundRectangle(PointF point, SizeF size, double width, double height, const Color& color);
    void fillRoundRectangle(double from_x, double from_y, double to_x, double to_y, double width, double height, const Color& color);
    void fillRoundRectangle(RectF rect, double radius, const Brush& brush);
    void fillRoundRectangle(PointF from, PointF to, double radius, const Brush& brush);
    void fillRoundRectangle(PointF point, SizeF size, double radius, const Brush& brush);
    void fillRoundRectangle(double from_x, double from_y, double to_x, double to_y, double radius, const Brush& brush);
    void fillRoundRectangle(RectF rect, double width, double height, const Brush& brush);
    void fillRoundRectangle(PointF from, PointF to, double width, double height, const Brush& brush);
    void fillRoundRectangle(PointF point, SizeF size, double width, double height, const Brush& brush);
    void fillRoundRectangle(double from_x, double from_y, double to_x, double to_y, double width, double height, const Brush& brush);

    void fillEllipse(PointF center, double width, double height, const Color& color);
    void fillEllipse(double x, double y, double width, double height, const Color& color);
    void fillEllipse(PointF center, double width, double height, const Brush& brush);
    void fillEllipse(double x, double y, double width, double height, const Brush& brush);

    void fillCircle(PointF center, double radius, const Color& color);
    void fillCircle(double x, double y, double radius, const Color& color);
    void fillCircle(PointF center, double radius, const Brush& brush);
    void fillCircle(double x, double y, double radius, const Brush& brush);

    void fillPath(const Path& path, const Color& color);
    void fillPath(const Paths& paths, const Color& color);
    void fillPath(const Path& path, const Brush& brush);
    void fillPath(const Paths& paths, const Brush& brush);

public:
    void endpaint(void);
    void endpaint(bool premultiply);

private:
    class Implement* impl_;
};

#endif //graphics_h_20140118
