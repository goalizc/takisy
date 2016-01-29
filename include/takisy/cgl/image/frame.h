#ifndef frame_h_20140408
#define frame_h_20140408

#include <takisy/cgl/image/canvas_adapter.h>

struct frame
{
    canvas_adapter::pointer canvas;
    unsigned int interval;
};

#endif // frame_h_20140408
