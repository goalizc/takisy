#ifndef define_h_20160303
#define define_h_20160303

enum Alignment
{
    // for compare operation
    aCenter      = 0x0000,
    aLeft        = 0x0001,
    aRight       = 0x0010,
    aTop         = 0x0100,
    aBottom      = 0x1000,
    aLeftTop     = aLeft  | aTop,
    aLeftBottom  = aLeft  | aBottom,
    aRightTop    = aRight | aTop,
    aRightBottom = aRight | aBottom,

    // for bit operation
    aHorizontal  = aLeft  | aRight,
    aVertical    = aTop   | aBottom,

    // for class layout
    aPrepose     = aLeftTop,
    aPostpose    = aRightBottom,
};

struct margin
{
    int left, top, right, bottom;
};

#endif // define_h_20160303
