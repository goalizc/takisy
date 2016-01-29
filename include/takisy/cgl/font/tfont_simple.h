#ifndef tfont_simple_h_20131101
#define tfont_simple_h_20131101

#include <takisy/cgl/font/font.h>

class tfont_simple : public font
{
public:
    double height(void) const override;
    double emheight(void) const override;
    const bitmap* get_bitmap(unsigned int char_code) const override;

public:
    static const tfont_simple* get(void);
};

#endif //tfont_simple_h_20131101
