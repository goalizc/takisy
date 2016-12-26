#ifndef format_h_20131120
#define format_h_20131120

#include <takisy/core/stream.h>
#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/image/canvas_adapter.h>

class format
{
public:
    struct frame
    {
        canvas_adapter::pointer canvas;
        unsigned int interval;
    };

    typedef stretchy_buffer<frame, true> frames;

public:
    virtual ~format(void) {}

public:
    virtual bool load(stream& stream, frames& frames) const = 0;
    virtual bool dump(const frames& frames, stream& stream) const = 0;
};

#define define_format(class_name)                                       \
    class class_name : public format                                    \
    {                                                                   \
        class implement;                                                \
                                                                        \
    public:                                                             \
        bool load(stream& stream, frames& frames) const override;       \
        bool dump(const frames& frames, stream& stream) const override; \
    };

define_format(png);
define_format(gif);
define_format(bmp);
define_format(stb);

#undef define_format

#endif //format_h_20131120
