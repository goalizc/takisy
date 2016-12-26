#ifndef scroll_view_h_20160302
#define scroll_view_h_20160302

#include <takisy/gui/widget/scroll.h>

class scroll_view : public scroll_area
{
    class implement;

public:
    scroll_view(widget* content);
   ~scroll_view(void);

public:
    widget* content(void) const;

public:
    bool onChildMoving(widget* child, Point& point) override;
    void onChildSize(widget* child) override;

private:
    class implement* impl_;
};

#endif // scroll_view_h_20160302
