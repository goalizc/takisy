#ifndef slider_h_20140117
#define slider_h_20140117

#include <memory>
#include <takisy/oldgui/widget/widget.h>

class Slider : public Widget
{
    class Implement;

    class OnSlideListener
    {
    public:
        virtual void onSlide(double value) = 0;
    };

public:
    Slider(void);
    Slider(double min, double max);
    Slider(double min, double max, double value);
    Slider(const Slider& slider);
   ~Slider(void);

    template <typename Lambda>
    Slider(double min, double max, Lambda lambda)
        : Slider(min, max) { onSlide(lambda); }

    template <typename Lambda>
    Slider(double min, double max, double value, Lambda lambda)
        : Slider(min, max, value) { onSlide(lambda); }

    Slider& operator=(const Slider& slider);

public:
    double min(void) const;
    double max(void) const;
    double value(void) const;
    bool horizontal(void) const;
    bool negativeDirection(void) const;

    void min(double min);
    void max(double max);
    void minmax(double min, double max);
    void value(double value);
    void horizontal(bool horizontal);
    void negativeDirection(bool negative_direction);

public:
    void onPaint(Graphics graphics) override;
    bool onMouseDown(MouseButton mb, KeyState ks, Point point) override;
    bool onMouseMove(KeyState ks, Point point) override;
    bool onMouseUp(MouseButton mb, KeyState ks, Point point) override;

public:
    template <typename Lambda>
    void onSlide(Lambda lambda);

private:
    void onSlide(const std::shared_ptr<OnSlideListener>& listener);

private:
    class Implement* impl_;
};

template <typename Lambda>
void Slider::onSlide(Lambda lambda)
{
    class LambdaListener : public OnSlideListener
    {
    public: LambdaListener(Lambda lambda) : lambda_(lambda) {}
    public: void onSlide(double value) override { lambda_(value); }
    private: Lambda lambda_;
    };

    onSlide(std::shared_ptr<OnSlideListener>(new LambdaListener(lambda)));
}

#endif // slider_h_20140117
