#ifndef codec_h_20150108
#define codec_h_20150108

#include <string>

class codec
{
public:
    static unsigned int gbk2uni(unsigned int gbk);
    static unsigned int uni2gbk(unsigned int uni);

public:
    static std::wstring gbk2uni(const std::string& gbk);
    static std::string  uni2gbk(const std::wstring& uni);
    static std::wstring utf82uni(const std::string& utf8);
    static std::string  uni2utf8(const std::wstring& uni);
};

#endif // codec_h_20150108
