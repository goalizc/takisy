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

    inline path::vertex_type at(int i) const
    {
        register int n = path_.size();

        return path_.at(i < 0 ? (n + i % n) % n : i % n);
    }

    inline path::vertex_type operator[](int index) const
    {
        return at(index);
    }

private:
    const class path& path_;
};

#endif // vertex_fetcher_h_20131220
