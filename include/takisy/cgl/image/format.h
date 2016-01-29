#ifndef format_h_20131120
#define format_h_20131120

#include <takisy/core/stream.h>
#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/image/frame.h>

class format
{
public:
    typedef stretchy_buffer<frame, true> frames;

public:
    virtual ~format(void) {}

public:
    virtual bool load(const stream& stream, frames& frames) = 0;
    virtual bool dump(const frames& frames, stream& stream) const = 0;
};

#define define_image_format(class_name)                                 \
    class class_name : public format                                    \
    {                                                                   \
        class implement;                                                \
                                                                        \
    public:                                                             \
        bool load(const stream& stream, frames& frames) override;       \
        bool dump(const frames& frames, stream& stream) const override; \
    };

define_image_format(bmp);
define_image_format(gif);
define_image_format(png);

#undef define_image_format

#endif //format_h_20131120
