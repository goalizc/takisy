#include <algorithm>
#include <cstring>
#include <vector>
#include <takisy/core/codec.h>

struct codemap {
    unsigned short code;
    unsigned short map_code;
public:
    inline bool operator<(const codemap& codemap) const
        { return code < codemap.code; }
    inline bool operator<(unsigned short thscode) const
        { return code < thscode; }
};

static std::vector<codemap> g_uni2gbk_map = {
    #include "codec_uni2gbk.h"
};
static std::vector<codemap> g_gbk2uni_map = g_uni2gbk_map;

unsigned int codec::gbk2uni(unsigned int gbk)
{
    static const bool sorted = [](){
        for (codemap& codemap : g_gbk2uni_map)
            std::swap(codemap.code, codemap.map_code);
        std::sort(g_gbk2uni_map.begin(), g_gbk2uni_map.end());
        return true;
    }();

    std::vector<codemap>::iterator cmit =
        std::lower_bound(g_gbk2uni_map.begin(), g_gbk2uni_map.end(), gbk);
    if (cmit != g_gbk2uni_map.end() && cmit->code == gbk && sorted)
        return cmit->map_code;

    return 0;
}

unsigned int codec::uni2gbk(unsigned int uni)
{
    std::vector<codemap>::iterator cmit =
        std::lower_bound(g_uni2gbk_map.begin(), g_uni2gbk_map.end(), uni);
    if (cmit != g_uni2gbk_map.end() && cmit->code == uni)
        return cmit->map_code;

    return 0;
}

std::wstring codec::gbk2uni(const std::string& gbk)
{
    std::wstring result;

    for (unsigned int i = 0; i < gbk.size(); ++i)
    {
        unsigned int ch = static_cast<unsigned char>(gbk[i]);

        if (ch > 0x7f)
            ch = (ch << 8) | static_cast<unsigned char>(gbk[++i]);

        result += gbk2uni(ch);
    }

    return result;
}

std::string codec::uni2gbk(const std::wstring& uni)
{
    std::string result;

    for (wchar_t uchr : uni)
    {
        unsigned int ch = uni2gbk(uchr);

        if (ch < 0x80)
            result += ch;
        else
        {
            result += ch >> 8;
            result += ch  & 0xff;
        }
    }

    return result;
}

std::wstring codec::utf82uni(const std::string& utf8)
{
    std::wstring result;

    for (unsigned int i = 0; i < utf8.size(); )
    {
        wchar_t uchr = 0;

        if ((utf8[i] & 0x80) == 0x00)
            uchr  =  utf8[i++];
        else if ((utf8[i] & 0xe0) == 0xc0)
        {
            uchr  = (utf8[i++] & 0x1f) << 6;
            uchr |=  utf8[i++] & 0x3f;
        }
        else if ((utf8[i] & 0xf0) == 0xe0)
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

std::string codec::uni2utf8(const std::wstring& uni)
{
    std::string result;

    for (wchar_t uchr : uni)
    {
        if (uchr < 0x80)
            result += uchr;
        else if (uchr < 0x800)
        {
            result += 0xc0 | (uchr >> 6);
            result += 0x80 | (uchr &  0x3f);
        }
        else
        {
            result += 0xe0 |  (uchr >> 12);
            result += 0x80 | ((uchr >>  6) & 0x3f);
            result += 0x80 | ((uchr >>  0) & 0x3f);
        }
    }

    return result;
}
