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
    private:
        typedef stretchy_buffer<unsigned char> buffer_t;

        struct segment
        {
            union { unsigned int literal; unsigned int offset; } aka;
            unsigned int width;

        public:
            bool equal_with(const buffer_t& sequence, const segment& s) const
            {
                if (width != s.width)
                    return false;

                for (unsigned int i = 0; i < width; ++i)
                    if (sequence[aka.offset + i] != sequence[s.aka.offset + i])
                        return false;

                return true;
            }

            inline void append_to_v1(buffer_t& sequence) const
            {
                sequence.append(aka.literal);
            }

            inline void append_to_v2(buffer_t& sequence) const
            {
                for (unsigned int i = 0; i < width; ++i)
                    sequence.append((unsigned char)sequence[aka.offset + i]);
            }

            inline void append_to(buffer_t& sequence) const
            {
                width == 1 ? append_to_v1(sequence) : append_to_v2(sequence);
            }
        };

        struct code_table_t
        {
            segment segments[4096];
            unsigned int c_clear;
            unsigned int c_eoi;
            unsigned int c_next;

        public:
            code_table_t(unsigned int code_size)
                : c_clear(1 << code_size), c_eoi(c_clear + 1), c_next(c_eoi + 1)
            {
                for (unsigned int i = 0; i < c_next; ++i)
                    segments[i] = segment {.aka = {.literal = i}, .width = 1};
            }

            inline void reset(void)
            {
                c_next = c_eoi + 1;
            }

            int code(const buffer_t& sequence, const segment& s)
            {
                if (s.width == 1)
                    return s.aka.literal;
                else
                {
                    for (unsigned int i = c_eoi + 1; i < c_next; ++i)
                        if (segments[i].equal_with(sequence, s))
                            return i;

                    return -1;
                }
            }
        };

    public:
        static bool decompress(unsigned int code_size,
                               const buffer_t& data, buffer_t& index_stream)
        {
            bit_buffer_i code_buffer(data.data(), data.size());
            code_table_t code_table(code_size); // code table
            lzw::segment cs, os = {0}; // current segment, old segment
            unsigned int cbits = code_size + 1, ccount = 1 << cbits;
            unsigned int code;

            while (true)
            {
                if (code_table.c_next >= ccount && cbits < 12)
                    ccount = 1 << ++cbits;

                try { code = code_buffer.read(cbits); }
                catch (...) { return false; }

                if (code == code_table.c_clear)
                {
                    code_table.reset();
                    ccount = 1 << (cbits = code_size + 1);
                    os.width = 0;
                }
                else
                if (code == code_table.c_eoi)
                    break;
                else
                {
                    if (code_table.c_next >= ccount)
                        return false;

                    if (code < code_table.c_next)
                    {
                        cs = code_table.segments[code];

                        if (os.width != 0)
                            code_table.segments[code_table.c_next++] = segment {
                                .aka   = os.aka,
                                .width = os.width + 1,
                            };

                        os = segment {
                            .aka   = {.offset = index_stream.size()},
                            .width = cs.width
                        };

                        cs.append_to(index_stream);
                    }
                    else
                    if (code == code_table.c_next)
                    {
                        cs = segment {
                            .aka   = {.offset = index_stream.size()},
                            .width = os.width + 1
                        };

                        os.append_to_v2(index_stream);
                        index_stream.append(index_stream[os.aka.offset]);

                        code_table.segments[code_table.c_next++] = cs;

                        os = cs;
                    }
                    else
                        return false;
                }
            }

            return true;
        }
    };

private:
    static stretchy_buffer<unsigned char> read_sub_blocks(stream& stream)
    {
        stretchy_buffer<unsigned char> result;

        while (true)
        {
            unsigned char sub_block_bytes;
            if (stream.read(sub_block_bytes) != sizeof(sub_block_bytes))
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
    static bool load(stream& stream, frames& frames)
    {
        typedef pf_rgb8 palette_pf_t;
        const unsigned int kPFPaletteBytes = palette_pf_t::pixel_bytes();

        header header;
        if (stream.read(header) != sizeof(header)
            || header.flag != kGifFlag
            || (header.version != kVersion87a && header.version != kVersion89a))
            return false;

        logical_screen_descriptor lsd;
        if (stream.read(lsd) != sizeof(lsd))
            return false;

        stretchy_buffer<palette_pf_t> gct;
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
            if (stream.read(flag) != sizeof(flag))
                return false;

            if (flag == kExtension)
            {
                unsigned char extension_type;
                if (stream.read(extension_type) != sizeof(extension_type))
                    return false;

                switch (extension_type)
                {
                case etGrahicsControl:
                    gce.reset(new graphics_control_extension);
                    if (stream.read(*gce) != sizeof(graphics_control_extension))
                        return false;
                    break;
                case etPlainText:
                case etApplication:
                    {
                        unsigned char skip_size;
                        if (stream.read(skip_size) != sizeof(skip_size))
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
                if (stream.read(id) != sizeof(id))
                    return false;

                stretchy_buffer<palette_pf_t> lct, *palette = &gct;
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
                if (stream.read(code_size) != sizeof(code_size))
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

                frames.append(frame {
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

bool gif::load(stream& stream, frames& frames) const
{
    return implement::load(stream, frames);
}

bool gif::dump(const frames& frames, stream& stream) const
{
    return false;
}
