#ifndef button_h_include_20140116
#define button_h_include_20140116

#include <memory>
#include <string>
#include <takisy/oldgui/widget/widget.h>

class Button : public Widget
{
    class Implement;

    class OnClickListener
    {
    public:
        virtual bool
            onClick(MouseButton mb, KeyState ks, int times, Point point) = 0;
    };

public:
    Button(void);
    Button(const Button& button);
   ~Button(void);

    Button& operator=(const Button& button);

public:
    void  bottom_color(Color bottom_color);
    Color bottom_color(void) const;

public:
    void onPaint(Graphics graphics) override;
    bool onMouseDown(MouseButton mb, KeyState ks, Point point) override;
    bool onMouseUp(MouseButton mb, KeyState ks, Point point) override;
    bool onMouseEnter(KeyState ks, Point point) override;
    bool onMouseLeave(KeyState ks, Point point) override;

public:
    template <typename Lambda>
    void onClick(Lambda lambda);

private:
    void onClick(const std::shared_ptr<OnClickListener>& listener);
    bool onClick(MouseButton mb, KeyState ks, int times, Point point) override;

private:
    class Implement* impl_;
};

template <typename Lambda>
void Button::onClick(Lambda lambda)
{
    class LambdaListener : public OnClickListener
    {
    public: LambdaListener(Lambda lambda) : lambda_(lambda) {}
    public: bool onClick(MouseButton mb, KeyState ks, Point point) override
        { return lambda_(mb, ks, point); }
    private: Lambda lambda_;
    };

    onClick(std::shared_ptr<OnClickListener>(new LambdaListener(lambda)));
}

class TextButton : public Button
{
public:
    TextButton(void);
    TextButton(const std::wstring& caption);
    TextButton(const std::wstring& caption, Color foreground_color);
    TextButton(const TextButton& button);
   ~TextButton(void);

    TextButton& operator=(const TextButton& button);

public:
    void         caption(const std::wstring& caption);
    std::wstring caption(void) const;

    void  foreground_color(Color foreground_color);
    Color foreground_color(void) const;

public:
    void onPaint(Graphics graphics) override;

private:
    std::wstring caption_;
    Color        foreground_color_;
};

#endif // button_h_include_20140116
