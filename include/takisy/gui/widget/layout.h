#ifndef layout_h_20151126
#define layout_h_20151126

#include <takisy/gui/basic/define.h>
#include <takisy/gui/widget/widget.h>

class layout : public widget
{
    friend class widget;
    class implement;

public:
    layout(void);
   ~layout(void);

public:
    Alignment     alignment(void) const;
    struct margin margin(void) const;
    unsigned int  spacing(void) const;

public:
    void alignment(Alignment alignment);
    void margin(const struct margin& margin);
    void margin(unsigned int margin);
    void margin(unsigned int left, unsigned int top,
                unsigned int right, unsigned int bottom);
    void margin_left(unsigned int left);
    void margin_top(unsigned int top);
    void margin_right(unsigned int right);
    void margin_bottom(unsigned int left);
    void spacing(unsigned int spacing);

private:
    virtual void readapt(void) = 0;

public:
    virtual void onSize(Size size) override;

protected:
    class implement* impl_;
};

class spacer_layout : public layout
{
    friend class horizontal_layout;
    friend class vertical_layout;
    class implement;

public:
    spacer_layout(void);
   ~spacer_layout(void);

public:
    void add_spacer(void);
    void add_spacer(unsigned int size);
    void add_spacer(unsigned int min_size, unsigned int max_size);

protected:
    class implement* impl_;
};

class horizontal_layout : public spacer_layout
{
public:
    horizontal_layout(void);

public:
    void readapt(void) override;
};

class vertical_layout : public spacer_layout
{
public:
    vertical_layout(void);

public:
    void readapt(void) override;
};

class grid_layout : public layout
{
};

#endif // layout_h_20151126
