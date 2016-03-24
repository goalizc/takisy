#ifndef check_h_20160309
#define check_h_20160309

#include <takisy/core/handler.h>
#include <takisy/gui/widget/widget.h>

class check : public widget
{
    class implement;

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
    Size    optimal_size(void) const override;

public:
    void content(widget* content);
    void checked(bool checked);

public:
    void onSize(void) override;
    bool onChildSizing(widget* child, Size& size) override;
    void onPaint(graphics graphics, Rect rect) override;
    bool onClick(sys::MouseButton button, int times, Point point) override;

protected:
    class implement* impl_;
};

class text_check : public check
{
    class implement;

public:
    text_check(const char* text);
    text_check(const char* text, bool checked);
    text_check(const char* text, const char* codec);
    text_check(const char* text, const char* codec, bool checked);
    text_check(const wchar_t* text);
    text_check(const wchar_t* text, bool checked);
   ~text_check(void);

private:
    using check::content;

public:
    bool           word_wrap(void) const;
    const char*    text(const char* codec) const;
    const wchar_t* text(void) const;
    const font&    font(void) const;

public:
    void word_wrap(bool word_wrap);
    void text(const char* text);
    void text(const char* text, const char* codec);
    void text(const wchar_t* text);
    void font(const class font& font);

private:
    class implement* impl_;
};

#endif // check_h_20160309
