#include <takisy/algorithm/stralgo.h>
#include <takisy/core/sys.h>

static std::string default_codec__ = "utf8";

unsigned long long sys::rdtsc(void)
{
    unsigned int lo, hi;

    asm volatile("rdtsc": "=a"(lo), "=d"(hi));

    return ((unsigned long long)hi << 32) | (unsigned long long)lo;
}

std::string sys::default_codec(void)
{
    return default_codec__.c_str();
}

std::string sys::default_codec(const std::string& codec)
{
    std::string default_codec = default_codec__;

    for (const std::string& cdc : stralgo::codecs())
        if (cdc == codec)
            default_codec__ = codec;

    return default_codec;
}
