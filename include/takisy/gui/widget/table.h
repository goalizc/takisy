#ifndef table_h_20150922
#define table_h_20150922

#include <set>
#include <vector>
#include <string>
#include <initializer_list>
#include <takisy/core/handler.h>
#include <takisy/gui/widget/scroll.h>

class table : public scroll_area
{
    class implement;

public:
    typedef const class font* fontptr;

    struct item_index
    {
        int row, col;

    public:
        bool operator==(const item_index& that) const;
        bool operator!=(const item_index& that) const;
        bool operator <(const item_index& that) const;
    };

    struct item;
    struct gridline;
    struct row_header;
    struct column_header;

public:
    DECLARE_HANDLER(onCurrentItemChanged,
                    item_index /* current  */,
                    item_index /* previous */);
    DECLARE_HANDLER(onCurrentRowChanged,
                    unsigned int /* current  */,
                    unsigned int /* previous */);
    DECLARE_HANDLER(onCurrentColumnChanged,
                    unsigned int /* current  */,
                    unsigned int /* previous */);
    DECLARE_HANDLER(onItemTextChanged, item_index /* index */);
    DECLARE_HANDLER(onItemClicked,
                    item_index   /* index  */,
                    sys::Button  /* button */,
                    unsigned int /* times  */);
    DECLARE_HANDLER(onItemDoubleClicked,
                    item_index  /* index  */,
                    sys::Button /* button */);
    DECLARE_HANDLER(onItemMouseEnter, item_index /* index */)
    DECLARE_HANDLER(onItemMouseLeave, item_index /* index */)
    DECLARE_HANDLER(onItemSelected, item_index /* index */);
    DECLARE_HANDLER(onItemDisselected, item_index /* index */);
    DECLARE_HANDLER(onItemSelectionChanged, item_index /* index */);
    DECLARE_HANDLER(onSelectionChanged,
                    const std::vector<item_index>& /* indexes  */,
                    bool                           /* selected */);

public:
    table(void);
    table(unsigned int column);
    table(unsigned int column, unsigned int row);
   ~table(void);

public:
    unsigned int      rows(void) const;
    unsigned int      columns(void) const;
    bool              row_header_visible(void) const;
    unsigned int      row_header_width(void) const;
    bool              column_header_visible(void) const;
    unsigned int      column_header_height(void) const;
    unsigned int      row_height(unsigned int row) const;
    unsigned int      column_width(unsigned int col) const;
    item_index        current(void) const;
    SelectionMode     selection_mode(void) const;
    SelectionBehavior selection_behavior(void) const;
    unsigned int      edit_trigger(void) const;
    bool              selected(const item_index& index) const;

public:
    const class item          item(const item_index& index) const;
    const class row_header    row_header(unsigned int index) const;
    const class column_header column_header(unsigned int index) const;
    const class gridline      border(void) const;
    const class gridline      vertical_gridline(unsigned int index) const;
    const class gridline      horizontal_gridline(unsigned int index) const;

public:
    class item          item(const item_index& index);
    class row_header    row_header(unsigned int index);
    class column_header column_header(unsigned int index);
    class gridline      border(void);
    class gridline      vertical_gridline(unsigned int index);
    class gridline      horizontal_gridline(unsigned int index);

public:
    void header_visible(bool visible);
    void row_header_visible(bool visible);
    void row_header_width(unsigned int width);
    void row_height(unsigned int row, unsigned int height);
    void column_header_visible(bool visible);
    void column_header_height(unsigned int height);
    void column_width(unsigned int col, unsigned int width);
    void show_header(void);
    void hide_header(void);
    void show_row_header(void);
    void hide_row_header(void);
    void show_column_header(void);
    void hide_column_header(void);

    void append_row(void);
    void append_row(const std::string& header);
    void append_row(const std::string& header, const std::string& codec);
    void append_row(const std::wstring& header);
    void insert_row(unsigned int row);
    void insert_row(unsigned int row, const std::string& header);
    void insert_row(unsigned int row,
                    const std::string& header, const std::string& codec);
    void insert_row(unsigned int row, const std::wstring& header);
    void remove_row(unsigned int row);
    void append_column(void);
    void append_column(const std::string& header);
    void append_column(const std::string& header, const std::string& codec);
    void append_column(const std::wstring& header);
    void insert_column(unsigned int col);
    void insert_column(unsigned int col, const std::string& header);
    void insert_column(unsigned int col,
                       const std::string& header, const std::string& codec);
    void insert_column(unsigned int col, const std::wstring& header);
    void remove_column(unsigned int col);
    void clear(void);
    void clear_rows(void);
    void clear_content(void);
    void clear_row(unsigned int row);
    void clear_column(unsigned int col);

    void current(const item_index& index);
    void current_row(unsigned int row);
    void current_column(unsigned int col);

    void selection_mode(SelectionMode selection_mode);
    void selection_behavior(SelectionBehavior selection_behavior);
    void edit_trigger(unsigned int edit_trigger);
    void selected(const item_index& index, bool selected);
    void selected(const item_index& begin, const item_index& end,
                  bool selected);
    void selected(const std::set<item_index>& indexes, bool selected);
    void select(const item_index& index);
    void select(const item_index& begin, const item_index& end);
    void select(const std::set<item_index>& indexes);
    void disselect(const item_index& index);
    void disselect(const item_index& begin, const item_index& end);
    void disselect(const std::set<item_index>& indexes);
    void clear_selection(void);

public:
    item_index hittest(Point point) const;
    void       scrollto(const item_index& index);
    void       scrollto_row(unsigned int row);
    void       scrollto_column(unsigned int col);
    void       item_repaint(const item_index& index);

public:
    void onSize(void) override;
    void onPaint(graphics graphics, Rect rect) override;
    bool onFocus(bool focus) override;
    bool onKeyDown(sys::VirtualKey vkey) override;
    bool onKeyPress(unsigned int chr) override;
    bool onMouseDown(sys::Button button, int times, Point point) override;
    bool onMouseUp(sys::Button button, Point point) override;
    bool onMouseMove(Point point) override;

private:
    class implement* impl_;
};

struct table::item
{
    class implement;
    std::shared_ptr<class implement> impl;

public:
    Rect                rect(void) const;
    const std::wstring& text(void) const;
    Margin              margin(void) const;
    unsigned int        indent(void) const;
    unsigned int        alignment(void) const;
    bool                multiline(void) const;
    bool                word_wrap(void) const;
    unsigned int        line_spacing(void) const;
    unsigned int        word_spacing(void) const;
    fontptr             font(void) const;
    brush_sptr          text_brush(void) const;
    brush_sptr          background_brush(void) const;
    brush_sptr          selection_brush(void) const;
    bool                editable(void) const;

public:
    void text(const std::string& text);
    void text(const std::string& text, const std::string& codec);
    void text(const std::wstring& text);
    void margin(int margin);
    void margin(int left, int top, int right, int bottom);
    void margin_left(int left);
    void margin_top(int top);
    void margin_right(int right);
    void margin_bottom(int bottom);
    void margin(const Margin& margin);
    void indent(unsigned int indent);
    void alignment(unsigned int alignment);
    void multiline(bool multiline);
    void word_wrap(bool word_wrap);
    void line_spacing(unsigned int line_spacing);
    void word_spacing(unsigned int word_spacing);
    void font(fontptr font);
    void text_color(const color& color);
    void text_brush(const brush_sptr& brush);
    void background_color(const color& color);
    void background_brush(const brush_sptr& brush);
    void selection_color(const color& color);
    void selection_brush(const brush_sptr& brush);
    void editable(bool editable);

public:
    void edit(void);
    void edit(const std::string& text);
    void edit(const std::string& text, const std::string& codec);
    void edit(const std::wstring& text);
};

struct table::gridline
{
    class implement;
    std::shared_ptr<class implement> impl;

public:
    unsigned int        width(void) const;
    brush_sptr          brush(void) const;
    std::vector<double> dash_array(void) const;

public:
    void width(unsigned int width);
    void color(const class color& color);
    void brush(brush_sptr brush);
    void dash_array(const std::vector<double>& dash_array);
    void dash_array(std::initializer_list<double> initlist);
    void dash_array(const double* dash_array, unsigned int count);
};

struct table::row_header : public item
{
    class implement;
    std::shared_ptr<class implement> impl;

public:
    unsigned int height(void) const;
    void         height(unsigned int height);
};

struct table::column_header : public item
{
    class implement;
    std::shared_ptr<class implement> impl;

public:
    unsigned int width(void) const;
    void         width(unsigned int width);
};

#endif // table_h_20150922
