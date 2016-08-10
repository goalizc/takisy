#ifndef cursor_h_20151230
#define cursor_h_20151230

class cursor
{
public:
    enum CursorType
    {
        ctAppStarting,
        ctArrow,
        ctCross,
        ctHand,
        ctHelp,
        ctIBeam,
        ctIcon,
        ctNo,
        ctSizeAll,
        ctSizeNESW,
        ctSizeNS,
        ctSizeNWSE,
        ctSizeWE,
        ctUpArrow,
        ctWait,
    };

public:
    static void set(CursorType cursor);
    static void set(const char* cursor_file);
};

#endif // cursor_h_20151230
