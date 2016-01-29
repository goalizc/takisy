#ifndef pixel_format_h_20131106
#define pixel_format_h_20131106

#include <initializer_list>
#include <takisy/core/endian_type.h>
#include <takisy/cgl/basic/color.h>

enum ChannelType
{
    none = 0x0,

    G    = 0x1, //(G)ray
    r    = 0x2, //(r)ed
    g    = 0x3, //(g)reen
    b    = 0x4, //(b)lue
    a    = 0x8, //(a)lpha

    R    = 0xf, //(R)eserved
};

template <typename T = unsigned char,
          ChannelType _1st_channel = none,
          ChannelType _2nd_channel = none,
          ChannelType _3rd_channel = none,
          ChannelType _4th_channel = none>
class pixel_format
{
public:
    typedef T data_type;

public:
    pixel_format(void) = default;
    pixel_format(const pixel_format&) = default;

    pixel_format(const color& color)
    {
        operator=(color);
    }

    pixel_format(std::initializer_list<data_type> lst)
    {
        unsigned int i = 0;

        if (lst.size() < channels())
        {
            for ( ; i < lst.size(); ++i) data_[i] = lst.begin()[i];
            for ( ; i < channels(); ++i) data_[i] = 0;
        }
        else
            for ( ; i < channels(); ++i) data_[i] = lst.begin()[i];
    }

    ~pixel_format(void) = default;

public:
    pixel_format& operator=(const pixel_format&) = default;

    pixel_format& operator=(const color& color)
    {
        if (exist_G_channel())
            G(c2p(color.grayscale()));
        else
        {
            if (exist_r_channel()) r(c2p(color.r));
            if (exist_g_channel()) g(c2p(color.g));
            if (exist_b_channel()) b(c2p(color.b));
            if (exist_a_channel()) a(c2p(color.a));
        }

        return *this;
    }

public:
    static inline constexpr unsigned int flag(void)
    {
        return (channel_bits() << 24) & (channels()   << 16)
             & (_1st_channel   << 12) & (_2nd_channel <<  8)
             & (_3rd_channel   <<  4) & (_4th_channel <<  0);
    }

    static inline constexpr unsigned int channels(void)
    {
        return (_1st_channel != none ? 1 : 0) + (_2nd_channel != none ? 1 : 0)
             + (_3rd_channel != none ? 1 : 0) + (_4th_channel != none ? 1 : 0);
    }

    template <unsigned int index>
    static inline constexpr ChannelType channel_type(void)
    {
        return index == 0 ? _1st_channel : index == 1 ? _2nd_channel :
               index == 2 ? _3rd_channel : index == 3 ? _4th_channel : none;
    }

    static inline constexpr unsigned int channel_bytes(void)
    {
        return sizeof(data_type);
    }

    static inline constexpr unsigned int channel_bits(void)
    {
        return channel_bytes() * 8;
    }

    static inline constexpr unsigned int pixel_bytes(void)
    {
        return channels() * channel_bytes();
    }

    static inline constexpr unsigned int pixel_bits(void)
    {
        return channels() * channel_bits();
    }

    template <ChannelType channel_type>
    static inline constexpr bool exist_channel(void)
    {
        return _1st_channel == channel_type || _2nd_channel == channel_type
            || _3rd_channel == channel_type || _4th_channel == channel_type;
    }

#define EXIST_X_CHANNEL(channel_name)                               \
    static inline constexpr bool exist_##channel_name##_channel(void) \
    {                                                                 \
        return exist_channel<ChannelType::channel_name>();            \
    }

    EXIST_X_CHANNEL(G)
    EXIST_X_CHANNEL(r)
    EXIST_X_CHANNEL(g)
    EXIST_X_CHANNEL(b)
    EXIST_X_CHANNEL(a)
#undef EXIST_X_CHANNEL

    template <ChannelType channel_type>
    static inline constexpr int channel_index(void)
    {
        return _1st_channel == channel_type ? 0 :
               _2nd_channel == channel_type ? 1 :
               _3rd_channel == channel_type ? 2 :
               _4th_channel == channel_type ? 3 : -1;
    }

#define CHANNEL_INDEX(channel_name)                        \
    static inline constexpr int channel_name##_index(void) \
    {                                                      \
        return channel_index<ChannelType::channel_name>(); \
    }

    CHANNEL_INDEX(G)
    CHANNEL_INDEX(r)
    CHANNEL_INDEX(g)
    CHANNEL_INDEX(b)
    CHANNEL_INDEX(a)
#undef CHANNEL_INDEX

    inline data_type* data(void)
    {
        return data_;
    }

    inline const data_type* data(void) const
    {
        return data_;
    }

    template <ChannelType channel_type>
    inline void channel(unsigned int value)
    {
        if (exist_channel<channel_type>())
            data_[channel_index<channel_type>()] = value;
    }

#define SET_CHANNEL(channel_name)                  \
    inline void channel_name(unsigned int value)   \
    {                                              \
        channel<ChannelType::channel_name>(value); \
    }

    SET_CHANNEL(G)
    SET_CHANNEL(r)
    SET_CHANNEL(g)
    SET_CHANNEL(b)
    SET_CHANNEL(a)
#undef SET_CHANNEL

    template <ChannelType channel_type>
    inline unsigned int channel(void) const
    {
        return exist_channel<channel_type>()
            ? static_cast<unsigned int>(data_[channel_index<channel_type>()])
            : 0;
    }

#define GET_CHANNEL(channel_name)                    \
    inline unsigned int channel_name(void) const     \
    {                                                \
        return channel<ChannelType::channel_name>(); \
    }

    GET_CHANNEL(G)
    GET_CHANNEL(r)
    GET_CHANNEL(g)
    GET_CHANNEL(b)
    GET_CHANNEL(a)
#undef GET_CHANNEL

public:
    inline operator color(void) const
    {
        return exist_G_channel()
             ? color(p2c(G()),
                     exist_a_channel() ? p2c(a()) : color::channel_mask)
             : color(p2c(r()), p2c(g()), p2c(b()),
                     exist_a_channel() ? p2c(a()) : color::channel_mask);
    }

    inline color to_color(void) const
    {
        return operator color();
    }

    inline color::channel_type grayscale(void) const
    {
        return exist_G_channel()
             ? p2c(G()) : p2c((r() + g() + g() + b()) >> 2);
    }

    inline color grayscale_color(void) const
    {
        return color(grayscale(), a());
    }

public:
    inline void grayscale(color::channel_type grayscale)
    {
        if (exist_G_channel())
            G(c2p(grayscale));
        else
        {
            if (exist_r_channel()) r(c2p(grayscale));
            if (exist_g_channel()) g(c2p(grayscale));
            if (exist_b_channel()) b(c2p(grayscale));
        }
    }

    inline void premultiply(void)
    {
        if (exist_a_channel())
        {
            unsigned int inc_a = a() + 1;

            if (exist_G_channel())
                G((G() * inc_a) >> channel_bits());
            else
            {
                if (exist_r_channel()) r((r() * inc_a) >> channel_bits());
                if (exist_g_channel()) g((g() * inc_a) >> channel_bits());
                if (exist_b_channel()) b((b() * inc_a) >> channel_bits());
            }
        }
    }

    inline void blend(const color& color)
    {
        blend_alpha(color, color.a);
    }

    inline void blend(const color& color, color::channel_type coverage)
    {
        blend_alpha(color, (color.a * (coverage + 1)) >> color::channel_shift);
    }

private:
    inline void blend_alpha(const color& color, color::channel_type alpha)
    {
        if (exist_a_channel())
        {
            if (alpha == 0)
                return;
            else
            if (alpha == color::channel_mask)
                return (void)operator=(color);

            constexpr double p_mask  = (1 << channel_bits()) - 1;
            constexpr double c_mask  = color::channel_mask;
            register  double p_alpha = a()   / p_mask;
            register  double c_alpha = alpha / c_mask;
            register  double _1      = p_alpha - c_alpha * p_alpha;
            register  double _2      = c_alpha + _1;
            register  double _3      = c_alpha / c_mask;
            register  double _4      = _1 / p_mask;
            register  double _5      = _2 / p_mask;

            #define BLEND_CHANNEL(p, c) p((c * _3 + p() * _4) / _5)

            if (exist_G_channel())
                BLEND_CHANNEL(G, color.grayscale());
            else
            {
                if (exist_r_channel()) BLEND_CHANNEL(r, color.r);
                if (exist_g_channel()) BLEND_CHANNEL(g, color.g);
                if (exist_b_channel()) BLEND_CHANNEL(b, color.b);
            }

            a(_2 * p_mask);

            #undef BLEND_CHANNEL
        }
        else
        {
            #define BLEND_CHANNEL(p, c) \
                p(p() + ((alpha * (c2p(c) - p())) >> color::channel_shift))

            if (exist_G_channel())
                BLEND_CHANNEL(G, color.grayscale());
            else
            {
                if (exist_r_channel()) BLEND_CHANNEL(r, color.r);
                if (exist_g_channel()) BLEND_CHANNEL(g, color.g);
                if (exist_b_channel()) BLEND_CHANNEL(b, color.b);
            }

            #undef BLEND_CHANNEL
        }
    }

    template <int Shift>
    static inline unsigned int shift(unsigned int value)
    {
        return Shift == 0 ? (value)
             : Shift  < 0 ? (value >> -Shift)
                          : (value <<  Shift);
    }

    static inline unsigned int p2c(unsigned int value)
    {
        return shift<color::channel_shift - channel_bits()>(value);
    }

    static inline unsigned int c2p(unsigned int value)
    {
        return shift<channel_bits() - color::channel_shift>(value);
    }

private:
    data_type data_[channels()];
};

typedef pixel_format<etlc_uint8, G>          pf_G8;
typedef pixel_format<etlc_uint8, G, a>       pf_Ga8;
typedef pixel_format<etlc_uint8, a, G>       pf_aG8;
typedef pixel_format<etlc_uint8, r, g, b>    pf_rgb8;
typedef pixel_format<etlc_uint8, b, g, r>    pf_bgr8;
typedef pixel_format<etlc_uint8, r, g, b, a> pf_rgba8;
typedef pixel_format<etlc_uint8, b, g, r, a> pf_bgra8;
typedef pixel_format<etlc_uint8, a, r, g, b> pf_argb8;
typedef pixel_format<etlc_uint8, a, b, g, r> pf_abgr8;
typedef pixel_format<etlc_uint8, a>          pf_a8, pf_mask8;

typedef pixel_format<etbe_uint8, G>          pf_G8be;
typedef pixel_format<etbe_uint8, G, a>       pf_Ga8be;
typedef pixel_format<etbe_uint8, a, G>       pf_aG8be;
typedef pixel_format<etbe_uint8, r, g, b>    pf_rgb8be;
typedef pixel_format<etbe_uint8, b, g, r>    pf_bgr8be;
typedef pixel_format<etbe_uint8, r, g, b, a> pf_rgba8be;
typedef pixel_format<etbe_uint8, b, g, r, a> pf_bgra8be;
typedef pixel_format<etbe_uint8, a, r, g, b> pf_argb8be;
typedef pixel_format<etbe_uint8, a, b, g, r> pf_abgr8be;
typedef pixel_format<etbe_uint8, a>          pf_a8be, pf_mask8be;

typedef pixel_format<etle_uint8, G>          pf_G8le;
typedef pixel_format<etle_uint8, G, a>       pf_Ga8le;
typedef pixel_format<etle_uint8, a, G>       pf_aG8le;
typedef pixel_format<etle_uint8, r, g, b>    pf_rgb8le;
typedef pixel_format<etle_uint8, b, g, r>    pf_bgr8le;
typedef pixel_format<etle_uint8, r, g, b, a> pf_rgba8le;
typedef pixel_format<etle_uint8, b, g, r, a> pf_bgra8le;
typedef pixel_format<etle_uint8, a, r, g, b> pf_argb8le;
typedef pixel_format<etle_uint8, a, b, g, r> pf_abgr8le;
typedef pixel_format<etle_uint8, a>          pf_a8le, pf_mask8le;

typedef pixel_format<etlc_uint16, G>          pf_G16;
typedef pixel_format<etlc_uint16, G, a>       pf_Ga16;
typedef pixel_format<etlc_uint16, a, G>       pf_aG16;
typedef pixel_format<etlc_uint16, r, g, b>    pf_rgb16;
typedef pixel_format<etlc_uint16, b, g, r>    pf_bgr16;
typedef pixel_format<etlc_uint16, r, g, b, a> pf_rgba16;
typedef pixel_format<etlc_uint16, b, g, r, a> pf_bgra16;
typedef pixel_format<etlc_uint16, a, r, g, b> pf_argb16;
typedef pixel_format<etlc_uint16, a, b, g, r> pf_abgr16;
typedef pixel_format<etlc_uint16, a>          pf_a16, pf_mask16;

typedef pixel_format<etbe_uint16, G>          pf_G16be;
typedef pixel_format<etbe_uint16, G, a>       pf_Ga16be;
typedef pixel_format<etbe_uint16, a, G>       pf_aG16be;
typedef pixel_format<etbe_uint16, r, g, b>    pf_rgb16be;
typedef pixel_format<etbe_uint16, b, g, r>    pf_bgr16be;
typedef pixel_format<etbe_uint16, r, g, b, a> pf_rgba16be;
typedef pixel_format<etbe_uint16, b, g, r, a> pf_bgra16be;
typedef pixel_format<etbe_uint16, a, r, g, b> pf_argb16be;
typedef pixel_format<etbe_uint16, a, b, g, r> pf_abgr16be;
typedef pixel_format<etbe_uint16, a>          pf_a16be, pf_mask16be;

typedef pixel_format<etle_uint16, G>          pf_G16le;
typedef pixel_format<etle_uint16, G, a>       pf_Ga16le;
typedef pixel_format<etle_uint16, a, G>       pf_aG16le;
typedef pixel_format<etle_uint16, r, g, b>    pf_rgb16le;
typedef pixel_format<etle_uint16, b, g, r>    pf_bgr16le;
typedef pixel_format<etle_uint16, r, g, b, a> pf_rgba16le;
typedef pixel_format<etle_uint16, b, g, r, a> pf_bgra16le;
typedef pixel_format<etle_uint16, a, r, g, b> pf_argb16le;
typedef pixel_format<etle_uint16, a, b, g, r> pf_abgr16le;
typedef pixel_format<etle_uint16, a>          pf_a16le, pf_mask16le;

#endif //pixel_format_h_20131106
