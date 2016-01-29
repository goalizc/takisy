#include <algorithm>
#include <takisy/core/codec.h>

static struct code_map {
    unsigned short code;
    unsigned short map_code;
public:
    inline bool operator<(const code_map& code_map) const
        { return code < code_map.code; }
    inline bool operator<(unsigned short cmp_code) const
        { return code < cmp_code; }
} g_unicode2gbk_map[] = {
    #include "codec_unicode2gbk.h"
};

static constexpr unsigned int kUnicode2GBKMapCount =
            sizeof(g_unicode2gbk_map) / sizeof(*g_unicode2gbk_map);

unsigned int codec::gbk2unicode(unsigned int gbk_code)
{
    static code_map* gbk2unicode_map = nullptr;
    static struct gbk2unicode_map_generator
    {
        gbk2unicode_map_generator(code_map*& map)
        {
            map = new code_map [kUnicode2GBKMapCount];

            for (unsigned int i = 0; i < kUnicode2GBKMapCount; ++i)
            {
                map[i].code     = g_unicode2gbk_map[i].map_code;
                map[i].map_code = g_unicode2gbk_map[i].code;
            }

            std::sort(&map[0], &map[kUnicode2GBKMapCount]);
        }
    } bar(gbk2unicode_map);

    static code_map* begin = &gbk2unicode_map[0];
    static code_map* end   = &gbk2unicode_map[kUnicode2GBKMapCount];

    code_map* um = std::lower_bound(begin, end, gbk_code);
    if (um != end && um->code == gbk_code)
        return um->map_code;

    return 0;
}

unsigned int codec::unicode2gbk(unsigned int unicode_code)
{
    static code_map* begin = &g_unicode2gbk_map[0];
    static code_map* end   = &g_unicode2gbk_map[kUnicode2GBKMapCount];

    code_map* um = std::lower_bound(begin, end, unicode_code);
    if (um != end && um->code == unicode_code)
        return um->map_code;

    return 0;
}
