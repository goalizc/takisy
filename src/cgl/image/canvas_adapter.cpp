#include <takisy/cgl/image/canvas_adapter.h>

canvas_adapter& canvas_adapter::operator=(const canvas_adapter& ca)
{
    if (this != &ca)
        operator=<canvas_adapter>(ca);

    return *this;
}

unsigned int canvas_adapter::row_bytes(void) const
{
    return width() * pixel_bytes();
}

unsigned int canvas_adapter::bytes(void) const
{
    return height() * row_bytes();
}

unsigned char* canvas_adapter::row_buffer(unsigned int y)
{
    return buffer() + y * row_bytes();
}

const unsigned char* canvas_adapter::row_buffer(unsigned int y) const
{
    return buffer() + y * row_bytes();
}
