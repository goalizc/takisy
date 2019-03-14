#include <takisy/core/algo.h>
#include <takisy/core/math.h>
#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/basic/raster.h>
#include <takisy/cgl/path/vertex_fetcher.h>

class raster::implement
{
    friend class raster;

    struct subpixel
    {
        enum
        {
            shift = 8,
            scale = 1 << shift,
            mask  = scale - 1,
        };

    public:
        static inline int i(int n) { return n >> shift; }
        static inline int f(int n) { return n  &  mask; }
    };

    struct cell
    {
        int x, cover, area;

    public:
        inline bool operator<(const cell& c) const
        {
            return x < c.x;
        }
    };

    typedef stretchy_buffer<cell>            cell_line;
    typedef stretchy_buffer<cell_line, true> cell_matrix;

public:
    implement(void)
        : evenodd_(false), bbox_(0, 0, 0, 0)
    {}

public:
    template <typename Path>
    void reset(const Path& path)
    {
        rectf bbox = path.bbox();

        bbox_.left   = math::floor(bbox.left);
        bbox_.right  = math::ceil (bbox.right);
        bbox_.top    = math::floor(bbox.top);
        bbox_.bottom = math::ceil (bbox.bottom);

        for (cell_line& cl : cell_matrix_)
            cl.clear();

        cell_matrix_.resize(bbox_.height());
    }

    void normalize(void)
    {
        for (cell_line& cl : cell_matrix_)
        {
            unsigned int i = 0, o = 0;

            algo::sort(cl.data(), cl.size());

            while (i < cl.size())
            {
                cl[o] = cl[i++];

                while (i < cl.size() && cl[i].x == cl[o].x)
                {
                    cl[o].cover += cl[i].cover;
                    cl[o].area  += cl[i].area;
                    ++i;
                }

                ++o;
            }

            cl.resize(o);
        }
    }

    scanline& fetch_scanline(int y)
    {
        const cell_line& cl = cell_matrix_[y - bbox_.top];

        scanline_.clear();

        if (cl.size() > 0)
        {
            int sum_cover = cl[0].cover;

            scanline_.append(span {
                .x        = cl[0].x,
                .length   = 1,
                .coverage = calculate_coverage(sum_cover, cl[0].area)
            });

            for (unsigned int i = 1; i < cl.size(); ++i)
            {
                if (cl[i].x > cl[i - 1].x + 1 && sum_cover != 0)
                    scanline_.append(span {
                        .x        = cl[i - 1].x + 1,
                        .length   = (unsigned int)(cl[i].x - cl[i - 1].x - 1),
                        .coverage = calculate_coverage(sum_cover, 0)
                    });

                sum_cover += cl[i].cover;

                scanline_.append(span {
                    .x        = cl[i].x,
                    .length   = 1,
                    .coverage = calculate_coverage(sum_cover, cl[i].area)
                });
            }
        }

        return scanline_;
    }

    inline void build_edge_cells(const path::vertex_type& vertex_1,
                                 const path::vertex_type& vertex_2)
    {
        build_edge_cells_claa(vertex_1, vertex_2);
    }

private:
    void build_edge_cells_claa(const path::vertex_type& vertex_1,
                               const path::vertex_type& vertex_2)
    {
        point _1(vertex_1.x * subpixel::scale, vertex_1.y * subpixel::scale);
        point _2(vertex_2.x * subpixel::scale, vertex_2.y * subpixel::scale);

        if (_1.y == _2.y)
            return;

        int ix_1 = subpixel::i(_1.x), fx_1 = subpixel::f(_1.x);
        int iy_1 = subpixel::i(_1.y), fy_1 = subpixel::f(_1.y);
        int iy_2 = subpixel::i(_2.y), fy_2 = subpixel::f(_2.y);

        if (_1.x == _2.x)
        {
            vertical_line(ix_1, fx_1, iy_1, fy_1, iy_2, fy_2);
            return;
        }

        int dy      = math::abs(_2.y - _1.y) * (_1.x < _2.x ? 1 : -1);
        int dx      = math::abs(_2.x - _1.x);
        int dx_roll = dx << subpixel::shift;
        int epsilon = 0, x_from, x_to;

        if (iy_1 < iy_2)
        {
            epsilon += dx * (subpixel::scale - fy_1);
            horizontal_line(iy_1, fy_1, subpixel::scale,
                            _1.x, (x_from = x_to = _1.x + epsilon / dy));
            epsilon %= dy;

            for (int y = iy_1 + 1; y < iy_2; ++y)
            {
                epsilon += dx_roll;
                x_to = x_from + epsilon / dy;
                horizontal_line(y, 0, subpixel::scale, x_from, x_to);
                x_from = x_to;
                epsilon %= dy;
            }

            horizontal_line(iy_2, 0, fy_2, x_from, _2.x);
        }
        else
        if (iy_1 > iy_2)
        {
            epsilon += dx * fy_1;
            horizontal_line(iy_1, fy_1, 0,
                            _1.x, (x_from = x_to = _1.x + epsilon / dy));
            epsilon %= dy;

            for (int y = iy_1 - 1; y > iy_2; --y)
            {
                epsilon += dx_roll;
                x_to = x_from + epsilon / dy;
                horizontal_line(y, subpixel::scale, 0, x_from, x_to);
                x_from = x_to;
                epsilon %= dy;
            }

            horizontal_line(iy_2, subpixel::scale, fy_2, x_from, _2.x);
        }
        else
            horizontal_line(iy_1, fy_1, fy_2, _1.x, _2.x);
    }

    void build_edge_cells_sfa(const path::vertex_type& vertex_1,
                              const path::vertex_type& vertex_2)
    {
        build_edge_cells_claa(vertex_1, vertex_2);
    }

    void horizontal_line(int iy, int fy_1, int fy_2, int x_1, int x_2)
    {
        if (fy_1 == fy_2)
            return;

        int ix_1 = subpixel::i(x_1), fx_1 = subpixel::f(x_1);
        int ix_2 = subpixel::i(x_2), fx_2 = subpixel::f(x_2);

        if (ix_1 < ix_2)
        {
            int dx      = x_2  - x_1;
            int dy      = fy_2 - fy_1;
            int dy_roll = dy << subpixel::shift;
            int epsilon = 0;

            epsilon += dy * (subpixel::scale - fx_1);
            add_cell(ix_1, iy, epsilon / dx, fx_1 + subpixel::scale);
            epsilon %= dx;

            for (int x = ix_1 + 1; x < ix_2; ++x)
            {
                epsilon += dy_roll;
                add_cell(x, iy, epsilon / dx, subpixel::scale);
                epsilon %= dx;
            }

            epsilon += dy * fx_2;
            add_cell(ix_2, iy, epsilon / dx, fx_2);
        }
        else
        if (ix_1 > ix_2)
        {
            int dx      = x_1  - x_2;
            int dy      = fy_2 - fy_1;
            int dy_roll = dy << subpixel::shift;
            int epsilon = 0;

            epsilon += dy * fx_1;
            add_cell(ix_1, iy, epsilon / dx, fx_1);
            epsilon %= dx;

            for (int x = ix_1 - 1; x > ix_2; --x)
            {
                epsilon += dy_roll;
                add_cell(x, iy, epsilon / dx, subpixel::scale);
                epsilon %= dx;
            }

            epsilon += dy * (subpixel::scale - fx_2);
            add_cell(ix_2, iy, epsilon / dx, subpixel::scale + fx_2);
        }
        else
        if (ix_1 == ix_2)
            add_cell(ix_1, iy, fy_2 - fy_1, fx_1 + fx_2);
    };

    void vertical_line(int ix, int fx, int iy_1, int fy_1, int iy_2, int fy_2)
    {
        int fx_x2 = fx << 1;

        if (iy_1 < iy_2)
        {
            add_cell(ix, iy_1, subpixel::scale - fy_1, fx_x2);

            for (int y = iy_1 + 1; y < iy_2; ++y)
                add_cell(ix, y, subpixel::scale, fx_x2);

            add_cell(ix, iy_2, fy_2, fx_x2);
        }
        else
        if (iy_1 > iy_2)
        {
            add_cell(ix, iy_1, -fy_1, fx_x2);

            for (int y = iy_1 - 1; y > iy_2; --y)
                add_cell(ix, y, -subpixel::scale, fx_x2);

            add_cell(ix, iy_2, fy_2 - subpixel::scale, fx_x2);
        }
        else
            add_cell(ix, iy_1, fy_2 - fy_1, fx_x2);
    }

    inline void add_cell(int x, int y, int cover, int sum_fx)
    {
        if (cover != 0)
            cell_matrix_[y - bbox_.top].append(cell {
                .x     = x,
                .cover = cover,
                .area  = cover * sum_fx
            });
    }

    inline unsigned char calculate_coverage(int cover, int area)
    {
        static constexpr unsigned int shift = subpixel::shift + 1;
        int coverage = ((cover << shift) - area) >> shift;

        if (coverage < 0)
            coverage = -coverage;

        if (evenodd_)
        {
            static constexpr unsigned int scale_x2 = subpixel::scale << 1;
            static constexpr unsigned int mask_x2  = scale_x2 - 1;

            coverage &= mask_x2;
            if (coverage > subpixel::scale)
                coverage = scale_x2 - coverage;
        }

        return coverage >= subpixel::mask ? 255 :
            subpixel::shift < 8 ? coverage << (8 - subpixel::shift) :
            subpixel::shift > 8 ? coverage >> (subpixel::shift - 8) : coverage;
    }

private:
    bool evenodd_;

    rect        bbox_;
    scanline    scanline_;
    cell_matrix cell_matrix_;
};

raster::raster(void)
    : impl_(new implement)
{}

raster::~raster(void)
{
    delete impl_;
}

bool raster::evenodd(void) const
{
    return impl_->evenodd_;
}

void raster::evenodd(bool evenodd)
{
    impl_->evenodd_ = evenodd;
}

void raster::rasterize(const path& path)
{
    impl_->reset(path);

    if (path.size() >= 3)
    {
        vertex_fetcher vf(path);

        for (unsigned int i = 0; i < vf.size(); ++i)
            impl_->build_edge_cells(vf[i + 0], vf[i + 1]);

        impl_->normalize();
    }
}

void raster::rasterize(const paths& paths)
{
    impl_->reset(paths);

    for (unsigned int i = 0; i < paths.size(); ++i)
    {
        vertex_fetcher vf(*paths[i]);

        if (vf.size() >= 3)
            for (unsigned int j = 0; j < vf.size(); ++j)
                impl_->build_edge_cells(vf[j + 0], vf[j + 1]);
    }

    impl_->normalize();
}

rect raster::bbox(void) const
{
    return impl_->bbox_;
}

raster::scanline raster::fetch_scanline(int y) const
{
    return std::move(impl_->fetch_scanline(y));
}
