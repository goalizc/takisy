#ifndef codec_h_20150108
#define codec_h_20150108

class codec
{
public:
    static unsigned int gbk2unicode(unsigned int gbk_code);
    static unsigned int unicode2gbk(unsigned int unicode_code);
};

#endif // codec_h_20150108
