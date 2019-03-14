#include <cstring>
#include "rfc1951.h"

namespace rfc1951 {

/* Order of the bit length code lengths */
static constexpr unsigned int kBorder[] = {
        16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
/* Copy lengths for literal codes 257..285 */
static constexpr unsigned int kCPlens[] = {
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 259};
/* Extra bits for literal codes 257..285 */
static constexpr unsigned int kCPlext[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
        3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
/* Copy offsets for distance codes 0..29 */
static constexpr unsigned int kCPdist[] = {
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
        257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
        8193, 12289, 16385, 24577, 32768};
/* Extra bits for distance codes */
static constexpr unsigned int kCPdext[] = {
        0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
        7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
        12, 12, 13, 13};

struct match_result
{
    short length, distance;
};

struct huffman_tree
{
    unsigned int clen, cmin, cmax; // code length, minimal/maximal code
    unsigned int* values;          // literal/length/distance
    struct huffman_tree* next;     // next huffman tree
};

static const int kHashTableSize = 1 << 15;
static const int kMinMatchCount = 3;
static const int kWindowSize    = 1 << 15;
static const int kQuality       = 16;

unsigned int bit_reverse(unsigned int bits, unsigned int count)
{
    unsigned int result = 0;

    while (count--)
    {
        result = (result << 1) | (bits & 1);
        bits >>= 1;
    }

    return result;
}

match_result match(stretchy_buffer<const unsigned char*>& positions,
                   const unsigned char* curr,
                   const unsigned char* end)
{
    const long long limit = end - curr;
    match_result result = {kMinMatchCount, -1};

    for (const unsigned char* position : positions)
    {
        if (curr - position >= kWindowSize)
            continue;

        int length = 0;

        for ( ; length < 258 && length < limit; ++length)
            if (position[length] != curr[length])
                break;

        if (length >= result.length)
            result.length = length, result.distance = curr - position;
    }

    if (positions.size() > kQuality * 2)
    {
        memmove(positions.data(), positions.data() + kQuality,
                positions.size() - kQuality);
        positions.resize(positions.size() - kQuality);
    }

    positions.append(curr);

    return result;
}

inline unsigned int hash(const unsigned char* data)
{
    unsigned int hash = data[0] + (data[1] << 8) + (data[2] << 16);

    hash ^= hash <<  3; hash += hash >>  5;
    hash ^= hash <<  4; hash += hash >> 17;
    hash ^= hash << 25; hash += hash >>  6;

    return hash & (kHashTableSize - 1);
}

inline void push_fixed_huffcode(bit_buffer::output& buffer, unsigned int value)
{
    return value <= 143 ? buffer.write(bit_reverse(value +  48, 8), 8)
         : value <= 255 ? buffer.write(bit_reverse(value + 256, 9), 9)
         : value <= 279 ? buffer.write(bit_reverse(value - 256, 7), 7)
                        : buffer.write(bit_reverse(value -  88, 8), 8);
}

inline void push_fixed_length(bit_buffer::output& buffer, unsigned int length)
{
    unsigned int i = 0;

    for ( ; length >= kCPlens[i + 1]; ++i);

    push_fixed_huffcode(buffer, i + 257);

    if (kCPlext[i])
        buffer.write(length - kCPlens[i], kCPlext[i]);
}

inline void push_fixed_distance(bit_buffer::output& buffer,
                                unsigned int distance)
{
    unsigned int i = 0;

    for ( ; distance >= kCPdist[i + 1]; ++i);

    buffer.write(bit_reverse(i, 5), 5);

    if (kCPdext[i])
        buffer.write(distance - kCPdist[i], kCPdext[i]);
}

void deflate_store(const unsigned char* buffer, unsigned int length,
                   bit_buffer::output& output)
{
    while (length)
    {
        const unsigned int cMaxSL = (1 << 15) - 1;
        unsigned int store_length = length > cMaxSL ? cMaxSL : length;

        output.write(store_length >= length, 1);
        output.write(0, 2);
        output.align();
        output.write(static_cast<unsigned short>( store_length), 16);
        output.write(static_cast<unsigned short>(~store_length), 16);

        for (unsigned int i = 0; i < store_length; ++i)
            output.write(buffer[i], 8);

        buffer += store_length;
        length -= store_length;
    }
}

void deflate_fixed(const unsigned char* buffer, int length,
                   bit_buffer::output& output)
{
    output.write(1, 1);
    output.write(1, 2);

    stretchy_buffer<const unsigned char*> hash_tables[kHashTableSize];
    const unsigned char *curr = buffer, *end = buffer + length;
    match_result result1, result2;

    while (curr < end - kMinMatchCount)
    {
        result1 = match(hash_tables[hash(curr)], curr, end);

        if (result1.distance < 0)
            push_fixed_huffcode(output, *curr++);
        else
        {
            result2 = match(hash_tables[hash(curr + 1)], curr + 1, end);

            if (result2.length > result1.length)
                push_fixed_huffcode(output, *curr++), result1 = result2;

            push_fixed_length(output, result1.length);
            push_fixed_distance(output, result1.distance);

            curr += result1.length;
        }
    }

    for ( ; curr < end; ++curr)
        push_fixed_huffcode(output, *curr);

    push_fixed_huffcode(output, 256);
}

inline void deflate_dynamic(const unsigned char* buffer, unsigned int length,
                            bit_buffer::output& output)
{
    return deflate_fixed(buffer, length, output);
}

void deflate(const unsigned char* buffer, unsigned int length,
             bit_buffer::output& output, unsigned int level)
{
    switch (level)
    {
    case  0: deflate_store  (buffer, length, output); break;
    case  1: deflate_fixed  (buffer, length, output); break;
    case  2: deflate_dynamic(buffer, length, output); break;
    default: throw ecInvalidBlockType;
    }

    output.align();
}

huffman_tree* build_huffman_tree(unsigned int* lengths, unsigned int count)
{
    stretchy_buffer<unsigned int> lengthss[17];
    huffman_tree* huft_h = nullptr; // head of huffman tree
    huffman_tree* huft_t = nullptr; // tail of huffman talbe

    for (unsigned int i = 0; i < count; ++i)
        lengthss[lengths[i]].append(i);
    lengthss[0].resize(0);

    for (unsigned int code = 0, i = 1; i < 17; ++i)
    {
        code = (code + lengthss[i - 1].size()) << 1;

        if (lengthss[i].size())
        {
            huffman_tree* new_huft = new huffman_tree {
                .clen   = i,
                .cmin   = code,
                .cmax   = code + lengthss[i].size(),
                .values = new unsigned int [lengthss[i].size()],
                .next   = nullptr,
            };

            huft_t = huft_t ? huft_t->next = new_huft : huft_h = new_huft;

            for (unsigned int j = 0; j < lengthss[i].size(); ++j)
                huft_t->values[j] = lengthss[i][j];
        }
    }

    return huft_h;
}

void free_huffman_tree(huffman_tree* huft)
{
    while (huft)
    {
        delete [] huft->values;
        huffman_tree* next_huft = huft->next;
        delete huft;
        huft = next_huft;
    }
}

int inflate_code(bit_buffer::input& input, huffman_tree* huft)
{
    unsigned int last_clen = 0;
    unsigned int code      = 0;

    while (huft)
    {
        unsigned int n = huft->clen - last_clen;

        code = (code << n) | bit_reverse(input.read(n), n);

        if (code < huft->cmax)
            return huft->values[code - huft->cmin];
        else
            last_clen = huft->clen, huft = huft->next;
    }

    return -1;
}

void inflate_codes(bit_buffer::input& input, buffer_type& output,
                   huffman_tree* lhuft, huffman_tree* dhuft)
{
    while (true)
    {
        int value = inflate_code(input, lhuft);

        if (value < 0)
            throw ecInvalidCode;
        else if (value < 256)
            output.append(value);
        else if (value == 256)
            break;
        else
        {
            int cpl_index = value - 257;
            unsigned int length = kCPlens[cpl_index];

            if (kCPlext[cpl_index])
                length += input.read(kCPlext[cpl_index]);

            int dcode = inflate_code(input, dhuft);

            if (dcode < 0)
                throw ecInvalidCode;
            else
            {
                unsigned int distance = kCPdist[dcode];

                if (kCPdext[dcode])
                    distance += input.read(kCPdext[dcode]);

                unsigned int back = output.size() - distance;

                for (unsigned int i = 0; i < length; ++i)
                    output.append((unsigned char)output[back + i]);
            }
        }
    }
}

void inflate_store(bit_buffer::input& input, buffer_type& output)
{
    input.align();

    unsigned short LEN = input.read(16);

    if (LEN != static_cast<unsigned short>(~input.read(16)))
        throw ecInvalidStoreLEN;

    for (unsigned int i = 0; i < LEN; ++i)
        output.append(input.read(8));
}

void inflate_fixed(bit_buffer::input& input, buffer_type& output)
{
    unsigned int lengths[288];

    for (int i =   0; i <= 143; ++i) lengths[i] = 8;
    for (int i = 144; i <= 255; ++i) lengths[i] = 9;
    for (int i = 256; i <= 279; ++i) lengths[i] = 7;
    for (int i = 280; i <= 287; ++i) lengths[i] = 8;
    huffman_tree* lhuft = build_huffman_tree(lengths, 288);

    for (int i =   0; i <=  29; ++i) lengths[i] = 5;
    huffman_tree* dhuft = build_huffman_tree(lengths, 30);

    inflate_codes(input, output, lhuft, dhuft);

    free_huffman_tree(dhuft);
    free_huffman_tree(lhuft);
}

void inflate_dynamic(bit_buffer::input& input, buffer_type& output)
{
    unsigned int HLIT  = input.read(5) + 257;
    unsigned int HDIST = input.read(5) +   1;
    unsigned int HCLEN = input.read(4) +   4;

    unsigned int lengths[288 + 32];
    unsigned int i;

    for (i = 0; i < HCLEN; ++i)
        lengths[kBorder[i]] = input.read(3);
    for (     ; i <    19; ++i)
        lengths[kBorder[i]] = 0;

    huffman_tree* chuft = build_huffman_tree(lengths, 19);
    unsigned int sum_HLIT_HDIST = HLIT + HDIST;
    int length, last_length = 0;

    for (i = 0; i < sum_HLIT_HDIST; )
    {
        length = inflate_code(input, chuft);

        if (length < 0)
            throw ecInvalidCodeLength;
        else if (length < 16)
            last_length = lengths[i++] = length;
        else if (length == 16)
        {
            unsigned int repeat_count = 3 + input.read(2);

            while (repeat_count--)
                lengths[i++] = last_length;
        }
        else
        {
            unsigned int zero_count;

            switch (length)
            {
            case 17: zero_count =  3 + input.read(3); break;
            case 18: zero_count = 11 + input.read(7); break;
            default: throw ecInvalidCodeLength;
            }

            while (zero_count--)
                lengths[i++] = 0;
        }
    }

    huffman_tree* lhuft = build_huffman_tree(lengths,        HLIT );
    huffman_tree* dhuft = build_huffman_tree(lengths + HLIT, HDIST);

    inflate_codes(input, output, lhuft, dhuft);

    free_huffman_tree(dhuft);
    free_huffman_tree(lhuft);
    free_huffman_tree(chuft);
}

bool inflate_block(bit_buffer::input& input, buffer_type& output)
{
    unsigned int BFINAL = input.read(1);
    unsigned int BTYPE  = input.read(2);

    switch (BTYPE)
    {
    case  0: inflate_store  (input, output); break;
    case  1: inflate_fixed  (input, output); break;
    case  2: inflate_dynamic(input, output); break;
    default: throw ecInvalidBlockType;
    }

    return BFINAL;
}

buffer_type inflate(bit_buffer::input& input)
{
    buffer_type output(0);

    try
    {
        while (!inflate_block(input, output));

        input.align();
    }
    catch (ExceptionCode ec)
    {
        return buffer_type();
    }

    return output;
}

} // end of namespace rfc1951
