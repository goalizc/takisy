#ifndef codec_h_20150108
#define codec_h_20150108

#include <string>

class codec
{
public:
    static unsigned int gbk2unicode(unsigned int gbk_code);
    static unsigned int unicode2gbk(unsigned int unicode_code);

public:
    static std::wstring gbk2unicode(const std::string& gbk);
    static std::string  unicode2gbk(const std::wstring& unicode);
    static std::wstring utf82unicode(const std::string& utf8);
    static std::string  unicode2utf8(const std::wstring& unicode);
};

#endif // codec_h_20150108
