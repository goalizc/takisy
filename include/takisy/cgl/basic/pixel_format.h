#ifndef pixel_format_h_20131106
#define pixel_format_h_20131106

#include <array>
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
    static constexpr unsigned int kChannels
            = !!_1st_channel + !!_2nd_channel + !!_3rd_channel + !!_4th_channel;

public:
    typedef T value_type;
    typedef std::array<value_type, kChannels> data_type;

public:
    pixel_format(void) = default;
    pixel_format(const pixel_format&) = default;

    pixel_format(const color& color)
    {
        operator=(color);
    }

    pixel_format(std::initializer_list<value_type> lst)
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
        }

        if (exist_a_channel())
            a(c2p(color.a));

        return *this;
    }

public:
    static inline constexpr unsigned int flag(void)
    {
        return (channel_bits() << 24) | (channels()   << 16)
             | (_1st_channel   << 12) | (_2nd_channel <<  8)
             | (_3rd_channel   <<  4) | (_4th_channel <<  0);
    }

    static inline constexpr unsigned int channels(void)
    {
        return kChannels;
    }

    template <unsigned int index>
    static inline constexpr ChannelType channel_type(void)
    {
        return index == 0 ? _1st_channel : index == 1 ? _2nd_channel :
               index == 2 ? _3rd_channel : index == 3 ? _4th_channel : none;
    }

    static inline constexpr unsigned int channel_bytes(void)
    {
        return sizeof(value_type);
    }

    static inline constexpr unsigned int channel_bits(void)
    {
        return channel_bytes() * 8;
    }

    static inline constexpr unsigned int channel_scale(void)
    {
        return 1 << channel_bits();
    }

    static inline constexpr unsigned int channel_mask(void)
    {
        return channel_scale() - 1;
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

#define EXIST_X_CHANNEL(channel_name)                                 \
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

#define CHANNEL_INDEX(channel_name)                                  \
    static inline constexpr int channel_##channel_name##_index(void) \
    {                                                                \
        return channel_index<ChannelType::channel_name>();           \
    }

    CHANNEL_INDEX(G)
    CHANNEL_INDEX(r)
    CHANNEL_INDEX(g)
    CHANNEL_INDEX(b)
    CHANNEL_INDEX(a)
#undef CHANNEL_INDEX

public:
    inline data_type& data(void)
    {
        return data_;
    }

    inline const data_type& data(void) const
    {
        return data_;
    }

    template <ChannelType channel_type>
    inline void channel(unsigned int value)
    {
        if (exist_channel<channel_type>())
            data_[channel_index<channel_type>()] = value;
    }

    inline void grayscale(unsigned int grayscale)
    {
        if (exist_G_channel())
            G(grayscale);
        else
        {
            if (exist_r_channel()) r(grayscale);
            if (exist_g_channel()) g(grayscale);
            if (exist_b_channel()) b(grayscale);
        }
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

    template <ChannelType channel_type, unsigned int default_value = 0>
    inline unsigned int channel(void) const
    {
        return exist_channel<channel_type>()
             ? static_cast<unsigned int>(data_[channel_index<channel_type>()])
             : default_value;
    }

    inline unsigned int grayscale(void) const
    {
        static constexpr unsigned int chs = channels() - exist_a_channel();

        return exist_G_channel() ? G() : ((r() + g() + b()) / chs);
    }

#define GET_CHANNEL(channel_name, default_value)                    \
    inline unsigned int channel_name(void) const                    \
    {                                                               \
        return channel<ChannelType::channel_name, default_value>(); \
    }

    GET_CHANNEL(G, 0)
    GET_CHANNEL(r, 0)
    GET_CHANNEL(g, 0)
    GET_CHANNEL(b, 0)
    GET_CHANNEL(a, channel_mask())
#undef GET_CHANNEL

public:
    inline operator color(void) const
    {
        return exist_G_channel()
             ? color(p2c(G()), p2c(a()))
             : color(p2c(r()), p2c(g()), p2c(b()), p2c(a()));
    }

public:
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

            constexpr double pm = channel_mask();      // pixel mask
            constexpr double cm = color::channel_mask; // color mask
                      double pa = a()   / pm;          // pixel alpha
                      double ca = alpha / cm;          // color alpha
                      double _1 = pa - ca * pa;
                      double _2 = ca + _1;
                      double _3 = ca / cm;
                      double _4 = _1 / pm;
                      double _5 = _2 / pm;

            #define BLEND_CHANNEL(p, c) p((c * _3 + p() * _4) / _5)

            if (exist_G_channel())
                BLEND_CHANNEL(G, color.grayscale());
            else
            {
                if (exist_r_channel()) BLEND_CHANNEL(r, color.r);
                if (exist_g_channel()) BLEND_CHANNEL(g, color.g);
                if (exist_b_channel()) BLEND_CHANNEL(b, color.b);
            }

            a(_2 * pm);

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

private:
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
    data_type data_;
};

typedef pixel_format<etlc_uint8, G>          pixfmt_G8;
typedef pixel_format<etlc_uint8, G, a>       pixfmt_Ga8;
typedef pixel_format<etlc_uint8, a, G>       pixfmt_aG8;
typedef pixel_format<etlc_uint8, r, g, b>    pixfmt_rgb8;
typedef pixel_format<etlc_uint8, b, g, r>    pixfmt_bgr8;
typedef pixel_format<etlc_uint8, r, g, b, a> pixfmt_rgba8;
typedef pixel_format<etlc_uint8, b, g, r, a> pixfmt_bgra8;
typedef pixel_format<etlc_uint8, a, r, g, b> pixfmt_argb8;
typedef pixel_format<etlc_uint8, a, b, g, r> pixfmt_abgr8;
typedef pixel_format<etlc_uint8, a>          pixfmt_a8;
typedef pixfmt_a8                            pixfmt_mask8;

typedef pixel_format<etbe_uint8, G>          pixfmt_G8be;
typedef pixel_format<etbe_uint8, G, a>       pixfmt_Ga8be;
typedef pixel_format<etbe_uint8, a, G>       pixfmt_aG8be;
typedef pixel_format<etbe_uint8, r, g, b>    pixfmt_rgb8be;
typedef pixel_format<etbe_uint8, b, g, r>    pixfmt_bgr8be;
typedef pixel_format<etbe_uint8, r, g, b, a> pixfmt_rgba8be;
typedef pixel_format<etbe_uint8, b, g, r, a> pixfmt_bgra8be;
typedef pixel_format<etbe_uint8, a, r, g, b> pixfmt_argb8be;
typedef pixel_format<etbe_uint8, a, b, g, r> pixfmt_abgr8be;
typedef pixel_format<etbe_uint8, a>          pixfmt_a8be;
typedef pixfmt_a8be                          pixfmt_mask8be;

typedef pixel_format<etle_uint8, G>          pixfmt_G8le;
typedef pixel_format<etle_uint8, G, a>       pixfmt_Ga8le;
typedef pixel_format<etle_uint8, a, G>       pixfmt_aG8le;
typedef pixel_format<etle_uint8, r, g, b>    pixfmt_rgb8le;
typedef pixel_format<etle_uint8, b, g, r>    pixfmt_bgr8le;
typedef pixel_format<etle_uint8, r, g, b, a> pixfmt_rgba8le;
typedef pixel_format<etle_uint8, b, g, r, a> pixfmt_bgra8le;
typedef pixel_format<etle_uint8, a, r, g, b> pixfmt_argb8le;
typedef pixel_format<etle_uint8, a, b, g, r> pixfmt_abgr8le;
typedef pixel_format<etle_uint8, a>          pixfmt_a8le;
typedef pixfmt_a8le                          pixfmt_mask8le;

typedef pixel_format<etlc_uint16, G>          pixfmt_G16;
typedef pixel_format<etlc_uint16, G, a>       pixfmt_Ga16;
typedef pixel_format<etlc_uint16, a, G>       pixfmt_aG16;
typedef pixel_format<etlc_uint16, r, g, b>    pixfmt_rgb16;
typedef pixel_format<etlc_uint16, b, g, r>    pixfmt_bgr16;
typedef pixel_format<etlc_uint16, r, g, b, a> pixfmt_rgba16;
typedef pixel_format<etlc_uint16, b, g, r, a> pixfmt_bgra16;
typedef pixel_format<etlc_uint16, a, r, g, b> pixfmt_argb16;
typedef pixel_format<etlc_uint16, a, b, g, r> pixfmt_abgr16;
typedef pixel_format<etlc_uint16, a>          pixfmt_a16;
typedef pixfmt_a16                            pixfmt_mask16;

typedef pixel_format<etbe_uint16, G>          pixfmt_G16be;
typedef pixel_format<etbe_uint16, G, a>       pixfmt_Ga16be;
typedef pixel_format<etbe_uint16, a, G>       pixfmt_aG16be;
typedef pixel_format<etbe_uint16, r, g, b>    pixfmt_rgb16be;
typedef pixel_format<etbe_uint16, b, g, r>    pixfmt_bgr16be;
typedef pixel_format<etbe_uint16, r, g, b, a> pixfmt_rgba16be;
typedef pixel_format<etbe_uint16, b, g, r, a> pixfmt_bgra16be;
typedef pixel_format<etbe_uint16, a, r, g, b> pixfmt_argb16be;
typedef pixel_format<etbe_uint16, a, b, g, r> pixfmt_abgr16be;
typedef pixel_format<etbe_uint16, a>          pixfmt_a16be;
typedef pixfmt_a16be                          pixfmt_mask16be;

typedef pixel_format<etle_uint16, G>          pixfmt_G16le;
typedef pixel_format<etle_uint16, G, a>       pixfmt_Ga16le;
typedef pixel_format<etle_uint16, a, G>       pixfmt_aG16le;
typedef pixel_format<etle_uint16, r, g, b>    pixfmt_rgb16le;
typedef pixel_format<etle_uint16, b, g, r>    pixfmt_bgr16le;
typedef pixel_format<etle_uint16, r, g, b, a> pixfmt_rgba16le;
typedef pixel_format<etle_uint16, b, g, r, a> pixfmt_bgra16le;
typedef pixel_format<etle_uint16, a, r, g, b> pixfmt_argb16le;
typedef pixel_format<etle_uint16, a, b, g, r> pixfmt_abgr16le;
typedef pixel_format<etle_uint16, a>          pixfmt_a16le;
typedef pixfmt_a16le                          pixfmt_mask16le;

#endif //pixel_format_h_20131106
