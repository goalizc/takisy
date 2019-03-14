#ifndef color_scheme_h_20160309
#define color_scheme_h_20160309

#include <takisy/cgl/basic/color.h>

class color_scheme
{
    class implement;

public:
    color_scheme(void);
    color_scheme(const color_scheme& colorscheme);
   ~color_scheme(void);

    color_scheme& operator=(const color_scheme& colorscheme);

public:
    color theme(void) const;
    color background(void) const;
    color border(void) const;
    color text(void) const;
    color selection(void) const;
    color inactive_text(void) const;
    color inactive_selection(void) const;
    color hyperlink(void) const;
    color other(const char* name) const;

public:
    void theme(const color& color);
    void background(const color& color);
    void border(const color& color);
    void text(const color& color);
    void selection(const color& color);
    void inactive_text(const color& color);
    void inactive_selection(const color& color);
    void hyperlink(const color& color);
    void other(const char* name, const color& color);

public:
    static color_scheme& default_color_scheme(void);

public:
    class implement* impl_;
};

#endif // color_scheme_h_20160309
