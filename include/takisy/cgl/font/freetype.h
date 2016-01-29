#ifndef freetype_h_20140826
#define freetype_h_20140826

#include <takisy/cgl/path/path.h>
#include <takisy/cgl/font/font.h>

class freetype : public font
{
    class implement;

public:
    enum RenderMode
    {
        rmNormal = 0,
        rmLight  = 1,
        rmMono   = 2,
    };

public:
    freetype(const char* font_file);
    freetype(const char* font_file, unsigned int size);
    freetype(const char* font_file, int face_index, unsigned int size);
   ~freetype(void);

private:
    freetype(const freetype& freetype);
    freetype& operator=(const freetype& freetype);

public:
    void render_mode(RenderMode render_mode);
    void ignore_bitmap(bool ignore);
    bool fontsize(unsigned int size);
    bool fontsize(unsigned int width, unsigned int height);
    void embolden(bool embolden);
    void embolden(double strength);
    void embolden(double xstrength, double ystrength);
    void italic(bool italic);
    void italic(double lean);

public:
    double ascender(void) const;
    double descender(void) const;
    double height(void) const override;
    double emheight(void) const override;
    RenderMode render_mode(void) const;

public:
    const bitmap* get_bitmap(unsigned int char_code) const override;

private:
    class implement* impl_;
};

#endif // freetype_h_20140826
