#include <string>
#include <takisy/core/osdet.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/core/sys.h>

#ifdef __os_win__
#include <Windows.h>
#endif

static std::string default_codec__ = "utf-8";

unsigned long long sys::rdtsc(void)
{
    unsigned int lo, hi;

    asm volatile("rdtsc": "=a"(lo), "=d"(hi));

    return ((unsigned long long)hi << 32) | (unsigned long long)lo;
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

bool sys::key_pressed(VirtualKey vkey)
{
#ifdef __os_win__
    return 0x0100 & GetKeyState(vkey);
#else
    return false;
#endif
}

bool sys::key_pressed(const std::vector<VirtualKey>& vkeys)
{
    for (VirtualKey vkey : vkeys)
        if (!key_pressed(vkey))
            return false;

    return true;
}
