#ifndef combo_h_20161207
#define combo_h_20161207

#include <vector>
#include <string>
#include <initializer_list>
#include <takisy/core/handler.h>
#include <takisy/gui/widget/list.h>

class combo : public widget
{
    class implement;

public:
    combo(void);
    combo(const std::vector<std::string>& items);
    combo(const std::vector<std::string>& items, const std::string& codec);
    combo(const std::vector<std::wstring>& items);
    combo(std::initializer_list<std::string> combo);
    combo(std::initializer_list<std::string> combo, const std::string& codec);
    combo(std::initializer_list<std::wstring> combo);
   ~combo(void);

public:
    class list&       list(void);
    const class list& list(void) const;

public:
    void onPaint(graphics graphics, Rect rect) override;
    bool onFocus(bool focus) override;
    bool onKeyDown(sys::VirtualKey vkey) override;
    bool onMouseDown(sys::Button button, int times, Point point) override;
    bool onMouseWheel(int delta, Point point) override;

private:
    class implement* impl_;
};

#endif // combo_h_20161207
