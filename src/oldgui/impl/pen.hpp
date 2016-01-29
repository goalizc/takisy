#ifndef pen_implement_hpp20140120
#define pen_implement_hpp20140120

#include <takisy/cgl/path/vertex_set.h>
#include <takisy/cgl/path/builder/contour.h>
#include <takisy/cgl/path/builder/dashed_line.h>

typedef paths Paths;

class Pen::Implement
{
    friend class Graphics;
    friend class Pen;

public:
    Implement(void)
        : brush_(nullptr), dashed_line_(nullptr)
    {}

public:
    template <typename Path>
    inline const Paths& build(const Path& path, bool closed)
    {
        if (dashed_line_)
        {
            contour_.closed(false);
            dashed_line_->closed(closed);
        }
        else
            contour_.closed(closed);

        return dashed_line_ ? stroke(dashed_line_->build(path)) : stroke(path);
    }

private:
    const Paths& stroke(const path& path)
    {
        vertex_set vertex_set = path;

        for (unsigned int i = 0; i < vertex_set.size(); ++i)
        {
            path::vertex_type& vertex = vertex_set[i];

            vertex.x += 0.5;
            vertex.y += 0.5;
        }

        return contour_.build(vertex_set);
    }

    const Paths& stroke(const paths& paths)
    {
        class paths vertex_sets;
        vertex_set* vertices = nullptr;

        for (unsigned int i = 0; i < paths.size(); ++i)
        {
            vertices = new vertex_set(*paths.fetch_path(i));

            for (unsigned int j = 0; j < vertices->size(); ++j)
            {
                path::vertex_type& vertex = vertices->operator[](j);

                vertex.x += 0.5;
                vertex.y += 0.5;
            }

            vertex_sets.append(vertices);
        }

        return contour_.build(vertex_sets);
    }

private:
    contour contour_;
    std::shared_ptr<Brush> brush_;
    std::shared_ptr<dashed_line> dashed_line_;
};

#endif // pen_implement_hpp20140120
