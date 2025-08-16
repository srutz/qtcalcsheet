
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
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();
    int row = index.row();
    int col = index.column();
    int number = row + 1;
    int square = number * number;
    int previousSquare = (number - 1) * (number - 1);
    switch (col)
    {
    case 0:
        return QString::number(number);
    case 1:
        return QString::number(square);
    case 2:
        return QString::number(square - previousSquare);
    default:
        return QVariant();
    }
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
