
#include "coord.h"

    
QString Coord::toString() const {
    return QString("(%1, %2)").arg(row).arg(column);
}