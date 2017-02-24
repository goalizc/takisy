#ifndef define_h_20160303
#define define_h_20160303

#include <takisy/cgl/basic/point.h>
#include <takisy/cgl/basic/size.h>
#include <takisy/cgl/basic/rect.h>

typedef point Point;
typedef sizeu Size;
typedef rect  Rect;

enum Alignment
{
    // for compare operation
    aCenter      = 0x00000,
    aLeft        = 0x00001,
    aRight       = 0x00010,
    aTop         = 0x00100,
    aBottom      = 0x01000,
    aUndefined   = 0x10000,

    aLeftTop     = aLeft  | aTop,
    aTopLeft     = aLeft  | aTop,
    aLeftBottom  = aLeft  | aBottom,
    aBottomLeft  = aLeft  | aBottom,
    aRightTop    = aRight | aTop,
    aTopRight    = aRight | aTop,
    aRightBottom = aRight | aBottom,
    aBottomRight = aRight | aBottom,

    // for bit operation
    aHorizontal  = aLeft  | aRight,
    aVertical    = aTop   | aBottom,

    // for layout
    aPrepose     = aLeftTop,
    aPostpose    = aRightBottom,
};

enum SelectionMode
{
    smNoSelection,
    smSingleSelection,
    smMultiSelection,
    smExtendedSelection,
    smContiguousSelection,
};

enum SelectionBehavior
{
    sbSelectItems,
    sbSelectRows,
    sbSelectColumns,
};

enum EditTrigger
{
    etNoEditTrigger   = 0x0000,
    etDoubleClicked   = 0x0001,
    etSelectedClicked = 0x0002,
    etEnterPressed    = 0x0004,
    etAnyKeyPressed   = 0x0008,
    etAllEditTriggers = 0x000f,
};

struct Margin
{
    int left, top, right, bottom;
};

#endif // define_h_20160303
