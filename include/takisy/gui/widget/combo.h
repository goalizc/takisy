#ifndef combo_h_20170303
#define combo_h_20170303

#include <vector>
#include <string>
#include <takisy/core/handler.h>
#include <takisy/gui/widget/layout.h>
#include <takisy/gui/widget/list.h>
#include <takisy/gui/widget/text_edit.h>

class combo : public horizontal_layout
{
    class implement;

public:
    DECLARE_HANDLER(onSelectionChanged);

public:
    combo(void);
    combo(const std::vector<std::string>& items);
    combo(const std::vector<std::string>& items, const std::string& codec);
    combo(const std::vector<std::wstring>& items);
   ~combo(void);

public:
    std::wstring selected(void) const;
    unsigned int selected_index(void) const;

public:
    text_edit& text(void);
    const text_edit& text(void) const;
    class list& list(void);
    const class list& list(void) const;

public:
    void select(unsigned int index);

public:
    void onSize(void) override;
    void onEndPaint(graphics graphics, Rect rect) override;
    bool onMouseMove(Point point) override;

private:
    class implement* impl_;
};

#endif // combo_h_20170303
