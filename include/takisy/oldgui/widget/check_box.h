#ifndef check_box_h_20150209
#define check_box_h_20150209

#include <memory>
#include <string>
#include <takisy/oldgui/basic/handler.h>
#include <takisy/oldgui/widget/widget.h>

class CheckBox : public Widget
{
    class Implement;

public:
    CheckBox(const std::string& caption);
    CheckBox(const std::string& caption, bool checked);
   ~CheckBox(void);

public:
    bool checked(void) const;
    std::string caption(void) const;

public:
    void check(bool checked);
    void caption(const std::string& caption);

public:
    template <typename Lambda>
    void onStatusChange(Lambda lambda);
    void onStatusChange(const std::shared_ptr<Handler>& handler);

public:
    void onPaint(Graphics graphics) override;
    bool onClick(MouseButton mb, KeyState ks, int times, Point point) override;

private:
    class Implement* impl_;
};

template <typename Lambda>
inline void CheckBox::onStatusChange(Lambda lambda)
{
    onStatusChange(get_lambda_handler<Lambda, CheckBox>(lambda));
}

#endif // check_box_h_20150209
