#include <memory>
#include <third_party/gpc.h>
#include <takisy/core/memory.h>
#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/path/vertex_set.h>
#include <takisy/cgl/path/builder/rectangle.h>
#include <takisy/cgl/path/path.h>

class gpc
{
public:
    template <typename Paths>
    static const paths& clip(const Paths& paths, unsigned int count,
                             const path& edge, ClipType clip_type)
    {
        gpc_vertex_list gvl     = {1, memory::malloc<gpc_vertex>()};
        gpc_polygon     subject = {0, nullptr, nullptr};
        gpc_polygon     clip    = {0, nullptr, nullptr};
        gpc_polygon     result  = {0, nullptr, nullptr};

        for (unsigned int i = 0; i < count; ++i)
            gpc_add_contour(&subject, &gpc::fill(gvl, paths[i]), 0);
        gpc_add_contour(&clip, &gpc::fill(gvl, edge), 0);

        gpc_op op;
        switch (clip_type)
        {
        default:
        case ctInt:   op = GPC_INT;   break;
        case ctDiff:  op = GPC_DIFF;  break;
        case ctXor:   op = GPC_XOR;   break;
        case ctUnion: op = GPC_UNION; break;
        }
        gpc_polygon_clip(op, &subject, &clip, &result);

        static class paths clipped_paths;
        clipped_paths.clear();

        for (int i = 0; i < result.num_contours; ++i)
        {
            vertex_set* clipped_result = new vertex_set;
            const gpc_vertex_list& gvl = result.contour[i];

            for (int i = 0; i < gvl.num_vertices; ++i)
                clipped_result->append(gvl.vertex[i].x, gvl.vertex[i].y);

            clipped_paths.append(clipped_result);
        }

        gpc_free_polygon(&result);
        gpc_free_polygon(&clip);
        gpc_free_polygon(&subject);
        memory::free(gvl.vertex, gvl.num_vertices);

        return clipped_paths;
    }

private:
    static gpc_vertex_list& fill(gpc_vertex_list& gvl, const path& path)
    {
        if (path.size() > static_cast<unsigned int>(gvl.num_vertices))
            memory::realloc(gvl.vertex, gvl.num_vertices, path.size());
        gvl.num_vertices = path.size();

        for (unsigned int i = 0; i < path.size(); ++i)
        {
            const path::vertex_type& vertex = path.fetch_vertex(i);
            gvl.vertex[i].x = vertex.x;
            gvl.vertex[i].y = vertex.y;
        }

        return gvl;
    }

    static inline gpc_vertex_list& fill(
            gpc_vertex_list& gvl, const std::shared_ptr<path>& path)
    {
        return fill(gvl, *path);
    }
};

path::~path(void) {}

const paths& path::clip(const rectf& rect) const
{
    return clip(rect.left, rect.top, rect.right, rect.bottom);
}

const paths& path::clip(vertex_type::axis_type left,
                        vertex_type::axis_type top,
                        vertex_type::axis_type right,
                        vertex_type::axis_type bottom) const
{
    return clip(rectangle().build(left, top, right, bottom));
}

const paths& path::clip(const path& edge) const
{
    return clip(edge, ctInt);
}

const paths& path::clip(const path& edge, ClipType clip_type) const
{
    return gpc::clip(this, 1, edge, clip_type);
}

bool path::pnpoly(const vertex_type& point) const
{
    return pnpoly(point.x, point.y);
}

bool path::pnpoly(vertex_type::axis_type x, vertex_type::axis_type y) const
{
    bool c = false;

    for (unsigned int i = 0, j = size() - 1; i < size(); j = i++)
    {
        vertex_type vi = fetch_vertex(i);
        vertex_type vj = fetch_vertex(j);

        if (((vi.y > y) != (vj.y > y))
            && (x < vi.x - (vi.y - y) * (vi.x - vj.x) / (vi.y - vj.y)))
            c = !c;
    }

    return c;
}

rectf path::bbox(void) const
{
    if (size() == 0)
        return rectf(0, 0, 0, 0);
    else
    {
        vertex_type vertex = fetch_vertex(0);
        rectf bbox(vertex.x, vertex.y, vertex.x, vertex.y);

        for (unsigned int i = 1; i < size(); ++i)
        {
            vertex = fetch_vertex(i);

            if (bbox.left   > vertex.x) bbox.left   = vertex.x;
            else
            if (bbox.right  < vertex.x) bbox.right  = vertex.x;

            if (bbox.top    > vertex.y) bbox.top    = vertex.y;
            else
            if (bbox.bottom < vertex.y) bbox.bottom = vertex.y;
        }

        return bbox;
    }
}

class paths::implement
{
    friend class paths;

private:
    stretchy_buffer<std::shared_ptr<path>, true> paths_;
};

paths::paths(void)
    : impl_(new implement)
{}

paths::paths(const paths& _paths)
    : paths()
{
    operator=(_paths);
}

paths::~paths(void)
{
    delete impl_;
}

paths& paths::operator=(const paths& paths)
{
    if (this != &paths)
        impl_->paths_ = paths.impl_->paths_.clone();

    return *this;
}

const paths& paths::clip(const rectf& rect) const
{
    return clip(rect.left, rect.top, rect.right, rect.bottom);
}

const paths& paths::clip(path::vertex_type::axis_type left,
                         path::vertex_type::axis_type top,
                         path::vertex_type::axis_type right,
                         path::vertex_type::axis_type bottom) const
{
    return clip(rectangle().build(left, top, right, bottom));
}

const paths& paths::clip(const path& edge) const
{
    return clip(edge, ctInt);
}

const paths& paths::clip(const path& edge, ClipType clip_type) const
{
    return gpc::clip(impl_->paths_, impl_->paths_.size(), edge, clip_type);
}

unsigned int paths::size(void) const
{
    return impl_->paths_.size();
}

path* paths::fetch_path(unsigned int index) const
{
    return impl_->paths_[index].get();
}

bool paths::pnpoly(const path::vertex_type& point) const
{
    return pnpoly(point.x, point.y);
}

bool paths::pnpoly(path::vertex_type::axis_type x,
                   path::vertex_type::axis_type y) const
{
    for (unsigned int i = 0; i < size(); ++i)
        if (fetch_path(i)->pnpoly(x, y))
            return true;

    return false;
}

rectf paths::bbox(void) const
{
    if (size() == 0)
        return rectf{0, 0, 0, 0};
    else
    {
        rectf bbox = fetch_path(0)->bbox();

        for (unsigned int i = 1; i < size(); ++i)
        {
            rectf bbox2 = fetch_path(i)->bbox();

            if (bbox.left   > bbox2.left  ) bbox.left   = bbox2.left;
            if (bbox.right  < bbox2.right ) bbox.right  = bbox2.right;
            if (bbox.top    > bbox2.top   ) bbox.top    = bbox2.top;
            if (bbox.bottom < bbox2.bottom) bbox.bottom = bbox2.bottom;
        }

        return bbox;
    }
}

void paths::clear(void)
{
    impl_->paths_.clear();
}

void paths::append(path* path)
{
    impl_->paths_.append(std::shared_ptr<class path>(path));
}
