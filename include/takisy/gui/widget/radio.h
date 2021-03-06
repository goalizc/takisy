#ifndef radio_h_20160816
#define radio_h_20160816

#include <string>
#include <vector>
#include <takisy/core/handler.h>
#include <takisy/gui/widget/widget.h>
#include <takisy/gui/widget/label.h>

using namespace std;

class radio : public widget
{
    class implement;

public:
    DECLARE_HANDLER(onSelected);
    DECLARE_HANDLER(onDisselected);

public:
    radio(void);
    radio(class widget* widget);
    radio(class widget* widget, bool selected);
   ~radio(void);

private:
    using widget::add;
    using widget::remove;

public:
    class widget* widget(void) const;
    bool selected(void) const;
    Size optimal(OptimalPolicy policy=opUnset) const override;

public:
    void widget(class widget* widget);
    void select(void);

public:
    void onSize(void) override;
    void onPaint(graphics graphics, Rect rect) override;
    bool onMouseDown(sys::Button button, int times, Point point) override;

protected:
    class implement* impl_;
};

class text_radio : public radio
{
    class implement;

public:
    text_radio(const string& text);
    text_radio(const string& text, bool selected);
    text_radio(const string& text, const string& codec);
    text_radio(const string& text,
               const string& codec, bool selected);
    text_radio(const wstring& text);
    text_radio(const wstring& text, bool selected);
   ~text_radio(void);

private:
    using radio::widget;

public:
    label&       text(void);
    const label& text(void) const;

private:
    class implement* impl_;
};

class radio_group : public widget
{
    class implement;

public:
    DECLARE_HANDLER(onSelectionChanged);

public:
    radio_group(void);
    radio_group(widget** items, unsigned int count);
    radio_group(const vector<widget*>& items);
   ~radio_group(void);

private:
    using widget::add;
    using widget::remove;

public:
    unsigned int    count(void) const;
    vector<widget*> items(void) const;
    widget*         item(unsigned int index) const;
    widget*         selected(void) const;
    int             selected_index(void) const;
    Size            optimal(OptimalPolicy policy=opUnset) const override;

    unsigned int    margin(void) const;
    unsigned int    spacing(void) const;

public:
    void append(widget* item);
    void insert(unsigned int index, widget* item);
    void remove(widget* item);
    void remove(unsigned int index);
    void remove(unsigned int begin, unsigned int end);
    void clear(void);
    void select(unsigned int index);
    void select(widget* selected);

    void margin(unsigned int margin);
    void spacing(unsigned int spacing);

public:
    void onSize(void) override;
    void onPaint(graphics graphics, Rect rect) override;
    bool onMouseDown(sys::Button button, int times, Point point) override;

private:
    class implement* impl_;
};

class text_radio_group : public radio_group
{
public:
    text_radio_group(void);
    text_radio_group(const vector<string>& items);
    text_radio_group(const vector<string>& items, const string& codec);
    text_radio_group(const vector<wstring>& items);
   ~text_radio_group(void);

private:
    using radio_group::append;
    using radio_group::insert;

public:
    void append(const string& item);
    void append(const string& item, const string& codec);
    void append(const wstring& item);
    void insert(unsigned int index, const string& item);
    void insert(unsigned int index, const string& item, const string& codec);
    void insert(unsigned int index, const wstring& item);

public:
    label&       text(unsigned int index);
    const label& text(unsigned int index) const;

public:
    void onRemove(widget* widget) override;
};

#endif // radio_h_20160816
