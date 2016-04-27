#include <map>
#include <takisy/core/macro.h>
#include <takisy/gui/basic/cursor.h>

#ifdef __os_win__
#include <Windows.h>
#endif

void cursor::set(CursorType cursor)
{
#ifdef __os_win__
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
#endif
}

void cursor::set(const char* cursor_file)
{
#ifdef __os_win__
    static class file_cursor
    {
    public:
        ~file_cursor(void)
        {
            for (auto& pair : cursors_)
                DestroyCursor(pair.second);
        }

    public:
        void set(const char* cursor_file)
        {
            HCURSOR cursor = nullptr;

            if (cursors_.find(cursor_file) != cursors_.end())
                cursor = cursors_[cursor_file];
            else
            {
                cursor = LoadCursorFromFile(cursor_file);
                if (cursor)
                    cursors_[cursor_file] = cursor;
            }

            if (cursor)
                SetCursor(cursor);
        }

    private:
        std::map<const char*, HCURSOR> cursors_;
    } file_cursor;

    file_cursor.set(cursor_file);
#endif
}
