#ifndef pen_implement_hpp_20150716
#define pen_implement_hpp_20150716

#include <takisy/cgl/path/builder/contour.h>
#include <takisy/cgl/path/builder/dashed_line.h>

class pen::implement
{
    friend class graphics;
    friend class pen;

public:
    implement(void)
        : brush_(nullptr), dashed_line_(nullptr)
    {}

public:
    template <typename Path>
    inline paths build(const Path& path, bool closed)
    {
        if (dashed_line_)
        {
            contour_.closed(false);
            dashed_line_->closed(closed);

            return contour_.build(dashed_line_->build(path));
        }
        else
        {
            contour_.closed(closed);

            return contour_.build(path);
        }
    }

private:
    contour contour_;
    brush_sptr brush_;
    std::shared_ptr<dashed_line> dashed_line_;
};

#endif // pen_implement_hpp_20150716
