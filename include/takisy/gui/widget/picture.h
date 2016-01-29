#ifndef picture_h_20160105
#define picture_h_20160105

#include <takisy/core/stream.h>
#include <takisy/cgl/image/image.h>
#include <takisy/gui/widget/widget.h>

class picture : public widget
{
    class implement;

public:
    picture(void);
    explicit
    picture(const char* uri);
    picture(const char* uri, const char* suffix);
    picture(const stream& stream);
    picture(const class image& image);
   ~picture(void);

public:
    bool load_uri(const char* uri);
    bool load_uri(const char* uri, const char* suffix);
    bool load_file(const char* file_path);
    bool load_file(const char* file_path, const char* suffix);
    bool load_stream(const stream& stream);
    bool load_stream(const stream& stream, const char* suffix);

public:
    bool               scalable(void) const;
    bool               keep_ratio(void) const;
    bool               dynamic(void) const;
    class image&       image(void);
    const class image& image(void) const;
    widget::Size       optimal(void) const;

public:
    void scalable(bool scalable);
    void keep_ratio(bool keep_ratio);
    void dynamic(bool dynamic);
    void image(const class image& image);

public:
    void onPaint(graphics graphics, Rect rect) override;

private:
    class implement* impl_;
};

#endif // picture_h_20160105
