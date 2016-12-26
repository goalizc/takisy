#include <algorithm>
#include <takisy/core/codec.h>

static struct codemap {
    unsigned short code;
    unsigned short map_code;
public:
    inline bool operator<(const codemap& codemap) const
        { return code < codemap.code; }
    inline bool operator<(unsigned short cmp_code) const
        { return code < cmp_code; }
} g_unicode2gbk_map[] = {
    #include "codec_unicode2gbk.h"
};

static constexpr unsigned int kUnicode2GBKMapCount =
            sizeof(g_unicode2gbk_map) / sizeof(*g_unicode2gbk_map);

unsigned int codec::gbk2unicode(unsigned int gbk_code)
{
    static codemap* gbk2unicode_map = nullptr;
    static struct gbk2unicode_map_generator
    {
        gbk2unicode_map_generator(codemap*& map)
        {
            map = new codemap [kUnicode2GBKMapCount];

            for (unsigned int i = 0; i < kUnicode2GBKMapCount; ++i)
            {
                map[i].code     = g_unicode2gbk_map[i].map_code;
                map[i].map_code = g_unicode2gbk_map[i].code;
            }

            std::sort(&map[0], &map[kUnicode2GBKMapCount]);
        }
    } bar(gbk2unicode_map);

    static codemap* begin = &gbk2unicode_map[0];
    static codemap* end   = &gbk2unicode_map[kUnicode2GBKMapCount];

    codemap* um = std::lower_bound(begin, end, gbk_code);
    if (um != end && um->code == gbk_code)
        return um->map_code;

    return 0;
}

unsigned int codec::unicode2gbk(unsigned int unicode)
{
    static codemap* begin = &g_unicode2gbk_map[0];
    static codemap* end   = &g_unicode2gbk_map[kUnicode2GBKMapCount];

    codemap* um = std::lower_bound(begin, end, unicode);
    if (um != end && um->code == unicode)
        return um->map_code;

    return 0;
}

std::wstring codec::gbk2unicode(const std::string& gbk)
{
    std::wstring result;

    for (unsigned int i = 0; i < gbk.size(); ++i)
    {
        unsigned int chr = static_cast<unsigned char>(gbk[i]);

        if (chr > 0x7f)
            chr = (chr << 8) | static_cast<unsigned char>(gbk[++i]);

        result += gbk2unicode(chr);
    }

    return result;
}

std::string codec::unicode2gbk(const std::wstring& unicode)
{
    std::string result;

    for (wchar_t uchr : unicode)
    {
        unsigned int chr = unicode2gbk(uchr);

        if (chr < 0x80)
            result += chr;
        else
        {
            result += chr >> 8;
            result += chr  & 0xff;
        }
    }

    return result;
}

std::wstring codec::utf82unicode(const std::string& utf8)
{
    std::wstring result;

    for (unsigned int i = 0; i < utf8.size(); )
    {
        wchar_t uchr = 0;

        if ((utf8[i] & 0x80) == 0x00)
            uchr = utf8[i++];
        else
        if ((utf8[i] & 0xe0) == 0xc0)
        {
            uchr  = (utf8[i++] & 0x1f) << 6;
            uchr |=  utf8[i++] & 0x3f;
        }
        else
        if ((utf8[i] & 0xf0) == 0xe0)
        {
            uchr  = (utf8[i++] & 0x0f) << 12;
            uchr |= (utf8[i++] & 0x3f) <<  6;
            uchr |=  utf8[i++] & 0x3f;
        }
        else
            break;

        result += uchr;
    }

    return result;
}

std::string codec::unicode2utf8(const std::wstring& unicode)
{
    std::string result;

    for (wchar_t uchr : unicode)
    {
        if (uchr < 0x80)
            result += uchr;
        else
        if (uchr < 0x800)
        {
            result += 0xc0 | (uchr >> 6);
            result += 0x80 | (uchr &  0x3f);
        }
        else
        if (uchr < 0x10000)
        {
            result += 0xe0 |  (uchr >> 12);
            result += 0x80 | ((uchr >>  6) & 0x3f);
            result += 0x80 | ((uchr >>  0) & 0x3f);
        }
        else
            result += static_cast<std::string::value_type>(0);
    }

    return result;
}
