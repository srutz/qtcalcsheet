
#include "calcsheetmodel.h"

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
    if (raw.isEmpty()) {
        m_data.remove({index.row(), index.column()});
    } else {
        m_data[{index.row(), index.column()}] = Cell(CellType::STRING, raw);
    }
    
    emit dataChanged(index, index, {role});
    return true;    
}
