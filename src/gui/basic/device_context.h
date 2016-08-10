#ifndef device_context_h_20160112
#define device_context_h_20160112

#include <takisy/cgl/basic/rect.h>
#include <takisy/cgl/basic/canvas.h>

struct device_context
{
    virtual ~device_context(void) {}
    virtual void refresh(const rect& rect, const canvas_bgra8& canvas) = 0;
};

#endif // device_context_h_20160112
