#include <cstring>
#include <takisy/core/math.h>
#include <takisy/core/endian_type.h>
#include <takisy/core/stretchy_buffer.h>
#include <takisy/cgl/basic/pixel_format.h>
#include <takisy/cgl/image/format.h>

#pragma pack(1)

class bmp::implement
{
    friend class bmp;

    static const unsigned int kBmpFlag = 0x4d42;

    struct file_header
    {
        etle_uint16 flag;
        etle_uint32 size;
        etle_uint16 reserved_1;
        etle_uint16 reserved_2;
        etle_uint32 offset;
    };

    struct info_header
    {
        etle_uint32 size;
        etle_sint32 width;
        etle_sint32 height;
        etle_uint16 planes;
        etle_uint16 bit_depth;
        etle_uint32 compression;
        etle_uint32 size_image;
        etle_sint32 x_pels_per_meter;
        etle_sint32 y_pels_per_meter;
        etle_uint32 color_used;
        etle_uint32 color_important;
    };

public:
    static inline
    unsigned int get_index(unsigned char* row_buffer, unsigned int bit_depth,
                           unsigned int x, unsigned int mask)
    {
        unsigned int _1 = x * bit_depth;

        return (row_buffer[_1 / 8] >> (8 - _1 % 8 - bit_depth)) & mask;
    }

public:
    static canvas_adapter::pointer load(const stream& stream)
    {
        file_header file_header;
        if (stream.read(file_header) != sizeof(file_header)
            || file_header.flag != kBmpFlag)
            return nullptr;

        info_header info_header;
        if (stream.read(info_header) != sizeof(info_header)
            || info_header.size != sizeof(info_header)
            || info_header.compression != 0)
            return nullptr;

        typedef pixel_format<unsigned char, b, g, r, R> pf_bgrR;
        canvas_adapter::pointer canvas;
        stretchy_buffer<pf_bgrR> palette;
        bool down2up = true;

        switch (info_header.bit_depth)
        {
        case  1: case  2: case  4:
        case  8: palette.resize(info_header.color_used > 0 ?
                    int(info_header.color_used) : 1 << info_header.bit_depth);
                 stream.read(palette.data(), sizeof(pf_bgrR) * palette.size());
        case 16:
        case 24: canvas = canvas_adapter::make<canvas_bgr8>(); break;
        case 32: canvas = canvas_adapter::make<canvas_bgra8>(); break;
        default: return nullptr;
        }

        canvas->resize(info_header.width, info_header.height);

        if (info_header.height < 0)
        {
            info_header.height = -info_header.height;
            down2up = false;
        }

        unsigned int row_bits  = info_header.bit_depth * info_header.width;
        unsigned int row_bytes = math::ceil(row_bits / 32.0) * 4;
        unsigned int row_index = down2up ? info_header.height - 1 : 0;
        stretchy_buffer<unsigned char> row_buffer(row_bytes);

        while (stream.read(row_buffer.data(), row_bytes) == row_bytes)
        {
            unsigned int bit_depth = info_header.bit_depth;
            unsigned int mask;

            switch (bit_depth)
            {
            case  1: case  2: case  4: case  8:
                mask = (1 << bit_depth) - 1;

                for (int x = 0; x < info_header.width; ++x)
                {
                    int pi = get_index(row_buffer.data(), bit_depth, x, mask);

                    canvas->pixel(x, row_index, palette[pi]);
                }

                break;
            case 16:
                {
                    struct pf565
                    {
                        unsigned short r:5, g:6, b:5;

                    public:
                        inline operator color(void) const
                        {
                            return color(r << 3, g << 2, b << 3);
                        }
                    } *pixels = reinterpret_cast<pf565*>(row_buffer.data());

                    for (int x = 0; x < info_header.width; ++x)
                        canvas->pixel(x, row_index, pixels[x]);
                }
                break;
            case 24: case 32:
                memory::_memcpy(canvas->row_buffer(row_index),
                                row_buffer.data(), canvas->row_bytes());
                break;
            default:
                return nullptr;
            }

            down2up ? --row_index : ++row_index;
        }

        return canvas;
    }

    static bool dump(const canvas_adapter::pointer& canvas, stream& stream)
    {
        unsigned int row_bytes = canvas->width() * 4;

        file_header file_header;
        file_header.flag       = kBmpFlag;
        file_header.size       = sizeof(file_header) + sizeof(info_header)
                                 + row_bytes * canvas->height();
        file_header.reserved_1 = 0;
        file_header.reserved_2 = 0;
        file_header.offset     = sizeof(file_header) + sizeof(info_header);
        if (stream.write(file_header) != sizeof(file_header))
            return false;

        info_header info_header;
        info_header.size             = sizeof(info_header);
        info_header.width            = canvas->width();
        info_header.height           = canvas->height();
        info_header.planes           = 1;
        info_header.bit_depth        = 32;
        info_header.compression      = 0;
        info_header.size_image       = row_bytes * canvas->height();
        info_header.x_pels_per_meter = 0x4c2;
        info_header.y_pels_per_meter = 0x4c2;
        info_header.color_used       = 0;
        info_header.color_important  = 0;
        if (stream.write(info_header) != sizeof(info_header))
            return false;

        stretchy_buffer<unsigned char> row_buffer(row_bytes);
        pf_bgra8* row_pixels = reinterpret_cast<pf_bgra8*>(row_buffer.data());

        for (int y = canvas->height() - 1; y >= 0; --y)
        {
            for (unsigned int x = 0; x < canvas->width(); ++x)
                row_pixels[x] = canvas->pixel(x, y);

            if (stream.write(row_buffer.data(), row_bytes) != row_bytes)
                return false;
        }

        return true;
    }
};

bool bmp::load(const stream& stream, frames& frames)
{
    canvas_adapter::pointer canvas = implement::load(stream);

    if (canvas)
    {
        frames.append(frame{.canvas = canvas, .interval = 0});

        return true;
    }
    else
        return false;
}

bool bmp::dump(const frames& frames, stream& stream) const
{
    return frames.size() ? implement::dump(frames[0].canvas, stream) : false;
}
