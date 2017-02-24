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

    struct index
    {
        int row, col;

    public:
        bool operator==(const index& that) const;
        bool operator!=(const index& that) const;
        bool operator <(const index& that) const;
    };

    struct item;
    struct gridline;
    struct row_headers;
    struct row_header;
    struct row;
    struct column_headers;
    struct column_header;
    struct column;

public:
    DECLARE_HANDLER(onCurrentItemChanged,
                    index /* current  */,
                    index /* previous */);
    DECLARE_HANDLER(onCurrentRowChanged,
                    unsigned int /* current  */,
                    unsigned int /* previous */);
    DECLARE_HANDLER(onCurrentColumnChanged,
                    unsigned int /* current  */,
                    unsigned int /* previous */);
    DECLARE_HANDLER(onItemTextChanged, index /* index */);
    DECLARE_HANDLER(onItemClicked,
                    index        /* index  */,
                    sys::Button  /* button */,
                    unsigned int /* times  */);
    DECLARE_HANDLER(onItemDoubleClicked,
                    index       /* index  */,
                    sys::Button /* button */);
    DECLARE_HANDLER(onItemMouseEnter, index /* index */)
    DECLARE_HANDLER(onItemMouseLeave, index /* index */)
    DECLARE_HANDLER(onItemSelected, index /* index */);
    DECLARE_HANDLER(onItemDisselected, index /* index */);
    DECLARE_HANDLER(onItemSelectionChanged, index /* index */);
    DECLARE_HANDLER(onRowMouseEnter, unsigned int /* row */);
    DECLARE_HANDLER(onRowMouseLeave, unsigned int /* row */);
    DECLARE_HANDLER(onColumnMouseEnter, unsigned int /* col */);
    DECLARE_HANDLER(onColumnMouseLeave, unsigned int /* col */);
    DECLARE_HANDLER(onSelectionChanged,
                    const std::vector<index>& /* indexes  */,
                    bool                      /* selected */);

public:
    table(void);
    table(unsigned int column);
    table(unsigned int column, unsigned int row);
   ~table(void);

public:
    unsigned int      rows(void) const;
    unsigned int      columns(void) const;
    index             current(void) const;
    SelectionMode     selection_mode(void) const;
    SelectionBehavior selection_behavior(void) const;
    unsigned int      edit_trigger(void) const;
    bool              selected(const index& index) const;

public:
    const struct item           item(const index& index) const;
    const struct gridline       border(void) const;
    const struct row_headers    row_headers(void) const;
    const struct column_headers column_headers(void) const;
    const struct row            row(unsigned int index) const;
    const struct column         column(unsigned int index) const;

public:
    struct item           item(const index& index);
    struct gridline       border(void);
    struct row_headers    row_headers(void);
    struct column_headers column_headers(void);
    struct row            row(unsigned int index);
    struct column         column(unsigned int index);

public:
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

    void current(const index& index);
    void current_row(unsigned int row);
    void current_column(unsigned int col);

    void selection_mode(SelectionMode selection_mode);
    void selection_behavior(SelectionBehavior selection_behavior);
    void edit_trigger(unsigned int edit_trigger);
    void selected(const index& index, bool selected);
    void selected(const index& begin, const index& end, bool selected);
    void selected(const std::set<index>& indexes, bool selected);
    void select(const index& index);
    void select(const index& begin, const index& end);
    void select(const std::set<index>& indexes);
    void disselect(const index& index);
    void disselect(const index& begin, const index& end);
    void disselect(const std::set<index>& indexes);
    void clear_selection(void);
    void text_color(color color);
    void text_brush(const brush_sptr& brush);
    void background_color(color color);
    void background_brush(const brush_sptr& brush);
    void selection_color(color color);
    void selection_brush(const brush_sptr& brush);

public:
    index hittest(Point point) const;
    void  scrollto(const index& index);
    void  scrollto(const index& index, unsigned int alignment);
    void  scrollto_row(unsigned int row);
    void  scrollto_row(unsigned int row, unsigned int alignment);
    void  scrollto_column(unsigned int col);
    void  scrollto_column(unsigned int col, unsigned int alignment);
    void  repaint_item(const index& index);

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
    struct implement;
    std::shared_ptr<struct implement> impl;

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
    void text_color(color color);
    void text_brush(const brush_sptr& brush);
    void background_color(color color);
    void background_brush(const brush_sptr& brush);
    void selection_color(color color);
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
    struct implement;
    std::shared_ptr<struct implement> impl;

public:
    unsigned int        width(void) const;
    brush_sptr          brush(void) const;
    std::vector<double> dash_array(void) const;

public:
    void width(unsigned int width);
    void color(const color& color);
    void brush(const brush_sptr& brush);
    void dash_array(const std::vector<double>& dash_array);
    void dash_array(std::initializer_list<double> initlist);
    void dash_array(const double* dash_array, unsigned int count);
};

struct table::row_headers
{
    struct implement;
    std::shared_ptr<struct implement> impl;

public:
    bool visible(void) const;
    unsigned int width(void) const;

public:
    void show(void);
    void hide(void);
    void visible(bool visible);
    void width(unsigned int width);
};

struct table::row_header : public item
{
    struct implement;
    std::shared_ptr<struct implement> impl;

public:
    unsigned int height(void) const;
    void         height(unsigned int height);
};

struct table::row
{
    struct implement;
    std::shared_ptr<struct implement> impl;

public:
    unsigned int height(void) const;
    const table::gridline gridline(void) const;
    const table::row_header header(void) const;

public:
    void height(unsigned int height);
    void text_color(color color);
    void text_brush(const brush_sptr& brush);
    void background_color(color color);
    void background_brush(const brush_sptr& brush);
    void selection_color(color color);
    void selection_brush(const brush_sptr& brush);
    table::gridline gridline(void);
    table::row_header header(void);
};

struct table::column_headers
{
    struct implement;
    std::shared_ptr<struct implement> impl;

public:
    bool visible(void) const;
    unsigned int height(void) const;

public:
    void show(void);
    void hide(void);
    void visible(bool visible);
    void height(unsigned int height);
};

struct table::column_header : public item
{
    struct implement;
    std::shared_ptr<struct implement> impl;

public:
    unsigned int width(void) const;
    void         width(unsigned int width);
};

struct table::column
{
    struct implement;
    std::shared_ptr<struct implement> impl;

public:
    unsigned int width(void) const;
    const table::gridline gridline(void) const;
    const table::column_header header(void) const;

public:
    void width(unsigned int width);
    void text_color(color color);
    void text_brush(const brush_sptr& brush);
    void background_color(color color);
    void background_brush(const brush_sptr& brush);
    void selection_color(color color);
    void selection_brush(const brush_sptr& brush);
    table::gridline gridline(void);
    table::column_header header(void);
};

#endif // table_h_20150922
