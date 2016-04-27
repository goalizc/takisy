#ifndef rasterizer_h_20130802
#define rasterizer_h_20130802

#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/path/path.h>

class raster
{
    class implement;

public:
    struct span
    {
        int x;
        unsigned int length:24;
        unsigned int coverage:8;
    };

    typedef stretchy_buffer<span> scanline;

public:
    raster(void);
   ~raster(void);

private:
    raster(const raster&) = delete;
    raster& operator=(const raster&) = delete;

public:
    void evenodd(bool evenodd);
    bool evenodd(void) const;

public:
    void rasterize(const path& path);
    void rasterize(const paths& paths);

public:
    rect     bbox(void) const;
    scanline fetch_scanline(int y) const;

private:
    class implement* impl_;
};

#endif //rasterizer_h_20130802
