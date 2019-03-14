#include <cstring>
#include <map>
#include <stdexcept>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include <takisy/cgl/font/freetype.h>

#define REAL(number, fractional_bit_count) \
    ((number) / static_cast<double>(1 << (fractional_bit_count)))

class freetype::implement
{
    friend class freetype;

    static class library
    {
    public:
        library(void) { if (!library_) FT_Init_FreeType(&library_); }
       ~library(void) { if ( library_) FT_Done_FreeType( library_); }

    public:
        inline operator const FT_Library(void) const
        {
            return library_;
        }

    private:
        FT_Library library_ = nullptr;
    } library;

    struct property
    {
        FT_Int32        load_flag = FT_LOAD_DEFAULT;
        FT_Render_Mode  render_mode = FT_RENDER_MODE_NORMAL;
        FT_UInt         width = 0, height = 0;
        FT_Pos          xstrength = 0, ystrength = 0;
        FT_Fixed        lean = 0;

    public:
        inline bool operator<(const property& property) const
        {
            return load_flag   < property.load_flag
                || render_mode < property.render_mode
                || width       < property.width
                || height      < property.height
                || xstrength   < property.xstrength
                || ystrength   < property.ystrength
                || lean        < property.lean;
        }
    };

    typedef std::map<unsigned int, bitmap> cache_type;

public:
    implement(void)
        : face_(nullptr)
    {}

    ~implement(void)
    {
        for (auto& i : cache_)
        for (auto& j : i.second)
            delete [] j.second.buffer;
    }

public:
    inline FT_GlyphSlot load_glyph(unsigned int char_code)
    {
        bool embolden   = property_.xstrength != 0 || property_.ystrength != 0;
        bool distortion = embolden || property_.lean != 0;
        FT_Int32 load_flag = property_.load_flag;
        if (distortion)
            load_flag |= FT_LOAD_NO_BITMAP;

        FT_UInt glyph_index = FT_Get_Char_Index(face_, char_code);
        if (0 != FT_Load_Glyph(face_, glyph_index, load_flag))
            return nullptr;

        FT_GlyphSlot glyph = face_->glyph;
        if (embolden && 0 != FT_Outline_EmboldenXY(&glyph->outline,
                                property_.xstrength, property_.ystrength))
            return nullptr;

        return glyph;
    }

    inline cache_type& get_cache(void)
    {
        return cache_[property_];
    }

private:
    FT_Face face_;
    property property_;
    std::map<property, cache_type> cache_;
};

class freetype::implement::library freetype::implement::library;

freetype::freetype(const char* font_file)
    : freetype(font_file, 12)
{}

freetype::freetype(const char* font_file, unsigned int size)
    : freetype(font_file, 0, size)
{}

freetype::freetype(const char* font_file, int face_index, unsigned int size)
    : impl_(new implement)
{
    FT_New_Face(implement::library, font_file, face_index, &impl_->face_);
    fontsize(size);
}

freetype::~freetype(void)
{
    if (impl_->face_)
        FT_Done_Face(impl_->face_);

    delete impl_;
}

freetype::freetype(const freetype& freetype)
{
    operator=(freetype);
}

freetype& freetype::operator=(const freetype& freetype)
{
    throw std::logic_error("class freetype is noncopyable class.");
}

double freetype::ascender(void) const
{
    return REAL(impl_->face_->size->metrics.ascender, 6);
}

double freetype::descender(void) const
{
    return REAL(impl_->face_->size->metrics.descender, 6);
}

double freetype::height(void) const
{
    return REAL(impl_->face_->size->metrics.height, 6);
}

double freetype::emheight(void) const
{
    return impl_->face_->size->metrics.y_ppem;
}

freetype::RenderMode freetype::render_mode(void) const
{
    switch (impl_->property_.render_mode)
    {
    default:                   return rmNormal;
    case FT_RENDER_MODE_LIGHT: return rmLight;
    case FT_RENDER_MODE_MONO:  return rmMono;
    }
}

void freetype::render_mode(RenderMode _render_mode)
{
    if (render_mode() != _render_mode)
    {
        switch (_render_mode)
        {
        default:      impl_->property_.render_mode = FT_RENDER_MODE_NORMAL;
        case rmLight: impl_->property_.render_mode = FT_RENDER_MODE_LIGHT;
        case rmMono:  impl_->property_.render_mode = FT_RENDER_MODE_MONO;
        }
    }
}

void freetype::ignore_bitmap(bool ignore)
{
    if (ignore)
        impl_->property_.load_flag |=  FT_LOAD_NO_BITMAP;
    else
        impl_->property_.load_flag &= ~FT_LOAD_NO_BITMAP;
}

bool freetype::fontsize(unsigned int size)
{
    return fontsize(size, size);
}

bool freetype::fontsize(unsigned int width, unsigned int height)
{
    if (0 != FT_Set_Pixel_Sizes(impl_->face_, width, height))
        return false;

    impl_->property_.width  = width;
    impl_->property_.height = height;

    return true;
}

void freetype::embolden(bool _embolden)
{
    embolden(_embolden ? impl_->property_.width / 12.0 : 0);
}

void freetype::embolden(double strength)
{
    embolden(strength, 0);
}

void freetype::embolden(double xstrength, double ystrength)
{
    impl_->property_.xstrength = xstrength * 64;
    impl_->property_.ystrength = ystrength * 64;
}

void freetype::italic(bool _italic)
{
    italic(_italic ? 0.382 : 0);
}

void freetype::italic(double lean)
{
    FT_Matrix matrix;

    matrix.xx = 1 * 0x10000;
    matrix.xy = impl_->property_.lean = lean * 0x10000;
    matrix.yx = 0;
    matrix.yy = 1 * 0x10000;

    FT_Set_Transform(impl_->face_, &matrix, 0);
}

const bitmap* freetype::get_bitmap(unsigned int char_code) const
{
    implement::cache_type& cache = impl_->get_cache();

    try
        { return &cache.at(char_code); }
    catch (const std::out_of_range&)
        {}

    FT_GlyphSlot glyph = impl_->load_glyph(char_code);
    if (!glyph || 0 != FT_Render_Glyph(glyph, impl_->property_.render_mode))
        return nullptr;

    bitmap* bitmap = &cache[char_code];
    unsigned int buffer_length = glyph->bitmap.rows * glyph->bitmap.pitch;

    bitmap->left       = glyph->bitmap_left;
    bitmap->top        = glyph->bitmap_top - ascender();
    bitmap->width      = glyph->bitmap.width;
    bitmap->height     = glyph->bitmap.rows;
    bitmap->advance    = REAL(glyph->linearHoriAdvance, 16);
    bitmap->pitch      = glyph->bitmap.pitch;
    bitmap->pixel_mode = glyph->bitmap.pixel_mode;
    bitmap->buffer     = reinterpret_cast<unsigned char*>(
                                memcpy(new unsigned char [buffer_length],
                                       glyph->bitmap.buffer,
                                       buffer_length));

    return bitmap;
}
