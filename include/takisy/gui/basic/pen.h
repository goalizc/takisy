#ifndef pen_h_20150715
#define pen_h_20150715

#include <initializer_list>
#include <takisy/cgl/basic/color.h>
#include <takisy/gui/basic/brush.h>

class pen
{
    friend class graphics;
    class implement;

public:
    enum Cap
        { cButt, cSquare, cRound };
    enum Joint
        { jBevel, jRound, jMiter };

public:
    pen(void);
    explicit pen(double width);
    pen(const color& color);
    pen(const brush_sptr& brush);
    pen(double width, const color& color);
    pen(double width, const brush_sptr& brush);
    pen(const pen& pen);
   ~pen(void);

    pen& operator=(const pen& pen);

public:
    double width(void) const;
    brush* brush(void) const;
    Cap    cap(void) const;
    Joint  joint(void) const;
    int    dash_array_size(void) const;
    double dash(int index) const;
    double offset(void) const;

public:
    void width(double width);
    void color(const color& color);
    void brush(const brush_sptr& brush);
    void cap(Cap cap);
    void joint(Joint joint);
    void dash_array(std::initializer_list<double> il);
    void dash_array(const double* dash_array, unsigned int size);
    void offset(double offset);

    template <unsigned int Size>
    inline void dash_array(double (&dash_array)[Size])
    {
        dash_array(dash_array, Size);
    }

private:
    class implement* impl_;
};

#endif // pen_h_20150715
