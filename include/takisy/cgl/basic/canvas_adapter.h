#ifndef canvas_adapter_h_20131213
#define canvas_adapter_h_20131213

#include <memory>
#include <type_traits>
#include <takisy/cgl/basic/color.h>
#include <takisy/cgl/basic/canvas.h>

class canvas_adapter
{
public:
    typedef std::shared_ptr<canvas_adapter> pointer;

public:
    virtual ~canvas_adapter(void) {}

public:
    template <typename PixelMatrix>
    canvas_adapter& operator=(const PixelMatrix& pixel_format);
    canvas_adapter& operator=(const canvas_adapter& canvas_adapter);


public:
    virtual void clear(void) = 0;
    virtual void clear(color color) = 0;
    virtual void resize(unsigned int width, unsigned int height) = 0;
    virtual unsigned int pixel_flag(void) const = 0;
    virtual unsigned int pixel_bytes(void) const { return 0; };
    virtual unsigned int width(void) const = 0;
    virtual unsigned int height(void) const = 0;
    virtual color pixel(int x, int y) const = 0;
    virtual unsigned char* buffer(void) = 0;
    virtual const unsigned char* buffer(void) const = 0;
    virtual void pixel(int x, int y, const color& color) = 0;
    virtual void pixel(int x, int y, const absolute_color& color) = 0;

    unsigned int row_bytes(void) const
        { return width() * pixel_bytes(); }
    unsigned int bytes(void) const
        { return height() * row_bytes(); }
    unsigned char* row_buffer(unsigned int y)
        { return buffer() + y * row_bytes(); }
    const unsigned char* row_buffer(unsigned int y) const
        { return buffer() + y * row_bytes(); }

public:
    template <typename Canvas>
    static pointer make(void);
    template <typename Canvas>
    static pointer make(unsigned int width, unsigned int height);
    template <typename Canvas, typename PixelMatrix>
    static pointer make(const PixelMatrix& pixel_format);
    template <typename Canvas>
    static pointer make(const canvas_adapter& canvas_adapter);
    template <typename Canvas>
    static Canvas* cast(canvas_adapter* canvas_adapter);
    template <typename Canvas>
    static const Canvas* cast(const canvas_adapter* canvas_adapter);
};

template <typename Canvas>
struct derived_canvas_adapter : public canvas_adapter
{
    typedef Canvas canvas_type;
    typedef typename std::remove_reference<Canvas>::type noref_canvas_type;
    typedef typename noref_canvas_type::pixel_format pixel_format;

    canvas_type canvas;

public:
    derived_canvas_adapter(void) : canvas() {}
    derived_canvas_adapter(unsigned int width, unsigned int height)
        : canvas(width, height) {}
    derived_canvas_adapter(noref_canvas_type& canvas) : canvas(canvas) {}
    template <typename PixelMatrix>
    derived_canvas_adapter(const PixelMatrix& pm) : canvas(pm) {}

public:
    void clear(void) override
        { canvas.clear(); }
    void clear(color color) override
        { canvas.clear(color); }
    void resize(unsigned int width, unsigned int height) override
        { canvas.resize(width, height); }
    unsigned int pixel_flag(void) const override
        { return pixel_format::flag(); }
    unsigned int pixel_bytes(void) const override
        { return pixel_format::pixel_bytes(); }
    unsigned int width(void) const override
        { return canvas.width(); }
    unsigned int height(void) const override
        { return canvas.height(); }
    color pixel(int x, int y) const override
        { return canvas.pixel(x, y); }
    unsigned char* buffer(void) override
        { return (unsigned char*)(canvas.pixels().data()); }
    const unsigned char* buffer(void) const override
        { return (const unsigned char*)(canvas.pixels().data()); }
    void pixel(int x, int y, const color& color) override
        { canvas.pixel(x, y, color); }
    void pixel(int x, int y, const absolute_color& color) override
        { canvas.pixel(x, y, color); }
};

template <typename PixelMatrix>
canvas_adapter& canvas_adapter::operator=(const PixelMatrix& pm)
{
    resize(pm.width(), pm.height());

    for (unsigned int y = 0; y < pm.height(); ++y)
    for (unsigned int x = 0; x < pm.width();  ++x)
        pixel(x, y, absolute_color(pm.pixel(x, y)));

    return *this;
}

inline canvas_adapter& canvas_adapter::operator=(const canvas_adapter& ca)
{
    if (this != &ca)
    {
        if (pixel_flag() == 0 || pixel_flag() != ca.pixel_flag())
            operator=<canvas_adapter>(ca);
        else
        {
            resize(ca.width(), ca.height());
            memcpy(buffer(), ca.buffer(), bytes());
        }
    }

    return *this;
}

template<typename Canvas>
inline canvas_adapter::pointer canvas_adapter::make(void)
{
    return pointer(new derived_canvas_adapter<Canvas>());
}

template <typename Canvas>
inline canvas_adapter::pointer
    canvas_adapter::make(unsigned int width, unsigned int height)
{
    return pointer(new derived_canvas_adapter<Canvas>(width, height));
}

template <typename Canvas, typename PixelMatrix>
inline canvas_adapter::pointer canvas_adapter::make(const PixelMatrix& pm)
{
    return pointer(new derived_canvas_adapter<Canvas>(pm));
}

template <typename Canvas>
canvas_adapter::pointer canvas_adapter::make(const canvas_adapter& ca)
{
    const Canvas* canvasptr = cast<Canvas>(&ca);

    if (canvasptr)
        return pointer(new derived_canvas_adapter<Canvas>(*canvasptr));

    return make<Canvas, canvas_adapter>(ca);
}

template <typename Canvas>
Canvas* canvas_adapter::cast(canvas_adapter* canvas_adapter)
{
    derived_canvas_adapter<Canvas>* dca =
        dynamic_cast<derived_canvas_adapter<Canvas>*>(canvas_adapter);
    if (dca)
        return &dca->canvas;

    return nullptr;
}

template <typename Canvas>
const Canvas* canvas_adapter::cast(const canvas_adapter* canvas_adapter)
{
    const derived_canvas_adapter<Canvas>* dca =
        dynamic_cast<const derived_canvas_adapter<Canvas>*>(canvas_adapter);
    if (dca)
        return &dca->canvas;

    return nullptr;
}

#endif // canvas_adapter_h_20131213
