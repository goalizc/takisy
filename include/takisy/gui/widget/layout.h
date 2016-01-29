#ifndef layout_h_20151126
#define layout_h_20151126

#include <takisy/gui/widget/widget.h>

class layout : public widget
{
    friend class widget;

    class implement;

public:
    struct Margin
    {
        unsigned int left, top, right, bottom;
    };

    enum Alignment
    {
        aFrontward, aCenter, aBackward
    };

public:
    layout(void);
   ~layout(void);

public:
    Alignment    alignment(void) const;
    Margin       margin(void) const;
    unsigned int spacing(void) const;

public:
    void alignment(Alignment alignment);
    void margin(const Margin& margin);
    void margin(unsigned int margin);
    void margin(unsigned int left, unsigned int top,
                unsigned int right, unsigned int bottom);
    void margin_left(unsigned int left);
    void margin_top(unsigned int top);
    void margin_right(unsigned int right);
    void margin_bottom(unsigned int left);
    void spacing(unsigned int spacing);

public:
    void add_spacer(void);
    void add_spacer(unsigned int size);
    void add_spacer(unsigned int min_size, unsigned int max_size);

public:
    void onSize(Size size) override;

protected:
    class implement* impl_;
};

typedef layout horizontal_layout;

class vertical_layout : public layout
{
public:
    vertical_layout(void);
};

#endif // layout_h_20151126
