#ifndef vertex_fetcher_h_20131220
#define vertex_fetcher_h_20131220

#include <takisy/cgl/path/path.h>

class vertex_fetcher
{
public:
    inline vertex_fetcher(const class path& path)
        : path_(path)
    {}

public:
    inline unsigned int size(void) const
    {
        return path_.size();
    }

    inline path::vertex_type fetch_vertex(int i) const
    {
        register int bar = path_.size();

        return path_.fetch_vertex(i < 0 ? (bar + i % bar) % bar : i % bar);
    }

private:
    const class path& path_;
};

#endif // vertex_fetcher_h_20131220
