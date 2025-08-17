#ifndef COORD_H
#define COORD_H

#include <QHash>
#include <QDebug>

class Coord {
public:
    int row;
    int column;
    
    Coord() : row(0), column(0) {}
    Coord(int r, int c) : row(r), column(c) {}
    Coord(const Coord& other) : row(other.row), column(other.column) {}
    
    Coord& operator=(const Coord& other) {
        if (this != &other) {
            row = other.row;
            column = other.column;
        }
        return *this;
    }
    
    bool operator==(const Coord& other) const {
        return row == other.row && column == other.column;
    }
    
    bool operator!=(const Coord& other) const {
        return !(*this == other);
    }
    
    bool operator<(const Coord& other) const {
        if (row != other.row) {
            return row < other.row;
        }
        return column < other.column;
    }
    
    QString toString() const;
};


// Hash function for Coord (required for QHash)
inline uint qHash(const Coord& coord, uint seed = 0) {
    return qHash(coord.row, seed) ^ qHash(coord.column, seed + 1);
}

// Debug stream operator
inline QDebug operator<<(QDebug debug, const Coord& coord) {
    debug.nospace() << "Coord(" << coord.row << ", " << coord.column << ")";
    return debug;
}

#endif // COORD_H
