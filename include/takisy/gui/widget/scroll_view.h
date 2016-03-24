#ifndef scroll_view_h_20160302
#define scroll_view_h_20160302

#include <takisy/gui/widget/widget.h>
#include <takisy/gui/widget/scroll.h>

class scroll_view : public widget
{
    class implement;

public:
    scroll_view(widget* content);
   ~scroll_view(void);

public:
    widget* content(void) const;
    vertical_scroll& vertical_scroll(void);
    horizontal_scroll& horizontal_scroll(void);

public:
    void onSize(void) override;
    void onChildSize(widget* child) override;
    bool onMouseWheel(int delta, Point point) override;

private:
    class implement* impl_;
};

#endif // scroll_view_h_20160302
