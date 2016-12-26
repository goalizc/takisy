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
