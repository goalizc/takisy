#ifndef dashed_line_h_20131029
#define dashed_line_h_20131029

#include <initializer_list>
#include <takisy/cgl/path/path.h>

class dashed_line
{
    class implement;

public:
    dashed_line(const double* dash_array, unsigned int size);
    dashed_line(const double* dash_array, unsigned int size, bool closed);
    dashed_line(const double* dash_array, unsigned int size, double offset);
    dashed_line(const double* dash_array, unsigned int size,
                bool closed, double offset);
    dashed_line(std::initializer_list<double> il);
    dashed_line(std::initializer_list<double> il, bool closed);
    dashed_line(std::initializer_list<double> il, double offset);
    dashed_line(std::initializer_list<double> il, bool closed, double offset);
    template <unsigned int Size>
    dashed_line(double (&dash_array)[Size])
        : dashed_line(dash_array, Size) {}
    template <unsigned int Size>
    dashed_line(double (&dash_array)[Size], bool closed)
        : dashed_line(dash_array, Size, closed) {}
    template <unsigned int Size>
    dashed_line(double (&dash_array)[Size], unsigned int offset)
        : dashed_line(dash_array, Size, offset) {}
    template <unsigned int Size>
    dashed_line(double (&dash_array)[Size], bool closed, unsigned int offset)
        : dashed_line(dash_array, Size, closed, offset) {}
    dashed_line(const dashed_line& dl);

   ~dashed_line(void);

    dashed_line& operator=(const dashed_line& dl);

public:
    template <unsigned int Size>
    inline dashed_line& dash_array(double (&_dash_array)[Size])
    {
        return dash_array(_dash_array, Size);
    }

    dashed_line& dash_array(std::initializer_list<double> il);
    dashed_line& dash_array(const double* dash_array, unsigned int size);
    dashed_line& dash(unsigned int index, double dash);
    dashed_line& closed(bool closed);
    dashed_line& offset(double offset);

    unsigned int dash_array_size(void) const;
    double dash(unsigned int index) const;
    bool closed(void) const;
    double offset(void) const;

public:
    const paths& build(const path& path);
    const paths& build(const path::vertex_type* vertices, unsigned int size);
    const paths& build(const paths& paths);
    const paths& build(const path* paths, unsigned int size);

private:
    class implement* impl_;
};

#endif //dashed_line_h_20131029
