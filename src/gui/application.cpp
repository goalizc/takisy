#include <Windows.h>
#include <takisy/gui/application.h>

int application::exec(void)
{
    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
