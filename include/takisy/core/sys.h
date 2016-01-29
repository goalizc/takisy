#ifndef sys_h_20131122
#define sys_h_20131122

#include <takisy/cgl/basic/rect.h>
#include <takisy/cgl/basic/color.h>

class sys
{
public:
    enum VirtualKey
    {
        vkLButton           = 0x01, vkRButton    = 0x02, vkCancel      = 0x03,
        vkMButton           = 0x04, vkXButton1   = 0x05, vkXButton2    = 0x06,
        vkBack              = 0x08, vkTab        = 0x09, vkClear       = 0x0C,
        vkReturn            = 0x0D, vkShift      = 0x10, vkControl     = 0x11,
        vkMenu              = 0x12, vkPause      = 0x13, vkCapital     = 0x14,
        vkKana              = 0x15, vkHanguel    = 0x15, vkHangul      = 0x15,
        vkJunja             = 0x17, vkFinal      = 0x18, vkHanja       = 0x19,
        vkKanji             = 0x19, vkEscape     = 0x1B, vkConvert     = 0x1C,
        vkNonconvert        = 0x1D, vkAccept     = 0x1E, vkModechange  = 0x1F,
        vkSpace             = 0x20, vkPrior      = 0x21, vkNext        = 0x22,
        vkEnd               = 0x23, vkHome       = 0x24, vkLeft        = 0x25,
        vkUp                = 0x26, vkRight      = 0x27, vkDown        = 0x28,
        vkSelect            = 0x29, vkPrint      = 0x2A, vkExecute     = 0x2B,
        vkSnapshot          = 0x2C, vkInsert     = 0x2D, vkDelete      = 0x2E,
        vkHelp              = 0x2F, vkKey0       = 0x30, vkKey1        = 0x31,
        vkKey2              = 0x32, vkKey3       = 0x33, vkKey4        = 0x34,
        vkKey5              = 0x35, vkKey6       = 0x36, vkKey7        = 0x37,
        vkKey8              = 0x38, vkKey9       = 0x39, vkKeyA        = 0x41,
        vkKeyB              = 0x42, vkKeyC       = 0x43, vkKeyD        = 0x44,
        vkKeyE              = 0x45, vkKeyF       = 0x46, vkKeyG        = 0x47,
        vkKeyH              = 0x48, vkKeyI       = 0x49, vkKeyJ        = 0x4A,
        vkKeyK              = 0x4B, vkKeyL       = 0x4C, vkKeyM        = 0x4D,
        vkKeyN              = 0x4E, vkKeyO       = 0x4F, vkKeyP        = 0x50,
        vkKeyQ              = 0x51, vkKeyR       = 0x52, vkKeyS        = 0x53,
        vkKeyT              = 0x54, vkKeyU       = 0x55, vkKeyV        = 0x56,
        vkKeyW              = 0x57, vkKeyX       = 0x58, vkKeyY        = 0x59,
        vkKeyZ              = 0x5A, vkLwin       = 0x5B, vkRwin        = 0x5C,
        vkApps              = 0x5D, vkSleep      = 0x5F, vkNumpad0     = 0x60,
        vkNumpad1           = 0x61, vkNumpad2    = 0x62, vkNumpad3     = 0x63,
        vkNumpad4           = 0x64, vkNumpad5    = 0x65, vkNumpad6     = 0x66,
        vkNumpad7           = 0x67, vkNumpad8    = 0x68, vkNumpad9     = 0x69,
        vkMultiply          = 0x6A, vkAdd        = 0x6B, vkSeparator   = 0x6C,
        vkSubtract          = 0x6D, vkDecimal    = 0x6E, vkDivide      = 0x6F,
        vkF1                = 0x70, vkF2         = 0x71, vkF3          = 0x72,
        vkF4                = 0x73, vkF5         = 0x74, vkF6          = 0x75,
        vkF7                = 0x76, vkF8         = 0x77, vkF9          = 0x78,
        vkF10               = 0x79, vkF11        = 0x7A, vkF12         = 0x7B,
        vkF13               = 0x7C, vkF14        = 0x7D, vkF15         = 0x7E,
        vkF16               = 0x7F, vkF17        = 0x80, vkF18         = 0x81,
        vkF19               = 0x82, vkF20        = 0x83, vkF21         = 0x84,
        vkF22               = 0x85, vkF23        = 0x86, vkF24         = 0x87,
        vkNumlock           = 0x90, vkScroll     = 0x91, vkLShift      = 0xA0,
        vkRShift            = 0xA1, vkLControl   = 0xA2, vkRControl    = 0xA3,
        vkLMenu             = 0xA4, vkRMenu      = 0xA5, vkBrowserBack = 0xA6,
        vkBrowserForward    = 0xA7, vkPlay       = 0xFA, vkBrowserStop = 0xA9,
        vkBrowserSearch     = 0xAA, vkCrsel      = 0xF7, vkBrowserHome = 0xAC,
        vkVolumeMute        = 0xAD, vkVolumeDown = 0xAE, vkVolumeUp    = 0xAF,
        vkMediaNextTrack    = 0xB0, vkOem7       = 0xDE, vkMediaStop   = 0xB2,
        vkMediaPlayPause    = 0xB3, vkLaunchMail = 0xB4, vkOemClear    = 0xFE,
        vkLaunchApp1        = 0xB6, vkLaunchApp2 = 0xB7, vkOem1        = 0xBA,
        vkOemPlus           = 0xBB, vkOemComma   = 0xBC, vkOemMinus    = 0xBD,
        vkOemPeriod         = 0xBE, vkOem2       = 0xBF, vkOem3        = 0xC0,
        vkOem4              = 0xDB, vkOem5       = 0xDC, vkOem6        = 0xDD,
        vkMediaPrevTrack    = 0xB1, vkOem8       = 0xDF, vkOem102      = 0xE2,
        vkProcesskey        = 0xE5, vkPacket     = 0xE7, vkAttn        = 0xF6,
        vkBrowserFavorites  = 0xAB, vkExsel      = 0xF8, vkEreof       = 0xF9,
        vkBrowserRefresh    = 0xA8, vkZoom       = 0xFB, vkNoname      = 0xFC,
        vkLaunchMediaSelect = 0xB5, vkPa1        = 0xFD,
    };

    enum MouseButton
    {
        mbNone     = 0x00,
        mbLButton  = 0x01,
        mbMButton  = 0x02,
        mbRButton  = 0x04,
        mbXButton1 = 0x08,
        mbXButton2 = 0x10,
    };

public:
    static unsigned long long rdtsc(void);
    static bool key_pressed(VirtualKey virtual_key);

    static unsigned int screen_width(void);
    static unsigned int screen_height(void);
    static rect         work_area(void);

    static color        default_interface_color(void);
    static const char*  default_codec(void);

public:
    static void default_interface_color(const color& color);
    static void default_codec(const char* codec);
};

#endif //sys_h_20131122
