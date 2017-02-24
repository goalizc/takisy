#ifndef image_h_20131120
#define image_h_20131120

#include <takisy/cgl/basic/color.h>
#include <takisy/cgl/image/format.h>

class image
{
    class implement;

public:
    image(void);
    explicit
    image(const char* uri);
    image(const char* uri, const format& format);
    image(const stream& stream);
    image(const stream& stream, const format& format);
    image(const image& image);
   ~image(void);

    image& operator=(const image& image);

public:
    static const format* register_format(const format* format);
    static void        deregister_format(const format* format);

public:
    bool load_uri(const char* uri);
    bool load_uri(const char* uri, const format& format);
    bool load_stream(const stream& stream);
    bool load_stream(const stream& stream, const format& format);
    bool dump_uri(const char* uri) const;
    bool dump_uri(const char* uri, const format& format) const;
    bool dump_stream(stream& stream) const;
    bool dump_stream(stream& stream, const format& format) const;

public:
    canvas_adapter& new_frame(unsigned int width, unsigned int height);
    canvas_adapter& new_frame(unsigned int width, unsigned int height,
                              unsigned int interval);
    canvas_adapter& new_frame(const canvas_adapter::pointer& canvas);
    canvas_adapter& new_frame(const canvas_adapter::pointer& canvas,
                              unsigned int interval);
    template <typename PixelMatrix> inline
    canvas_adapter& new_frame(const PixelMatrix& pixel_matrix);
    template <typename PixelMatrix> inline
    canvas_adapter& new_frame(const PixelMatrix& pixel_matrix,
                              unsigned int interval);

    unsigned int count(void) const;
    unsigned int duration(void) const;
    unsigned int seek(unsigned int timestamp) const;

          canvas_adapter& first_frame(void);
    const canvas_adapter& first_frame(void) const;
          canvas_adapter& frame(void);
    const canvas_adapter& frame(void) const;
          canvas_adapter& frame(unsigned int frame_index);
    const canvas_adapter& frame(unsigned int frame_index) const;
          canvas_adapter& last_frame(void);
    const canvas_adapter& last_frame(void) const;

    // PixelMatrix interface, equal to first_frame().<method>()
    // Warning: image must has 1 frame at least.
    unsigned int width(void) const;
    unsigned int height(void) const;
    color        pixel(unsigned int x, unsigned int y) const;
    void         pixel(unsigned int x, unsigned int y, const color& color);

    void clear(void);

private:
    class implement* impl_;
};

template <typename PixelMatrix>
canvas_adapter& image::new_frame(const PixelMatrix& pixel_matrix)
{
    return new_frame(canvas_adapter::make<canvas_bgra8>(pixel_matrix));
}

template <typename PixelMatrix>
canvas_adapter& image::new_frame(const PixelMatrix& pixel_matrix,
                                 unsigned int interval)
{
    return new_frame(canvas_adapter::make<canvas_bgra8>(pixel_matrix), interval);
}

#endif //image_h_20131120
