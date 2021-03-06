#ifndef label_h_20151208
#define label_h_20151208

#include <string>
#include <takisy/core/handler.h>
#include <takisy/gui/basic/define.h>
#include <takisy/gui/widget/widget.h>

class label : public widget
{
    class implement;

public:
    DECLARE_HANDLER(onTextChanged);

public:
    label(void);
    label(const std::string& text);
    label(const std::string& text, const std::string& codec);
    label(const std::wstring& text);
   ~label(void);

public:
    std::wstring      text(void) const;
    Margin            margin(void) const;
    int               margin_left(void) const;
    int               margin_top(void) const;
    int               margin_right(void) const;
    int               margin_bottom(void) const;
    unsigned int      indent(void) const;
    bool              word_wrap(void) const;
    unsigned int      line_spacing(void) const;
    unsigned int      word_spacing(void) const;
    const class font* font(void) const;
    brush_sptr        text_brush(void) const;
    brush_sptr        background_brush(void) const;
    bool              fixed_brush(void) const;
    unsigned int      alignment(void) const;
    Size              optimal(OptimalPolicy policy=opUnset) const override;

public:
    void text(const std::string& text);
    void text(const std::string& text, const std::string& codec);
    void text(const std::wstring& text);
    void margin(int margin);
    void margin(const Margin& margin);
    void margin(int left, int top, int right, int bottom);
    void margin_left(int left);
    void margin_top(int top);
    void margin_right(int right);
    void margin_bottom(int bottom);
    void indent(unsigned int indent);
    void word_wrap(bool word_wrap);
    void line_spacing(unsigned int line_spacing);
    void word_spacing(unsigned int word_spacing);
    void font(const class font* font);
    void text_color(const color& text_color);
    void text_brush(const brush_sptr& text_brush);
    void background_color(const color& background_color);
    void background_brush(const brush_sptr& background_brush);
    void fixed_brush(bool fixed);
    void alignment(unsigned int alignment);

public:
    void onSize(void) override;
    void onPaint(graphics graphics, Rect rect) override;

private:
    class implement* impl_;
};

#endif // label_h_20151208
