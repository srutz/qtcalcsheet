
#include "calcsheetmodel.h"
#include "formulaengine.h"

static QString computeExcelColumnName(int col)
{
    QString columnName;
    while (col >= 0)
    {
        columnName.prepend(QChar('A' + (col % 26)));
        col = col / 26 - 1;
    }
    return columnName;
}

int CalcSheetModel::rowCount(const QModelIndex &parent) const { return m_rows; }

int CalcSheetModel::columnCount(const QModelIndex &parent) const { return m_columns; }

QLocale CalcSheetModel::locale() const{
    return m_locale;
};

void CalcSheetModel::setLocale(const QLocale &locale) {
    if (m_locale != locale) {
        m_locale = locale;
    }
}

QVariant CalcSheetModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    Coord coord(index.row(), index.column());
    if (!m_data.contains(coord)) {
        return QVariant(); // Return empty if no data exists for this cell
    }
    auto r = m_data.value(coord);
    return r.value();
}

QVariant CalcSheetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            return computeExcelColumnName(section);
        }
        else if (orientation == Qt::Vertical)
        { 
            return QString::number(section + 1); // Row numbers starting from 1
        }
    }
    return QVariant();
}

bool CalcSheetModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    auto raw = value.toString();
    if (raw.trimmed().isEmpty()) {
        m_data.remove({index.row(), index.column()});
    } else {
        auto cell = Cell::valueOf(m_locale, raw);
        m_data[{index.row(), index.column()}] = cell;
    }
    
    emit dataChanged(index, index, {role});
    return true;    
}

Qt::ItemFlags CalcSheetModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

void CalcSheetModel::dump() const
{
    qDebug() << "Dumping model data:";
    for (auto it = m_data.constBegin(); it != m_data.constEnd(); ++it) {
        const Coord &coord = it.key();
        const Cell &cell = it.value();
        qDebug() << QString("Cell(%1, %2): Type=%3, Value=%4")
                        .arg(coord.row)
                        .arg(coord.column)
                        .arg(cell.type())
                        .arg(cell.value().toString());
        if (cell.type() == CellType::FORMULA) {
            FormulaEngine engine;
            qDebug() << "  Formula text:" << cell.value().toString();
            auto result = engine.evaluate(cell.value().toString(), {});
            qDebug() << "  Formula:" << result;
        }
    }
}
