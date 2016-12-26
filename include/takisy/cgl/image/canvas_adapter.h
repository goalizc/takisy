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
    canvas_adapter& operator=(const PixelMatrix& pm);
    canvas_adapter& operator=(const canvas_adapter& ca);

public:
    virtual void clear(void) = 0;
    virtual void clear(color color) = 0;
    virtual void resize(unsigned int width, unsigned int height) = 0;
    virtual unsigned int pixel_bytes(void) const = 0;
    virtual unsigned int width(void) const = 0;
    virtual unsigned int height(void) const = 0;
    virtual color pixel(int x, int y) const = 0;
    virtual unsigned char* buffer(void) = 0;
    virtual const unsigned char* buffer(void) const = 0;
    virtual void pixel(int x, int y, const color& color) = 0;
    virtual void pixel(int x, int y, const absolute_color& color) = 0;

    unsigned int row_bytes(void) const;
    unsigned int bytes(void) const;
    unsigned char* row_buffer(unsigned int y);
    const unsigned char* row_buffer(unsigned int y) const;

public:
    template <typename Canvas>
    static pointer make(void);
    template <typename Canvas>
    static pointer make(unsigned int width, unsigned int height);
    template <typename Canvas, typename PixelMatrix>
    static pointer make(const PixelMatrix& pm);
};

template <typename Canvas>
class derived_canvas_adapter : public canvas_adapter
{
    friend class canvas_adapter;
    typedef typename std::remove_reference<Canvas>::type noref_canvas;
    typedef typename noref_canvas::pixel_format pixel_format;

public:
    derived_canvas_adapter(void) : canvas() {}
    derived_canvas_adapter(unsigned int width, unsigned int height)
        : canvas(width, height) {}
    derived_canvas_adapter(noref_canvas& canvas) : canvas(canvas) {}
    template <typename PixelMatrix>
    derived_canvas_adapter(const PixelMatrix& pm) : canvas(pm) {}

public:
    void clear(void) override
        { canvas.clear(); }
    void clear(color color) override
        { canvas.clear(color); }
    void resize(unsigned int width, unsigned int height) override
        { canvas.resize(width, height); }
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

private:
    Canvas canvas;
};

template <typename PixelMatrix>
canvas_adapter& canvas_adapter::operator=(const PixelMatrix& pm)
{
    resize(pm.width(), pm.height());

    for (unsigned int y = 0; y < pm.height(); ++y)
    for (unsigned int x = 0; x < pm.width(); ++x)
        pixel(x, y, absolute_color(pm.pixel(x, y)));

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

#endif // canvas_adapter_h_20131213
