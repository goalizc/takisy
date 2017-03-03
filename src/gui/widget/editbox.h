#ifndef editbox_h_20160928
#define editbox_h_20160928

#include <takisy/gui/widget/text_edit.h>
#include "../basic/text.hpp"

class editbox : public text_edit
{
public:
    DECLARE_HANDLER(onEditComplete, const std::wstring& /* text */);
    DECLARE_HANDLER(onEditFinish);

public:
    static editbox& pop(widget* father, const Rect& rect,
                        const class text& text, const std::wstring& content);

public:
    bool onFocus(bool focus) override;
    bool onKeyPress(unsigned int chr) override;
};

#endif // editbox_h_20160928
