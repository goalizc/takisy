#ifndef check_h_20160309
#define check_h_20160309

#include <string>
#include <takisy/core/handler.h>
#include <takisy/gui/widget/widget.h>
#include <takisy/gui/widget/label.h>

class check : public widget
{
    class implement;

public:
    DECLARE_HANDLER(onChecked);

public:
    check(void);
    check(widget* content);
    check(widget* content, bool checked);
   ~check(void);

private:
    using widget::add;
    using widget::remove;

public:
    widget* content(void) const;
    bool    checked(void) const;
    Size    optimal(OptimalPolicy policy=opUnset) const override;

public:
    void content(widget* content);
    void checked(bool checked);

public:
    void onSize(void) override;
    void onPaint(graphics graphics, Rect rect) override;
    bool onMouseDown(sys::Button button, int times, Point point) override;

protected:
    class implement* impl_;
};

class text_check : public check
{
    class implement;

public:
    text_check(const std::string& text);
    text_check(const std::string& text, bool checked);
    text_check(const std::string& text, const std::string& codec);
    text_check(const std::string& text, const std::string& codec, bool checked);
    text_check(const std::wstring& text);
    text_check(const std::wstring& text, bool checked);
   ~text_check(void);

private:
    using check::content;

public:
    label&       text(void);
    const label& text(void) const;

private:
    class implement* impl_;
};

#endif // check_h_20160309
