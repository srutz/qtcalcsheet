#ifndef CALCSHEETMODEL_H
#define CALCSHEETMODEL_H

#include "cell.h"
#include "coord.h"
#include <QAbstractTableModel>


class CalcSheetModel : public QAbstractTableModel {
    Q_OBJECT

public:
    CalcSheetModel(QObject *parent = nullptr) : QAbstractTableModel(parent) {
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

private:
    int m_rows = 1000;
    int m_columns = 26 * 26;
    QHash<Coord, Cell> m_data;
};

#endif /* CALCSHEETMODEL_H */
