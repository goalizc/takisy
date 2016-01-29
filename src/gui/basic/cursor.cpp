#include <Windows.h>
#include <takisy/gui/basic/cursor.h>

void cursor::set(CursorType cursor)
{
    switch (cursor)
    {
    default:
    case ctArrow:       SetCursor(LoadCursor(nullptr, IDC_ARROW));       break;
    case ctAppStarting: SetCursor(LoadCursor(nullptr, IDC_APPSTARTING)); break;
    case ctCross:       SetCursor(LoadCursor(nullptr, IDC_CROSS));       break;
    case ctHand:        SetCursor(LoadCursor(nullptr, IDC_HAND));        break;
    case ctHelp:        SetCursor(LoadCursor(nullptr, IDC_HELP));        break;
    case ctIBeam:       SetCursor(LoadCursor(nullptr, IDC_IBEAM));       break;
    case ctIcon:        SetCursor(LoadCursor(nullptr, IDC_ICON));        break;
    case ctNo:          SetCursor(LoadCursor(nullptr, IDC_NO));          break;
    case ctSizeAll:     SetCursor(LoadCursor(nullptr, IDC_SIZEALL));     break;
    case ctSizeNESW:    SetCursor(LoadCursor(nullptr, IDC_SIZENESW));    break;
    case ctSizeNS:      SetCursor(LoadCursor(nullptr, IDC_SIZENS));      break;
    case ctSizeNWSE:    SetCursor(LoadCursor(nullptr, IDC_SIZENWSE));    break;
    case ctSizeWE:      SetCursor(LoadCursor(nullptr, IDC_SIZEWE));      break;
    case ctUpArrow:     SetCursor(LoadCursor(nullptr, IDC_UPARROW));     break;
    case ctWait:        SetCursor(LoadCursor(nullptr, IDC_WAIT));        break;
    }
}
