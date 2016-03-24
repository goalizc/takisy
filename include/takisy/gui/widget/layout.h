#ifndef layout_h_20151126
#define layout_h_20151126

#include <takisy/gui/basic/define.h>
#include <takisy/gui/widget/widget.h>

class layout : public widget
{
protected:
    class implement;

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
    void onAdd(widget* widget) override;
    void onRemove(widget* widget) override;
    void onSize(void) override;
    bool onChildMoving(widget* child, Point& point) override;
    bool onChildSizing(widget* child, Size& size) override;
    void onChildShown(widget* child) override;
    void onChildHidden(widget* child) override;

protected:
    class implement* impl_;
};

class spacer_layout : public layout
{
    friend class horizontal_layout;
    friend class vertical_layout;
    class implement;

protected:
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
    class implement;

public:
    horizontal_layout(void);
};

class vertical_layout : public spacer_layout
{
    class implement;

public:
    vertical_layout(void);
};

class grid_layout final : public layout
{
    class implement;

public:
    grid_layout(unsigned int column, unsigned int row);

public:
    widget* item(unsigned int col, unsigned int row) const;
    widget* item(unsigned int col, unsigned int row, widget* item);

public:
    bool onAdding(widget* widget) override;
    bool onRemoving(widget* widget) override;

private:
    class implement* impl_;
};

#endif // layout_h_20151126
