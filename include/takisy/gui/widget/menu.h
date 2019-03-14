#ifndef menu_h_20160920
#define menu_h_20160920

#include <takisy/core/stream.h>
#include <takisy/cgl/image/image.h>
#include <takisy/gui/widget/layout.h>

class menu : public vertical_layout
{
    class implement;

public:
    class item : public horizontal_layout
    {
        class implement;

    public:
        DECLARE_HANDLER(onClick);
        DECLARE_HANDLER(onTextChanged);

    public:
        item(void);
       ~item(void);

    public:
        class menu* submenu(void);
        const class menu* submenu(void) const;
        std::wstring text(void) const;
        Size optimal(OptimalPolicy policy=opUnset) const override;

    public:
        class item& submenu(class menu* submenu);
        class item& text(const std::string& text);
        class item& text(const std::string& text, const std::string& codec);
        class item& text(const std::wstring& text);
        class item& icon(const char* uri);
        class item& icon(const stream& istream);
        class item& icon(const image& image);

    public:
        void onEndPaint(graphics graphics, Rect rect) override;

    private:
        class implement* impl_;
    };

public:
    DECLARE_HANDLER(onItemClicked, class item& /* item */);

public:
    menu(void);
   ~menu(void);

public:
    unsigned int count(void);
    class item* item(int index);
    const class item* item(int index) const;
    Size optimal(OptimalPolicy policy=opUnset) const override;

public:
    void add_separator(void);
    class item& add(const std::string& text);
    class item& add(const std::string& text, const std::string& codec);
    class item& add(const std::wstring& text);

    void submenu(int index, class menu* submenu);
    void text(int index, const std::string& text);
    void text(int index, const std::string& text, const std::string& codec);
    void text(int index, const std::wstring& text);
    void icon(int index, const char* uri);
    void icon(int index, const stream& istream);
    void icon(int index, const image& image);

public:
    void show(widget* widget, Point point);
    void show(widget* widget, int x, int y);
    void close(void);

public:
    void onAdd(widget* widget) override;
    void onRemove(widget* widget) override;
    void onPaint(graphics graphics, Rect rect) override;
    bool onMouseDown(sys::Button button, int times, Point point) override;
    bool onMouseUp(sys::Button button, int times, Point point) override;
    bool onMouseMove(Point point) override;

private:
    class implement* impl_;
};

#endif // menu_h_20160920
