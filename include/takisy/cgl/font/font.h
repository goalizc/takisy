#ifndef basic_h_20150102
#define basic_h_20150102

struct bitmap
{
public:
    enum PixelMode { pmMono = 1, pmGray, pmGray2, pmGray4 };
    typedef unsigned char (*Gray)(const unsigned char* buffer, unsigned int x);

public:
    int left, top;
    unsigned int width, height;
    unsigned int advance;
    int pitch;
    unsigned char pixel_mode;
    unsigned char* buffer;

public:
    static Gray get_gray(unsigned char pixel_mode);

public:
    template <typename Canvas, typename Brush>
    inline int render(Canvas& canvas, int x, int y, const Brush& brush) const
    {
        return render(canvas, x, y, ~0u >> 1, ~0u >> 1, brush);
    }

    template <typename Canvas, typename Brush>
    inline int render(Canvas& canvas, int x, int y, int right, int bottom,
                      const Brush& brush) const
    {
        return render(canvas, x, y, x, y, right, bottom, brush);
    }

    template <typename Canvas, typename Brush>
    int render(Canvas& canvas, int x, int y, int l, int t, int r, int b,
               const Brush& brush) const
    {
        bitmap::Gray gray = bitmap::get_gray(pixel_mode);

        for (int i = 0, Y = y - top + i; i < int(height); ++Y, ++i) {
            if (Y < t) continue; if (Y >= b) break;
            for (int j = 0, X = x + left + j; j < int(width); ++X, ++j) {
                if (X < l) continue; if (X >= r) break;
                unsigned char alpha = gray(&buffer[i * pitch], j);
                if (alpha) canvas.pixel(X, Y, brush, alpha);
            }
        }

        return x + advance;
    }
};

class font
{
public:
    virtual ~font();

public:
    virtual double height(void) const = 0;
    virtual double emheight(void) const = 0;
    virtual const bitmap* get_bitmap(unsigned int char_code) const = 0;
};

#endif // basic_h_20150102
