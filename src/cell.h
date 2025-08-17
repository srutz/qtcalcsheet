
#ifndef CELL_H
#define CELL_H

#include <QVariant>

enum CellType {
    STRING = 1,
    NUMBER = 2,
    DATE = 3,
    BOOLEAN = 4,
    FORMULA = 5,
    ERROR = 6,
    EMPTY = 7
};

class Cell {
public:
    Cell();
    Cell(const CellType &type, const QVariant &value);
    Cell(const Cell &other);    

    CellType type() const;
    void setType(CellType type);
    QVariant value() const;
    void setValue(const QVariant &value);

private:
    CellType m_type;
    QVariant m_value;
};

#endif // CELL_H
