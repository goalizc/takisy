#include <string>
#include <takisy/core/macro.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/core/sys.h>

#ifdef OS_WIN
#include <Windows.h>
#endif

static std::string default_codec__ = "utf-8";

unsigned long long sys::rdtsc(void)
{
    unsigned int lo, hi;

    asm volatile("rdtsc": "=a"(lo), "=d"(hi));

    return ((unsigned long long)hi << 32) | (unsigned long long)lo;
}

bool sys::key_pressed(VirtualKey virtual_key)
{
#ifdef OS_WIN
    return 0x0100 & GetKeyState(virtual_key);
#else
    return false;
#endif
}

const char* sys::default_codec(void)
{
    return default_codec__.c_str();
}

void sys::default_codec(const char* _codec)
{
    for (const std::string& codec : stralgo::codecs())
        if (codec == _codec)
            default_codec__ = _codec;
}
