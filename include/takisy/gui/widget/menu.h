#ifndef menu_h_20160920
#define menu_h_20160920

#include <takisy/gui/widget/widget.h>

class menu : public widget
{
    class implement;

public:
    menu(void);

private:
    class implement* impl_;
};

#endif // menu_h_20160920
