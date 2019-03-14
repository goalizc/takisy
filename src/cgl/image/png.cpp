#include <cstring>
#include <takisy/core/endian_type.h>
#include <takisy/core/stretchy_buffer.h>
#include <takisy/algorithm/crc.h>
#include <takisy/algorithm/zlib.h>
#include <takisy/cgl/basic/pixel_format.h>
#include <takisy/cgl/image/format.h>

#pragma pack(1)

class png::implement
{
    friend class png;

    static const unsigned long long kPngFlag = 0x89504e470d0a1a0a;

    enum ChunkType
    {
        ct_IHDR = 0x49484452, ct_cHRM = 0x6348524d, ct_gAMA = 0x67414d41,
        ct_sBIT = 0x73424954, ct_PLTE = 0x504c5445, ct_bKGD = 0x624b4744,
        ct_hIST = 0x68495354, ct_tRNS = 0x74524e53, ct_oFFs = 0x6f464673,
        ct_pHYs = 0x70485973, ct_sCAL = 0x7343414c, ct_IDAT = 0x49444154,
        ct_tIME = 0x74494d45, ct_tEXt = 0x74455874, ct_zTXt = 0x7a545874,
        ct_fRAc = 0x66524163, ct_gIFg = 0x67494667, ct_gIFt = 0x67494674,
        ct_gIFx = 0x67494678, ct_IEND = 0x49454e44,
    };

    struct chunk
    {
        etbe_uint32 length     = 0;
        etbe_uint32 chunk_type = 0;
        void*       chunk_data = nullptr;
        etbe_uint32 crc        = 0;

    public:
        ~chunk(void)
        {
            if (chunk_data)
                delete [] reinterpret_cast<char*>(chunk_data);
        }

    public:
        bool load(const stream& stream)
        {
            return !stream.read(length)
                && !stream.read(chunk_type) && read_chunk_data(stream)
                && !stream.read(crc) && calculate_crc() == crc;
        }

        bool dump(stream& stream)
        {
            crc = calculate_crc();

            return !stream.write(length)
                && !stream.write(chunk_type)
                &&  stream.write(chunk_data, length) == length
                && !stream.write(crc);
        }

    private:
        bool read_chunk_data(const stream& stream)
        {
            if (chunk_data)
            {
                delete [] reinterpret_cast<char*>(chunk_data);;
                chunk_data = nullptr;
            }

            if (length > 0)
            {
                chunk_data = new char [length];

                return chunk_data && stream.read(chunk_data, length) == length;
            }

            return true;
        }

        unsigned int calculate_crc(void) const
        {
            class crc32 crc_obj;

            crc_obj.update(&chunk_type, sizeof(chunk_type));
            crc_obj.update(chunk_data, length);

            return crc_obj.digest();
        }
    };

    struct ihdr_data
    {
        etbe_uint32 width;
        etbe_uint32 height;
        etbe_uint8  bit_depth;
        etbe_uint8  color_type;
        etbe_uint8  method_compression;
        etbe_uint8  method_filter;
        etbe_uint8  method_interlace;
    };

    struct plte_data
    {
        unsigned int size = 0;
        pixfmt_rgb8* data = nullptr;

    public:
        ~plte_data(void)
        {
            if (data)
                delete [] reinterpret_cast<char*>(data);
        }

    public:
        bool init(chunk& chunk)
        {
            if (chunk.length % 3 != 0)
                return false;

            size = chunk.length / 3;
            data = reinterpret_cast<pixfmt_rgb8*>(chunk.chunk_data);

            chunk.chunk_data = nullptr;

            return true;
        }

        pixfmt_rgb8 operator[](unsigned int index)
        {
            return index < size ? data[index] : data[0];
        }
    };

private:
    static inline int paeth(int a, int b, int c)
    {
        int p = a + b - c, pa = p - a, pb = p - b, pc = p - c;

        if (pa < 0) pa = -pa;
        if (pb < 0) pb = -pb;
        if (pc < 0) pc = -pc;

        return pa <= pb && pa <= pc ? a : pb <= pc ? b : c;
    }

    static inline void refilter(const unsigned char* flt,
                                const unsigned char* pre,
                                int row_bytes, int alignment,
                                stretchy_buffer<unsigned char>& row)
    {
        int alignmentc = alignment;

        switch (*flt++)
        {
        case 0:
            while (row_bytes--)
                row.append(*flt++);
            break;
        case 1:
            while (alignmentc--)
                row.append(*flt++), --row_bytes;
            while (row_bytes--)
                row.append(*flt++ + row.end()[-alignment]);
            break;
        case 2:
            while (row_bytes--)
                row.append(*flt++ + *pre++);
            break;
        case 3:
            while (alignmentc--)
                row.append(*flt++ + *pre++ / 2), --row_bytes;
            while (row_bytes--)
                row.append(*flt++ + (row.end()[-alignment] + *pre++) / 2);
            break;
        case 4:
            while (alignmentc--)
                row.append(*flt++ + *pre++), --row_bytes;
            while (row_bytes--)
                row.append(*flt++ + paeth(row.end()[-alignment],
                                          pre[0], pre[-alignment])), ++pre;
            break;
        default:
            throw "invalid filter type";
        }
    }

    static inline unsigned int grayscale(unsigned char* row_buffer,
                                         unsigned int bit_count,
                                         unsigned int x)
    {
        unsigned int _1 = x * bit_count;
        unsigned int _2 = 8 - bit_count;

        return (row_buffer[_1 / 8] >> (8 - _1 % 8 - bit_count)) << _2;
    }

    static inline
    unsigned int get_index(unsigned char* row_buffer, unsigned int bit_count,
                           unsigned int x, unsigned int mask)
    {
        unsigned int _1 = x * bit_count;

        return (row_buffer[_1 / 8] >> (8 - _1 % 8 - bit_count)) & mask;
    }

public:
    static canvas_adapter::pointer load(const stream& stream)
    {
        etbe_uint64 flag;
        if (!stream.read(flag) || flag != kPngFlag)
            throw "this file is not png format";

        chunk chunk;
        canvas_adapter::pointer canvas;

        if (!chunk.load(stream) || chunk.chunk_type != ct_IHDR)
            throw "fail to load chunk or first chunk is not IHDR";

        ihdr_data ihdr = *reinterpret_cast<ihdr_data*>(chunk.chunk_data);
        unsigned int pixel_bits;

        switch (ihdr.color_type)
        {
        case 0: // gray image
            pixel_bits = ihdr.bit_depth * 1;
            canvas = ihdr.bit_depth == 16
                    ? canvas_adapter::make<canvas_G16>()
                    : canvas_adapter::make<canvas_G8>();
            break;
        case 2: // real color image
            pixel_bits = ihdr.bit_depth * 3;
            canvas = ihdr.bit_depth == 16
                    ? canvas_adapter::make<canvas_rgb16>()
                    : canvas_adapter::make<canvas_rgb8>();
            break;
        case 3: // index color image
            pixel_bits = ihdr.bit_depth * 1;
            canvas = canvas_adapter::make<canvas_rgb8>();
            break;
        case 4: // gray color image with alpha channel
            pixel_bits = ihdr.bit_depth * 2;
            canvas = ihdr.bit_depth == 16
                    ? canvas_adapter::make<canvas_Ga16>()
                    : canvas_adapter::make<canvas_Ga8>();
            break;
        case 6: // real color image with alpha channel
            pixel_bits = ihdr.bit_depth * 4;
            canvas = ihdr.bit_depth == 16
                    ? canvas_adapter::make<canvas_rgba16>()
                    : canvas_adapter::make<canvas_rgba8>();
            break;
        default:
            throw "invalid color type";
        }

        canvas->resize(ihdr.width, ihdr.height);

        plte_data plte;
        zlib::buffer_type data;

        while (true)
        {
            if (!chunk.load(stream))
                throw "fail to load chunk";

            if (chunk.chunk_type == ct_IEND)
                break;

            switch (chunk.chunk_type)
            {
            case ct_PLTE:
                if (!plte.init(chunk))
                    throw "fail to initialize pallete";
                break;

            case ct_IDAT:
                data.append(reinterpret_cast<zlib::buffer_type::value_type*>
                                (chunk.chunk_data),
                            chunk.length);
                break;

            default:
                break;
            }
        }

        zlib::buffer_type rawdata = zlib::decompress(data.data(), data.size());
        if (rawdata.null())
            throw "fail to decompress IDAT chunk data";

        unsigned int mask;
        unsigned int row_bits = pixel_bits * ihdr.width;
        unsigned int row_bytes = (row_bits + 7) / 8;
        int alignment = pixel_bits < 8 ? 1 : pixel_bits / 8;
        stretchy_buffer<stretchy_buffer<unsigned char>, true> rows(ihdr.height);
        stretchy_buffer<unsigned char> prerow(row_bytes);

        memset(prerow.data(), 0, prerow.size());
        refilter(rawdata.data(), prerow.data(), row_bytes, alignment, rows[0]);
        for (unsigned int y = 1; y < ihdr.height; ++y)
            refilter(rawdata.data() + y * (1 + row_bytes),
                     rows[y - 1].data(), row_bytes, alignment, rows[y]);

        switch (ihdr.color_type)
        {
        case 0:
            if (ihdr.bit_depth < 8)
            {
                for (unsigned int y = 0; y < ihdr.height; ++y)
                for (unsigned int x = 0; x < ihdr.width; ++x)
                    canvas->pixel(x, y,
                        color(grayscale(rows[y].data(), ihdr.bit_depth, x)));
                break;
            }
        case 2: case 4: case 6:
            for (unsigned int y = 0; y < ihdr.height; ++y)
                memcpy(canvas->row_buffer(y), rows[y].data(), row_bytes);
            break;
        case 3:
            mask = (1 << ihdr.bit_depth) - 1;

            for (unsigned int y = 0; y < ihdr.height; ++y)
            for (unsigned int x = 0; x < ihdr.width; ++x)
                canvas->pixel(x, y,
                    plte[get_index(rows[y].data(), ihdr.bit_depth, x, mask)]);

            break;
        }

        return canvas;
    }

    static bool dump(const canvas_adapter::pointer& canvas, stream& stream)
    {
        static const etbe_uint64 png_flag = kPngFlag;
        typedef pixfmt_rgba8 pixel_format;
        typedef struct {
            unsigned char filter_type;
            pixel_format  pixels[1];
        } filter_row_t;

        // write png flag
        if (!stream.write(png_flag))
            return false;

        // write ihdr chunk
        ihdr_data* ihdr = new ihdr_data {
            .width              = canvas->width(),
            .height             = canvas->height(),
            .bit_depth          = pixel_format::channel_bits(),
            .color_type         = 6,
            .method_compression = 0,
            .method_filter      = 0,
            .method_interlace   = 0,
        };

        chunk chunk_ihdr;
        chunk_ihdr.length     = sizeof(ihdr_data);
        chunk_ihdr.chunk_type = ct_IHDR;
        chunk_ihdr.chunk_data = ihdr;
        if (!chunk_ihdr.dump(stream))
            return false;

        // write idat chunk
        unsigned int rlen = ihdr->width  * pixel_format::pixel_bytes() + 1;
        unsigned int tlen = ihdr->height * rlen;
        stretchy_buffer<unsigned char> flt_data(tlen);
        filter_row_t* filter_row;

        for (unsigned int y = 0, i = 0; i < tlen; ++y, i += rlen)
        {
            filter_row = reinterpret_cast<filter_row_t*>(flt_data.data() + i);

            for (unsigned int x = 0; x < ihdr->width; ++x)
                filter_row->pixels[x] = canvas->pixel(x, y);

            filter_row->filter_type = 0;
        }

        zlib::buffer_type data =
                zlib::compress(flt_data.data(), flt_data.size());

        chunk chunk_idat;
        chunk_idat.length     = data.size();
        chunk_idat.chunk_type = ct_IDAT;
        chunk_idat.chunk_data = data.data();
        if (!chunk_idat.dump(stream))
            return chunk_idat.chunk_data = nullptr, false;
        else
            chunk_idat.chunk_data = nullptr;

        // write iend chunk
        chunk chunk_iend;
        chunk_iend.length     = 0;
        chunk_iend.chunk_type = ct_IEND;
        chunk_iend.chunk_data = nullptr;
        if (!chunk_iend.dump(stream))
            return false;

        return true;
    }
};

bool png::load(const stream& stream, frames& frames) const
{
    try
    {
        canvas_adapter::pointer canvas = implement::load(stream);

        if (canvas)
        {
            frames.append({.canvas = canvas, .interval = 0});
            return true;
        }

        return false;
    }
    catch (const char* e)
    {
        return false;
    }
}

bool png::dump(const frames& frames, stream& stream) const
{
    return frames.size() ? implement::dump(frames[0].canvas, stream) : false;
}
