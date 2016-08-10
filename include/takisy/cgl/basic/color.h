#ifndef color_h_20130717
#define color_h_20130717

#include <takisy/core/math.h>

struct color
{
    typedef unsigned char channel_type;

    enum
    {
        channel_shift = sizeof(channel_type) * 8,
        channel_scale = 1 << channel_shift,
        channel_mask  = channel_scale - 1,
    };

    channel_type b, g, r, a;

public:
    color(void)
        : b(0), g(0), r(0), a(0)
    {}

    explicit
    color(channel_type grayscale)
        : b(grayscale), g(grayscale), r(grayscale), a(channel_mask)
    {}

    color(channel_type grayscale, channel_type a)
        : b(grayscale), g(grayscale), r(grayscale), a(a)
    {}

    color(channel_type r, channel_type g, channel_type b)
        : b(b), g(g), r(r), a(channel_mask)
    {}

    color(channel_type r, channel_type g, channel_type b, channel_type a)
        : b(b), g(g), r(r), a(a)
    {}

    color(const color& c)
        : color(c.r, c.g, c.b, c.a)
    {}

    ~color(void)
    {}

public:
    inline color& operator=(const color& c)
    {
        r = c.r; g = c.g; b = c.b; a = c.a;

        return *this;
    }

public:
    static inline color rgb(unsigned int rgb)
    {
        return argb(0xff000000 | rgb);
    }

    static inline color argb(unsigned int argb)
    {
        return color((argb & 0x00ff0000) >> 16, (argb & 0x0000ff00) >>  8,
                     (argb & 0x000000ff),       (argb & 0xff000000) >> 24);
    }

    static inline color rgba(unsigned int rgba)
    {
        return color((rgba & 0xff000000) >> 24, (rgba & 0x00ff0000) >> 16,
                     (rgba & 0x0000ff00) >>  8, (rgba & 0x000000ff));
    }

    static inline color rand_rgb(void)
    {
        return rand_rgb(channel_mask);
    }

    static inline color rand_rgb(channel_type a)
    {
        return color(math::rand(channel_scale),
                     math::rand(channel_scale),
                     math::rand(channel_scale), a);
    }

    static inline color rand_rgba(void)
    {
        return rand_rgb(math::rand(channel_scale));
    }

#define ALIAS(alias, r, g, b)                                               \
    static inline color alias(void)           { return color(r, g, b);    } \
    static inline color alias(channel_type a) { return color(r, g, b, a); }

    ALIAS(alice_blue,              0xf0, 0xf8, 0xff)
    ALIAS(antique_white,           0xfa, 0xeb, 0xd7)
    ALIAS(aqua,                    0x00, 0xff, 0xff)
    ALIAS(aquamarine,              0x7f, 0xff, 0xd4)
    ALIAS(azure,                   0xf0, 0xff, 0xff)
    ALIAS(beige,                   0xf5, 0xf5, 0xdc)
    ALIAS(bisque,                  0xff, 0xe4, 0xc4)
    ALIAS(black,                   0x00, 0x00, 0x00)
    ALIAS(blanched_almond,         0xff, 0xeb, 0xcd)
    ALIAS(blue,                    0x00, 0x00, 0xff)
    ALIAS(blue_violet,             0x8a, 0x2b, 0xe2)
    ALIAS(brown,                   0xa5, 0x2a, 0x2a)
    ALIAS(burly_wood,              0xde, 0xb8, 0x87)
    ALIAS(cadet_blue,              0x5f, 0x9e, 0xa0)
    ALIAS(chartreuse,              0x7f, 0xff, 0x00)
    ALIAS(chocolate,               0xd2, 0x69, 0x1e)
    ALIAS(coral,                   0xff, 0x7f, 0x50)
    ALIAS(cornflower_blue,         0x64, 0x95, 0xed)
    ALIAS(cornsilk,                0xff, 0xf8, 0xdc)
    ALIAS(crimson,                 0xdc, 0x14, 0x3c)
    ALIAS(cyan,                    0x00, 0xff, 0xff)
    ALIAS(dark_blue,               0x00, 0x00, 0x8b)
    ALIAS(dark_cyan,               0x00, 0x8b, 0x8b)
    ALIAS(dark_golden_rod,         0xb8, 0x86, 0x0b)
    ALIAS(dark_gray,               0xa9, 0xa9, 0xa9)
    ALIAS(dark_green,              0x00, 0x64, 0x00)
    ALIAS(dark_khaki,              0xbd, 0xb7, 0x6b)
    ALIAS(dark_magenta,            0x8b, 0x00, 0x8b)
    ALIAS(dark_olive_green,        0x55, 0x6b, 0x2f)
    ALIAS(darkorange,              0xff, 0x8c, 0x00)
    ALIAS(dark_orchid,             0x99, 0x32, 0xcc)
    ALIAS(dark_red,                0x8b, 0x00, 0x00)
    ALIAS(dark_salmon,             0xe9, 0x96, 0x7a)
    ALIAS(dark_sea_green,          0x8f, 0xbc, 0x8f)
    ALIAS(dark_slate_blue,         0x48, 0x3d, 0x8b)
    ALIAS(dark_slate_gray,         0x2f, 0x4f, 0x4f)
    ALIAS(dark_turquoise,          0x00, 0xce, 0xd1)
    ALIAS(dark_violet,             0x94, 0x00, 0xd3)
    ALIAS(deep_pink,               0xff, 0x14, 0x93)
    ALIAS(deep_sky_blue,           0x00, 0xbf, 0xff)
    ALIAS(dim_gray,                0x69, 0x69, 0x69)
    ALIAS(dodger_blue,             0x1e, 0x90, 0xff)
    ALIAS(fire_brick,              0xb2, 0x22, 0x22)
    ALIAS(floral_white,            0xff, 0xfa, 0xf0)
    ALIAS(forest_green,            0x22, 0x8b, 0x22)
    ALIAS(fuchsia,                 0xff, 0x00, 0xff)
    ALIAS(gainsboro,               0xdc, 0xdc, 0xdc)
    ALIAS(ghost_white,             0xf8, 0xf8, 0xff)
    ALIAS(gold,                    0xff, 0xd7, 0x00)
    ALIAS(golden_rod,              0xda, 0xa5, 0x20)
    ALIAS(gray,                    0x80, 0x80, 0x80)
    ALIAS(green,                   0x00, 0x80, 0x00)
    ALIAS(green_yellow,            0xad, 0xff, 0x2f)
    ALIAS(honey_dew,               0xf0, 0xff, 0xf0)
    ALIAS(hot_pink,                0xff, 0x69, 0xb4)
    ALIAS(indian_red,              0xcd, 0x5c, 0x5c)
    ALIAS(indigo,                  0x4b, 0x00, 0x82)
    ALIAS(ivory,                   0xff, 0xff, 0xf0)
    ALIAS(khaki,                   0xf0, 0xe6, 0x8c)
    ALIAS(lavender,                0xe6, 0xe6, 0xfa)
    ALIAS(lavender_blush,          0xff, 0xf0, 0xf5)
    ALIAS(lawn_green,              0x7c, 0xfc, 0x00)
    ALIAS(lemon_chiffon,           0xff, 0xfa, 0xcd)
    ALIAS(light_blue,              0xad, 0xd8, 0xe6)
    ALIAS(light_coral,             0xf0, 0x80, 0x80)
    ALIAS(light_cyan,              0xe0, 0xff, 0xff)
    ALIAS(light_golden_rod_yellow, 0xfa, 0xfa, 0xd2)
    ALIAS(light_gray,              0xd3, 0xd3, 0xd3)
    ALIAS(light_green,             0x90, 0xee, 0x90)
    ALIAS(light_pink,              0xff, 0xb6, 0xc1)
    ALIAS(light_salmon,            0xff, 0xa0, 0x7a)
    ALIAS(light_sea_green,         0x20, 0xb2, 0xaa)
    ALIAS(light_sky_blue,          0x87, 0xce, 0xfa)
    ALIAS(light_slate_gray,        0x77, 0x88, 0x99)
    ALIAS(light_steel_blue,        0xb0, 0xc4, 0xde)
    ALIAS(light_yellow,            0xff, 0xff, 0xe0)
    ALIAS(lime,                    0x00, 0xff, 0x00)
    ALIAS(lime_green,              0x32, 0xcd, 0x32)
    ALIAS(linen,                   0xfa, 0xf0, 0xe6)
    ALIAS(magenta,                 0xff, 0x00, 0xff)
    ALIAS(maroon,                  0x80, 0x00, 0x00)
    ALIAS(medium_aqua_marine,      0x66, 0xcd, 0xaa)
    ALIAS(medium_blue,             0x00, 0x00, 0xcd)
    ALIAS(medium_orchid,           0xba, 0x55, 0xd3)
    ALIAS(medium_purple,           0x93, 0x70, 0xdb)
    ALIAS(medium_sea_green,        0x3c, 0xb3, 0x71)
    ALIAS(medium_slate_blue,       0x7b, 0x68, 0xee)
    ALIAS(medium_spring_green,     0x00, 0xfa, 0x9a)
    ALIAS(medium_turquoise,        0x48, 0xd1, 0xcc)
    ALIAS(medium_violet_red,       0xc7, 0x15, 0x85)
    ALIAS(midnight_blue,           0x19, 0x19, 0x70)
    ALIAS(mint_cream,              0xf5, 0xff, 0xfa)
    ALIAS(misty_rose,              0xff, 0xe4, 0xe1)
    ALIAS(moccasin,                0xff, 0xe4, 0xb5)
    ALIAS(navajo_white,            0xff, 0xde, 0xad)
    ALIAS(navy,                    0x00, 0x00, 0x80)
    ALIAS(old_lace,                0xfd, 0xf5, 0xe6)
    ALIAS(olive,                   0x80, 0x80, 0x00)
    ALIAS(olive_drab,              0x6b, 0x8e, 0x23)
    ALIAS(orange,                  0xff, 0xa5, 0x00)
    ALIAS(orange_red,              0xff, 0x45, 0x00)
    ALIAS(orchid,                  0xda, 0x70, 0xd6)
    ALIAS(pale_golden_rod,         0xee, 0xe8, 0xaa)
    ALIAS(pale_green,              0x98, 0xfb, 0x98)
    ALIAS(pale_turquoise,          0xaf, 0xee, 0xee)
    ALIAS(pale_violet_red,         0xdb, 0x70, 0x93)
    ALIAS(papaya_whip,             0xff, 0xef, 0xd5)
    ALIAS(peach_puff,              0xff, 0xda, 0xb9)
    ALIAS(peru,                    0xcd, 0x85, 0x3f)
    ALIAS(pink,                    0xff, 0xc0, 0xcb)
    ALIAS(plum,                    0xdd, 0xa0, 0xdd)
    ALIAS(powder_blue,             0xb0, 0xe0, 0xe6)
    ALIAS(purple,                  0x80, 0x00, 0x80)
    ALIAS(red,                     0xff, 0x00, 0x00)
    ALIAS(rosy_brown,              0xbc, 0x8f, 0x8f)
    ALIAS(royal_blue,              0x41, 0x69, 0xe1)
    ALIAS(saddle_brown,            0x8b, 0x45, 0x13)
    ALIAS(salmon,                  0xfa, 0x80, 0x72)
    ALIAS(sandy_brown,             0xf4, 0xa4, 0x60)
    ALIAS(sea_green,               0x2e, 0x8b, 0x57)
    ALIAS(sea_shell,               0xff, 0xf5, 0xee)
    ALIAS(sienna,                  0xa0, 0x52, 0x2d)
    ALIAS(silver,                  0xc0, 0xc0, 0xc0)
    ALIAS(sky_blue,                0x87, 0xce, 0xeb)
    ALIAS(slate_blue,              0x6a, 0x5a, 0xcd)
    ALIAS(slate_gray,              0x70, 0x80, 0x90)
    ALIAS(snow,                    0xff, 0xfa, 0xfa)
    ALIAS(spring_green,            0x00, 0xff, 0x7f)
    ALIAS(steel_blue,              0x46, 0x82, 0xb4)
    ALIAS(tan,                     0xd2, 0xb4, 0x8c)
    ALIAS(teal,                    0x00, 0x80, 0x80)
    ALIAS(thistle,                 0xd8, 0xbf, 0xd8)
    ALIAS(tomato,                  0xff, 0x63, 0x47)
    ALIAS(turquoise,               0x40, 0xe0, 0xd0)
    ALIAS(violet,                  0xee, 0x82, 0xee)
    ALIAS(wheat,                   0xf5, 0xde, 0xb3)
    ALIAS(white,                   0xff, 0xff, 0xff)
    ALIAS(white_smoke,             0xf5, 0xf5, 0xf5)
    ALIAS(yellow,                  0xff, 0xff, 0x00)
    ALIAS(yellow_green,            0x9a, 0xcd, 0x32)
#undef ALIAS

public:
    inline color& clear(void)
    {
        r = g = b = a = 0;

        return *this;
    }

    inline color clone(void) const
    {
        return *this;
    }

public:
    inline unsigned int rgb(void) const
    {
        return 0xff000000 | (r << 16) | (g << 8) | b;
    }

    inline unsigned int argb(void) const
    {
        return (a << 24) | (r << 16) | (g << 8) | b;
    }

    inline unsigned int rgba(void) const
    {
        return (r << 24) | (g << 16) | (b << 8) | a;
    }

    inline unsigned char grayscale(void) const
    {
        return static_cast<unsigned char>((r + g + g + b) >> 2);
    }

    inline color grayscale_color(void) const
    {
        return color(grayscale(), a);
    }

    inline color& grayscale(channel_type grayscale)
    {
        r = g = b = grayscale;

        return *this;
    }

    inline color& multiply(channel_type alpha)
    {
        a = (a * (alpha + 1)) >> channel_shift;

        return *this;
    }

    inline color multiply_copy(channel_type alpha) const
    {
        return clone().multiply(alpha);
    }

    inline color& premultiply(void)
    {
        register unsigned int inc_a = a + 1;

        r = (r * inc_a) >> channel_shift;
        g = (g * inc_a) >> channel_shift;
        b = (b * inc_a) >> channel_shift;

        return *this;
    }

    inline color premultiply_copy(void) const
    {
        return clone().premultiply();
    }

    inline color& visual(void)
    {
        return premultiply().opaque();
    }

    inline color visual_copy(void) const
    {
        return clone().visual();
    }

    inline color& transparent(void)
    {
        return a = 0, *this;
    }

    inline color transparent_copy(void) const
    {
        return clone().transparent();
    }

    inline color& opaque(void)
    {
        return a = channel_mask, *this;
    }

    inline color opaque_copy(void) const
    {
        return clone().opaque();
    }

    color& blend(const color& fg_color);

    color blend_copy(const color& fg_color) const
    {
        return clone().blend(fg_color);
    }

public:
    inline color operator~(void) const
    {
        return color(~r, ~g, ~b, a);
    }

    inline color operator&(const color& c) const
    {
        return (clone() &= c);
    }

    inline color& operator&=(const color& c)
    {
        r &= c.r; g &= c.g; b &= c.b; a &= c.a;

        return *this;
    }

    inline color operator^(const color& c) const
    {
        return (clone() ^= c);
    }

    inline color& operator^=(const color& c)
    {
        r ^= c.r; g ^= c.g; b ^= c.b; a ^= c.a;

        return *this;
    }

    inline color operator|(const color& c) const
    {
        return (clone() |= c);
    }

    inline color& operator|=(const color& c)
    {
        r |= c.r; g |= c.g; b |= c.b; a |= c.a;

        return *this;
    }

    inline color operator*(channel_type factor) const
    {
        return multiply_copy(factor);
    }

    inline color& operator*=(channel_type factor)
    {
        return multiply(factor);
    }

    inline bool operator==(const color& color) const
    {
        return r == color.r && g == color.g && b == color.b && a == color.a;
    }

    inline bool operator!=(const color& color) const
    {
        return r != color.r || g != color.g || b != color.b || a != color.a;
    }
};

color operator*(color::channel_type factor, const color& color);

#endif //color_h_20130717
