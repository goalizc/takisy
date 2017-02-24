#include <memory>
#include <string>
#include <takisy/core/bit_buffer.h>
#include <takisy/core/endian_type.h>
#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/basic/pixel_format.h>
#include <takisy/cgl/image/format.h>

#pragma pack(1)

class gif::implement
{
    friend class gif;

    static const unsigned int kGifFlag         = 0x474946;
    static const unsigned int kVersion87a      = 0x383761;
    static const unsigned int kVersion89a      = 0x383961;
    static const unsigned int kExtension       = 0x21;
    static const unsigned int kImageDescriptor = 0x2c;
    static const unsigned int kTrailer         = 0x3b;

    enum ExtensionType
    {
        etGrahicsControl = 0xf9,
        etPlainText      = 0x01,
        etApplication    = 0xff,
        etComment        = 0xfe,
    };

    struct header
    {
        class
        {
        public:
            inline operator unsigned int(void) const
            {
                return (data_[0] << 16) | (data_[1] << 8) | data_[2];
            }

        private:
            unsigned char data_[3];
        }

        flag,
        version;
    };

    struct logical_screen_descriptor
    {
        etle_uint16 width;
        etle_uint16 height;
        etle_uint8  size_of_global_color_table:3;
        etle_uint8  sort_flag:1;
        etle_uint8  color_resolution:3;
        etle_uint8  global_color_table_flag:1;
        etle_uint8  background_color_index;
        etle_uint8  pixel_aspect_ratio;
    };

    struct graphics_control_extension
    {
        etle_uint8  block_size;
        etle_uint8  transparent_color_flag:1;
        etle_uint8  user_input_flag:1;
        etle_uint8  disposal_method:3;
        etle_uint8  reversed:3;
        etle_uint16 delay_time;
        etle_uint8  transparent_color_index;
        etle_uint8  block_terminator;
    };

    struct image_descriptor
    {
        etle_uint16 image_left;
        etle_uint16 image_top;
        etle_uint16 image_width;
        etle_uint16 image_height;
        etle_uint8  size_of_local_color_table:3;
        etle_uint8  reversed:2;
        etle_uint8  sort_flag:1;
        etle_uint8  interlace_flag:1;
        etle_uint8  local_color_table_flag:1;
    };

    class lzw
    {
    public:
        static bool decompress(unsigned int codesize,
                               const stretchy_buffer<unsigned char>& input,
                                     stretchy_buffer<unsigned char>& output)
        {
            struct context
            {
                unsigned short codesize, cursize, clear, eoi, slot, topslot;
                unsigned char  suffix[4096], stack[4096];
                unsigned short prefix[4096];

            public:
                context(unsigned int codesize)
                    : codesize(codesize), cursize(codesize + 1)
                    , clear(1 << codesize), eoi(clear + 1), slot(eoi + 1)
                    , topslot(1 << cursize)
                {}
            };

            context s(codesize);
            bit_buffer::input bbi(input.data(), input.size());
            int c, code, fc, oc = fc = -1;
            unsigned char* sp = s.stack;

            while (true)
            {
                try { c = bbi.read(s.cursize); }
                catch (...) { return false; }

                if (c == s.eoi)
                    break;
                else if (c == s.clear)
                    s.topslot = 1 << (s.cursize = s.codesize + 1),
                    s.slot = s.eoi + 1, oc = fc = -1;
                else
                {
                    code = c;
                    if (code == s.slot && fc >= 0)
                        *sp++ = fc, code = oc;
                    else if (code >= s.slot)
                        return false;
                    while (code > s.eoi)
                        *sp++ = s.suffix[code], code = s.prefix[code];
                        *sp++ = code;
                    while (sp > s.stack)
                        output.append(*--sp);
                    if (s.slot < s.topslot && oc >= 0)
                        s.suffix[s.slot] = code, s.prefix[s.slot++] = oc;
                    fc = code, oc = c;
                    if (s.slot >= s.topslot && s.cursize < 12)
                        s.topslot = 1 << ++s.cursize;
                }
            }

            return true;
        }
    };

private:
    static stretchy_buffer<unsigned char> read_sub_blocks(const stream& stream)
    {
        stretchy_buffer<unsigned char> result;

        while (true)
        {
            unsigned char sub_block_bytes;
            if (!stream.read(sub_block_bytes))
                return stretchy_buffer<unsigned char>();

            if (sub_block_bytes == 0)
                break;

            unsigned char buffer[256];
            if (stream.read(buffer, sub_block_bytes) != sub_block_bytes)
                return stretchy_buffer<unsigned char>();

            result.append(buffer, sub_block_bytes);
        }

        return result;
    }

public:
    static bool load(const stream& stream, frames& frames)
    {
        typedef pixfmt_rgb8 pixfmt_palette;
        const unsigned int kPFPaletteBytes = pixfmt_palette::pixel_bytes();

        header header;
        if (!stream.read(header)
            || header.flag != kGifFlag
            || (header.version != kVersion87a && header.version != kVersion89a))
            return false;

        logical_screen_descriptor lsd;
        if (!stream.read(lsd))
            return false;

        stretchy_buffer<pixfmt_palette> gct;
        if (lsd.global_color_table_flag)
        {
            gct.resize(2 << lsd.size_of_global_color_table);

            unsigned int gct_bytes = gct.size() * kPFPaletteBytes;
            if (stream.read(gct.data(), gct_bytes) != gct_bytes)
                return false;
        }

        std::shared_ptr<graphics_control_extension> gce(nullptr);
        canvas_adapter::pointer canvas =
                canvas_adapter::make<canvas_rgba8>(lsd.width, lsd.height);

        while (true)
        {
            unsigned char flag;
            if (!stream.read(flag))
                return false;

            if (flag == kExtension)
            {
                unsigned char extension_type;
                if (!stream.read(extension_type))
                    return false;

                switch (extension_type)
                {
                case etGrahicsControl:
                    gce.reset(new graphics_control_extension);
                    if (!stream.read(*gce))
                        return false;
                    break;
                case etPlainText:
                case etApplication:
                    {
                        unsigned char skip_size;
                        if (!stream.read(skip_size))
                            return false;
                        stretchy_buffer<unsigned char> skip_data(skip_size);
                        if (stream.read(skip_data.data(), skip_size)
                            != skip_size)
                            return false;
                    }
                case etComment:
                    read_sub_blocks(stream);
                    break;
                default:
                    return false;
                }
            }
            else
            if (flag == kImageDescriptor)
            {
                image_descriptor id;
                if (!stream.read(id))
                    return false;

                stretchy_buffer<pixfmt_palette> lct, *palette = &gct;
                if (id.local_color_table_flag)
                {
                    lct.resize(2 << id.size_of_local_color_table);

                    unsigned int lct_bytes = lct.size() * kPFPaletteBytes;
                    if (stream.read(lct.data(), lct_bytes) != lct_bytes)
                        return false;

                    palette = &lct;
                }
                else
                if (!lsd.global_color_table_flag)
                    return false;

                unsigned char code_size;
                if (!stream.read(code_size))
                    return false;

                stretchy_buffer<unsigned char> data = read_sub_blocks(stream);
                if (data.null())
                    return false;

                stretchy_buffer<unsigned char> index_stream;
                if (!lzw::decompress(code_size, data, index_stream))
                    return false;

                unsigned int interval = 100;

                if (gce)
                {
                    switch (gce->disposal_method)
                    {
                    default:
                    case 0:
                    case 1:
                        break;
                    case 2:
                        canvas->clear(gct[lsd.background_color_index]);
                        break;
                    case 3:
                        if (frames.size() >= 2)
                            *canvas = *frames[frames.size() - 2].canvas;
                        break;
                    }

                    if (gce->delay_time > 0)
                        interval = gce->delay_time * 10;
                }

                for (unsigned int y = 0; y < id.image_height; ++y)
                for (unsigned int x = 0; x < id.image_width; ++x)
                {
                    unsigned int index = index_stream[y * id.image_width + x];

                    if (!gce
                        || !gce->transparent_color_flag
                        ||  gce->transparent_color_index != index)
                        canvas->pixel(x + id.image_left,
                                      y + id.image_top, (*palette)[index]);
                }

                frames.append({
                    .canvas   = canvas_adapter::make<canvas_rgba8>(*canvas),
                    .interval = interval
                });

                gce.reset();
            }
            else
            if (flag == kTrailer)
                break;
            else
                return false;
        }

        return frames.size() != 0;
    }
};

bool gif::load(const stream& stream, frames& frames) const
{
    return implement::load(stream, frames);
}

bool gif::dump(const frames& frames, stream& stream) const
{
    return false;
}
