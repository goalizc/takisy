#ifndef color_scheme_h_20160309
#define color_scheme_h_20160309

#include <takisy/cgl/basic/color.h>

class color_scheme
{
    class implement;

public:
    color_scheme(void);
    color_scheme(const color& main, const color& cool, const color& warm);
    color_scheme(const color_scheme& color_scheme);
   ~color_scheme(void);

    color_scheme& operator=(const color_scheme& color_scheme);

public:
    color main(void) const;
    color main(signed char degree) const;
    color cool(void) const;
    color cool(signed char degree) const;
    color warm(void) const;
    color warm(signed char degree) const;

public:
    void main(const color& main);
    void cool(const color& cool);
    void warm(const color& warm);

public:
    static color_scheme& default_color_scheme(void);

public:
    static const color_scheme& seven_eleven(void);

private:
    class implement* impl_;
};

#endif // color_scheme_h_20160309
