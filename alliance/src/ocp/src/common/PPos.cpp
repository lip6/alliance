#include "PPos.h"
#include <float.h>

const PPos PPos::Min(-DBL_MAX, -DBL_MAX);
const PPos PPos::Max(+DBL_MAX, +DBL_MAX);

bool  PPos::operator==(const PPos& pos) const
{
    return (X == pos.X) && (Y == pos.Y);
}

bool  PPos::operator!=(const PPos& pos) const
{
    return (X != pos.X) || (Y != pos.Y);
}

bool  PPos::operator<(const PPos& pos) const
{
    return (X < pos.X) || ((X == pos.X) && (Y < pos.Y));
}

bool  PPos::operator>(const PPos& pos) const
{
    return (X > pos.X) || ((X == pos.X) && (Y > pos.Y));
}

ostream&
PPos::Print(ostream& os) const
{
    return os << '(' << X << " , " << Y << ')';
}
