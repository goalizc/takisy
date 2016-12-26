#ifndef edit_box_h_20160928
#define edit_box_h_20160928

#include <takisy/gui/widget/text_edit.h>
#include "../basic/text.hpp"

class edit_box : public text_edit
{
public:
    DECLARE_HANDLER(onEditComplete, const std::wstring& /* text */);
    DECLARE_HANDLER(onEditFinish);

public:
    static edit_box& pop(widget* father, const Rect& rect,
                         const class text& text, const std::wstring& content);

public:
    bool onFocus(bool focus) override;
    bool onKeyPress(unsigned int chr) override;
};

#endif // edit_box_h_20160928
