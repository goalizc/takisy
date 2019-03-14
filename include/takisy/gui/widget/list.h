#ifndef list_h_20160905
#define list_h_20160905

#include <set>
#include <vector>
#include <string>
#include <takisy/core/handler.h>
#include <takisy/gui/widget/scroll.h>

class list : public scroll_area
{
    class implement;

public:
    DECLARE_HANDLER(onCurrentItemChanged,
                    unsigned int /* current  */,
                    unsigned int /* previous */);
    DECLARE_HANDLER(onItemTextChanged, unsigned int /* index */);
    DECLARE_HANDLER(onItemClicked,
                    unsigned int /* index  */,
                    sys::Button  /* button */);
    DECLARE_HANDLER(onItemDoubleClicked,
                    unsigned int /* index  */,
                    sys::Button  /* button */);
    DECLARE_HANDLER(onItemSelected, unsigned int /* index */);
    DECLARE_HANDLER(onItemDisselected, unsigned int /* index */);
    DECLARE_HANDLER(onItemSelectionChanged, unsigned int /* index */);
    DECLARE_HANDLER(onSelectionChanged,
                    const std::vector<unsigned int>& /* indexes  */,
                    bool                             /* selected */);

public:
    list(void);
    list(const std::vector<std::string>& items);
    list(const std::vector<std::string>& items, const std::string& codec);
    list(const std::vector<std::wstring>& items);
   ~list(void);

public:
    unsigned int  count(void) const;
    std::vector<std::wstring> items(void) const;
    std::wstring  item(unsigned int index) const;
    unsigned int  current(void) const;
    unsigned int  edit_trigger(void) const;
    bool          editable(unsigned int index) const;
    SelectionMode selection_mode(void) const;
    bool          selected(unsigned int index) const;
    std::set<unsigned int> selected(void) const;
    const class font* font(unsigned int index) const;
    brush_sptr    text_brush(unsigned int index) const;
    brush_sptr    background_brush(unsigned int index) const;
    brush_sptr    selection_brush(unsigned int index) const;
    unsigned int  alignment(unsigned int index) const;
    Size          optimal(OptimalPolicy policy=opUnset) const override;

public:
    void item(unsigned int index, const std::string& text);
    void item(unsigned int index,
              const std::string& text, const std::string& codec);
    void item(unsigned int index, const std::wstring& text);
    void append(const std::string& text);
    void append(const std::string& text, const std::string& codec);
    void append(const std::wstring& text);
    void insert(unsigned int index, const std::string& text);
    void insert(unsigned int index,
                const std::string& text, const std::string& codec);
    void insert(unsigned int index, const std::wstring& text);
    void remove(unsigned int index);
    void clear(void);
    void current(unsigned int index);
    void edit_trigger(unsigned int edit_trigger);
    void editable(unsigned int index, bool editable);
    void selection_mode(SelectionMode selection_mode);
    void selected(unsigned int index, bool selected);
    void selected(unsigned int begin, unsigned int end, bool selected);
    void selected(const std::set<unsigned int>& indexes, bool selected);
    void select(unsigned int index);
    void select(unsigned int begin, unsigned int end);
    void select(const std::set<unsigned int>& indexes);
    void disselect(unsigned int index);
    void disselect(unsigned int begin, unsigned int end);
    void disselect(const std::set<unsigned int>& indexes);
    void clear_selection(void);
    void font(const class font* font);
    void font(unsigned int index, const class font* font);
    void text_color(const color& color);
    void text_color(unsigned int index, const color& color);
    void text_brush(const brush_sptr& brush);
    void text_brush(unsigned int index, const brush_sptr& brush);
    void background_color(const color& color);
    void background_color(unsigned int index, const color& color);
    void background_brush(const brush_sptr& brush);
    void background_brush(unsigned int index, const brush_sptr& brush);
    void selection_color(const color& color);
    void selection_color(unsigned int index, const color& color);
    void selection_brush(const brush_sptr& brush);
    void selection_brush(unsigned int index, const brush_sptr& brush);
    void alignment(unsigned int alignment);
    void alignment(unsigned int index, unsigned int alignment);

public:
    void edit(unsigned int index);
    void edit(unsigned int index, const std::string& text);
    void edit(unsigned int index,
              const std::string& text, const std::string& codec);
    void edit(unsigned int index, const std::wstring& text);
    long hittest(Point point) const;
    void scrollto(unsigned int index);
    void scrollto(unsigned int index, unsigned int alignment);

public:
    void onSize(void) override;
    void onPaint(graphics graphics, Rect rect) override;
    bool onFocus(bool focus) override;
    bool onKeyDown(sys::VirtualKey vkey) override;
    bool onKeyPress(unsigned int chr) override;
    bool onMouseDown(sys::Button button, int times, Point point) override;
    bool onMouseMove(Point point) override;
    bool onMouseUp(sys::Button button, int times, Point point) override;

private:
    class implement* impl_;
};

#endif // list_h_20160905
