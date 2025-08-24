#include "cell.h"
#include "util.h"
#include "formulaengine.h"
#include <QDate>

Cell Cell::valueOf(const QLocale &locale, const QString &raw) {
    QString trimmed = raw.trimmed();
    if (trimmed.isEmpty()) {
        return Cell(CellType::EMPTY, QVariant());
    }
    // Check for formula
    if (trimmed.startsWith('=')) {
        FormulaEngine engine;
        auto formula = trimmed.mid(1).trimmed();
        auto computedValue = engine.evaluate(formula, {});
        auto cell = Cell(CellType::FORMULA, computedValue);
        cell.setFormula(formula);
        return cell;
    }
    // Check for boolean
    if (trimmed.compare("TRUE", Qt::CaseInsensitive) == 0) {
        return Cell(CellType::BOOLEAN, true);
    }
    if (trimmed.compare("FALSE", Qt::CaseInsensitive) == 0) {
        return Cell(CellType::BOOLEAN, false);
    }
    // Check for number
    {
        auto n = Util::parseNumber(locale, trimmed);
        if (n.isValid() && (n.type() == QVariant::Double || n.type() == QVariant::Int || n.type() == QVariant::LongLong)) {
            return Cell(CellType::NUMBER, n);
        }
    }
    // Check for date (simple ISO format check)
    QDate date = QDate::fromString(trimmed, Qt::ISODate);
    if (date.isValid()) {
        return Cell(CellType::DATE, date);
    }
    // Default to string
    return Cell(CellType::STRING, trimmed);
}

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

QString Cell::formula() const { return m_formula; }
void Cell::setFormula(const QString &formula) { m_formula = formula; }
