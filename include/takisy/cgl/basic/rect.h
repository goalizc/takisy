#ifndef rect_h_20130722
#define rect_h_20130722

#include <cmath>
#include <algorithm>
#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/basic/size.h>

template <typename T>
struct rect_type
{
    typedef rect_type self_type;
    typedef T         value_type;

    value_type left, top, right, bottom;

public:
    inline rect_type(void)
        : left(0), top(0), right(0), bottom(0)
    {}

    inline rect_type(value_type width, value_type height)
        : rect_type(0, 0, width, height)
    {}

    inline rect_type(value_type left,  value_type top,
                  value_type right, value_type bottom)
        : left(left), top(top), right(right), bottom(bottom)
    {}

    template <typename TT>
    inline rect_type(const size_type<TT>& size)
        : rect_type(size.width, size.height)
    {}

    template <typename TT, typename TTT>
    inline rect_type(const point_type<TT>& lt, const point_type<TTT>& rb)
        : left(lt.x), top(lt.y), right(rb.x), bottom(rb.y)
    {}

    template <typename TT, typename TTT>
    inline rect_type(const point_type<TT>& p, const size_type<TTT>& sz)
        : left(p.x), top(p.y), right(p.x + sz.width), bottom(p.y + sz.height)
    {}

    template <typename Rect>
    inline rect_type(const Rect& rect)
        : left(rect.left), top(rect.top), right(rect.right), bottom(rect.bottom)
    {}

    inline self_type& operator=(const self_type& rect)
    {
        if (this != &rect)
        {
            left   = rect.left;
            top    = rect.top;
            right  = rect.right;
            bottom = rect.bottom;
        }

        return *this;
    }

    template <typename Rect>
    inline self_type& operator=(const Rect& rect)
    {
        left   = rect.left;
        top    = rect.top;
        right  = rect.right;
        bottom = rect.bottom;

        return *this;
    }

public:
    template <typename TT>
    inline bool operator==(const rect_type<TT>& rect) const
    {
        return left == rect.left && right  == rect.right
            && top  == rect.top  && bottom == rect.bottom;
    }

    template <typename TT>
    inline bool operator!=(const rect_type<TT>& rect) const
    {
        return !operator==(rect);
    }

public:
    inline point_type<value_type> left_top(void) const
    {
        return point_type<value_type>(left, top);
    }

    inline point_type<value_type> left_bottom(void) const
    {
        return point_type<value_type>(left, bottom);
    }

    inline point_type<value_type> right_top(void) const
    {
        return point_type<value_type>(right, top);
    }

    inline point_type<value_type> right_bottom(void) const
    {
        return point_type<value_type>(right, bottom);
    }

    inline point_type<value_type> center(void) const
    {
        return point_type<value_type>((left + right) / 2, (top + bottom) / 2);
    }

    inline value_type width(void) const
    {
        return right - left;
    }

    inline value_type height(void) const
    {
        return bottom - top;
    }

    inline size_type<value_type> size(void) const
    {
        return size_type<value_type>(width(), height());
    }

public:
    inline bool empty(void) const
    {
        return left == right || top == bottom;
    }

    template <typename TT>
    inline bool inside(const point_type<TT>& point) const
    {
        return inside(point.x, point.y);
    }

    inline bool inside(value_type x, value_type y) const
    {
        return left <= x && x < right && top <= y && y < bottom;
    }

    template <typename TT>
    inline bool outside(const point_type<TT>& point) const
    {
        return outside(point.x, point.y);
    }

    inline bool outside(value_type x, value_type y) const
    {
        return !inside(x, y);
    }

    template <typename TT>
    inline bool intersectant(const rect_type<TT>& rect) const
    {
        point_type<value_type> &&c = center(), &&rc = rect.center();

        return abs(c.x - rc.x) * 2 < width()  + rect.width()
            && abs(c.y - rc.y) * 2 < height() + rect.height();
    }

public:
    template <typename Point>
    inline void left_top(const Point& lefttop)
    {
        left_top(lefttop.x, lefttop.y);
    }

    template <typename Point>
    inline void left_bottom(const Point& leftbottom)
    {
        left_bottom(leftbottom.x, leftbottom.y);
    }

    template <typename Point>
    inline void right_top(const Point& righttop)
    {
        right_top(righttop.x, righttop.y);
    }

    template <typename Point>
    inline void right_bottom(const Point& rightbottom)
    {
        right_bottom(rightbottom.x, rightbottom.y);
    }

    inline void left_top(value_type _left, value_type _top)
    {
        left = _left;
        top  = _top;
    }

    inline void left_bottom(value_type _left, value_type _bottom)
    {
        left   = _left;
        bottom = _bottom;
    }

    inline void right_top(value_type _right, value_type _top)
    {
        right = _right;
        top   = _top;
    }

    inline void right_bottom(value_type _right, value_type _bottom)
    {
        right  = _right;
        bottom = _bottom;
    }

    inline void width(value_type width)
    {
        right = left + width;
    }

    inline void height(value_type height)
    {
        bottom = top + height;
    }

    template <typename Size>
    inline void size(const Size& _size)
    {
        size(_size.width, _size.height);
    }

    inline void size(value_type _width, value_type _height)
    {
        width(_width);
        height(_height);
    }

    template <typename Point>
    inline void center(const Point& center)
    {
        *this = offset(center - center());
    }

    inline void center(value_type x, value_type y)
    {
        center(point_type<value_type>(x, y));
    }

public:
    template <typename TT>
    inline self_type move(const point_type<TT>& point) const
    {
        return move(point.x, point.y);
    }

    inline self_type move(value_type x, value_type y) const
    {
        return offset(x - left, y - top);
    }

    template <typename TT>
    inline self_type offset(const point_type<TT>& _offset) const
    {
        return offset(_offset.x, _offset.y);
    }

    inline self_type offset(value_type dx, value_type dy) const
    {
        return self_type(left + dx, top + dy, right + dx, bottom + dy);
    }

    inline self_type inflate(value_type d) const
    {
        return inflate(d, d);
    }

    inline self_type inflate(value_type dx, value_type dy) const
    {
        return self_type(left - dx, top - dy, right + dx, bottom + dy);
    }

    inline self_type expand(value_type d) const
    {
        return expand(d, d);
    }

    inline self_type expand(value_type dx, value_type dy) const
    {
        return self_type(left, top, right + dx, bottom + dy);
    }

    template <typename TT>
    inline self_type intersect(const rect_type<TT>& rect) const
    {
        if (!intersectant(rect))
            return self_type(0, 0, 0, 0);

        self_type nthis = normalize();
        self_type nrect = rect.normalize();

        return self_type(std::max(nthis.left,   nrect.left),
                         std::max(nthis.top,    nrect.top),
                         std::min(nthis.right,  nrect.right),
                         std::min(nthis.bottom, nrect.bottom));
    }

    template <typename TT>
    inline self_type unit(const rect_type<TT>& rect) const
    {
        if (empty())
            return rect;
        else if (rect.empty())
            return *this;

        self_type nthis = normalize();
        self_type nrect = rect.normalize();

        return self_type(std::min(nthis.left,   nrect.left),
                         std::min(nthis.top,    nrect.top),
                         std::max(nthis.right,  nrect.right),
                         std::max(nthis.bottom, nrect.bottom));
    }

    inline self_type normalize(void) const
    {
        self_type rect = *this;

        if (rect.left > rect.right)
            std::swap(rect.left, rect.right);
        if (rect.top  > rect.bottom)
            std::swap(rect.top,  rect.bottom);

        return rect;
    }

public:
    template <typename TT>
    inline self_type operator+(const point_type<TT>& _offset)
    {
        return offset(_offset);
    }

    template <typename TT>
    inline self_type operator-(const point_type<TT>& _offset)
    {
        return offset(-_offset);
    }

    template <typename TT>
    inline self_type operator+(const size_type<TT>& size)
    {
        return self_type(left, top, right + size.width, bottom + size.height);
    }

    template <typename TT>
    inline self_type operator-(const size_type<TT>& size)
    {
        return self_type(left, top, right - size.width, bottom - size.height);
    }

    template <typename TT>
    inline self_type operator+=(const point_type<TT>& offset)
    {
        left  += offset.x; top    += offset.y;
        right += offset.x; bottom += offset.y;

        return *this;
    }

    template <typename TT>
    inline self_type operator-=(const point_type<TT>& offset)
    {
        left  -= offset.x; top    -= offset.y;
        right -= offset.x; bottom -= offset.y;

        return *this;
    }

    template <typename TT>
    inline self_type operator+=(const size_type<TT>& size)
    {
        right  += size.width;
        bottom += size.height;

        return *this;
    }

    template <typename TT>
    inline self_type operator-=(const size_type<TT>& size)
    {
        right  -= size.width;
        bottom -= size.height;

        return *this;
    }
};

typedef rect_type<int>          rect;
typedef rect_type<unsigned int> rectu;
typedef rect_type<double>       rectf;

#endif //rect_h_20130722
