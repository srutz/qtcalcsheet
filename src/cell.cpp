#include "cell.h"

Cell::Cell() : m_type(CellType::EMPTY), m_value(QVariant()) {

}

Cell::Cell(const CellType &type, const QVariant &value) : m_type(type), m_value(value) {
}

Cell::Cell(const Cell &other) : m_type(other.m_type), m_value(other.m_value) {
}

CellType Cell::type() const { return m_type; }
void Cell::setType(CellType type) { m_type = type; }

QVariant Cell::value() const { return m_value; }
void Cell::setValue(const QVariant &value) { m_value = value; }
