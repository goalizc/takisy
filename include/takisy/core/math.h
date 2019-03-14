#ifndef math_h_20130815
#define math_h_20130815

#include <cmath>
#include <algorithm>

class math
{
public:
    static constexpr double pi = 3.14159265358979323846264338327950;
    static constexpr double e  = 2.71828182845904523536028747135266;

public:
    static inline unsigned int seed(unsigned int seed)
    {
        std::swap(seed_, seed);

        return seed;
    }

    static inline int rand(void)
    {
        return (seed_ = (seed_ * 16807ull + 0) % 2147483647);
    }

    static inline int rand(int upper_bound)
    {
        return rand() % upper_bound;
    }

    static inline int rand(int lower_bound, int upper_bound)
    {
        return rand() % (upper_bound - lower_bound) + lower_bound;
    }

    static inline double frand(void)
    {
        return rand() * 4.656612875245797e-10;
    }

    static inline double frand(double upper_bound)
    {
        return frand() * upper_bound;
    }

    static inline double frand(double lower_bound, double upper_bound)
    {
        return frand() * (upper_bound - lower_bound) + lower_bound;
    }

public:
    template <typename T>
    static inline T abs(const T& n)
    {
        return n >= 0 ? n : -n;
    }

    static inline double ceil(double n)
    {
        return ::ceil(n);
    }

    static inline double floor(double n)
    {
        return ::floor(n);
    }

    static inline double fmod(double n, double m)
    {
        return ::fmod(n, m);
    }

public:
    static inline double exp(double n)
    {
        return ::exp(n);
    }

    static inline double exp2(double n)
    {
        return ::exp2(n);
    }

    static inline double log(double n)
    {
        return ::log(n);
    }

    static inline double log2(double n)
    {
        return ::log2(n);
    }

    static inline double log10(double n)
    {
        return ::log10(n);
    }

    template <typename BaseType, typename ExponentType>
    static inline BaseType pow(BaseType base, ExponentType exponent)
    {
        return ::pow(base, exponent);
    }

    template <typename T>
    static inline T sqr(T n)
    {
        return n * n;
    }

    static inline float sqrtf(float n)
    {
        return ::sqrtf(n);
    }

    static inline double sqrt(double n)
    {
        return ::sqrt(n);
    }

    static inline double cbrt(double n)
    {
        return ::cbrt(n);
    }

    template <typename T>
    static inline double hypot(T x, T y)
    {
    #if 0
        return ::hypot(x, y);
    #else
        return sqrt(x * x + y * y);
    #endif
    }

    template <typename PointType>
    static inline double hypot(const PointType& point)
    {
        return hypot(point.x, point.y);
    }

public:
    template <int rad>
    static inline constexpr double rad2deg(void)
    {
        return rad * (180 / pi);
    }

    template <int deg>
    static inline constexpr double deg2rad(void)
    {
        return deg * (pi / 180);
    }

    static inline double rad2deg(double rad)
    {
        return rad * (180 / pi);
    }

    static inline double deg2rad(double deg)
    {
        return deg * (pi / 180);
    }

    static inline double sin(double angle)
    {
        return ::sin(angle);
    }

    static inline double cos(double angle)
    {
        return ::cos(angle);
    }

    static inline double tan(double angle)
    {
        return ::tan(angle);
    }

    static inline double asin(double fnval)
    {
        return ::asin(fnval);
    }

    static inline double acos(double fnval)
    {
        return ::acos(fnval);
    }

    static inline double atan(double fnval)
    {
        return ::atan(fnval);
    }

    static inline double atan2(double x, double y)
    {
        return ::atan2(y, x);
    }

    template <typename PointType>
    static inline double atan2(const PointType& point)
    {
        return atan2(point.x, point.y);
    }

public:
    template <typename FloatType>
    static inline bool isnan(FloatType n)
    {
        return std::isnan(n);
    }

    template <typename FloatType>
    static inline bool isinf(FloatType n)
    {
        return std::isinf(n);
    }

private:
    static unsigned int seed_;
};

#endif //math_h_20130815
