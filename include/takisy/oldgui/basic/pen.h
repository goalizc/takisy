#ifndef pen_h_20140118
#define pen_h_20140118

#include <initializer_list>
#include <takisy/oldgui/basic/brush.h>

class Pen
{
    friend class Graphics;
    class Implement;

public:
    enum Cap   { cButt, cSquare, cRound };
    enum Joint { jBevel, jRound, jMiter };

public:
    Pen(void);
    explicit
    Pen(double width);
    Pen(Color color);
    Pen(const std::shared_ptr<Brush>& brush);
    Pen(double width, Color color);
    Pen(double width, const std::shared_ptr<Brush>& brush);
    Pen(const Pen& pen);
   ~Pen(void);

    Pen& operator=(const Pen& pen);

public:
    double width(void) const;
    Brush* brush(void) const;
    Cap    cap(void) const;
    Joint  joint(void) const;
    int    dashArrayCount(void) const;
    double dash(int index) const;
    double offset(void) const;

public:
    void width(double width);
    void brush(const std::shared_ptr<Brush>& brush);
    void cap(Cap cap);
    void joint(Joint joint);
    void dashArray(std::initializer_list<double> il);
    void dashArray(const double* dash_array, unsigned int size);
    void offset(double offset);

    template <unsigned int Size>
    inline void dashArray(double (&dash_array)[Size])
    {
        dashArray(dash_array, Size);
    }

private:
    class Implement* impl_;
};

#endif // pen_h_20140118
