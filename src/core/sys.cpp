#include <string>
#include <Windows.h>
#include <takisy/algorithm/stralgo.h>
#include <takisy/core/sys.h>

static std::string __sDefaultCodec = "utf-8";
static color       __sDefaultInterfaceColor = color::sea_green();

unsigned long long sys::rdtsc(void)
{
    unsigned int lo, hi;

    asm volatile("rdtsc": "=a"(lo), "=d"(hi));

    return ((unsigned long long)hi << 32) | (unsigned long long)lo;
}

bool sys::key_pressed(VirtualKey virtual_key)
{
    return 0x0100 & GetKeyState(virtual_key);
}

unsigned int sys::screen_width(void)
{
    return GetSystemMetrics(SM_CXSCREEN);
}

unsigned int sys::screen_height(void)
{
    return GetSystemMetrics(SM_CYSCREEN);
}

rect sys::work_area(void)
{
    RECT RECT;

    if (!SystemParametersInfo(SPI_GETWORKAREA, 0, &RECT, 0))
        return rect(0, 0, screen_width(), screen_height());

    return RECT;
}

color sys::default_interface_color(void)
{
    return __sDefaultInterfaceColor;
}

const char* sys::default_codec(void)
{
    return __sDefaultCodec.c_str();
}

void sys::default_interface_color(const color& color)
{
    __sDefaultInterfaceColor = color;
}

void sys::default_codec(const char* _codec)
{
    for (const std::string& codec : stralgo::codecs())
        if (codec == _codec)
            __sDefaultCodec = _codec;
}
