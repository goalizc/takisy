#include <takisy/core/memory.h>
#include <takisy/cgl/image/canvas_adapter.h>

canvas_adapter& canvas_adapter::operator=(const canvas_adapter& ca)
{
    if (this != &ca)
    {
        resize(ca.width(), ca.height());

        if (flag() == ca.flag())
            memory::std::memcpy(buffer(), ca.buffer(), height() * row_bytes());
        else
            for (unsigned int y = 0; y < height(); ++y)
            for (unsigned int x = 0; x < width(); ++x)
                pixel(x, y, ca.pixel(x, y));
    }

    return *this;
}
