#ifndef CALCSHEETMODEL_H
#define CALCSHEETMODEL_H

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

private:
    int m_rows = 1000;
    int m_columns = 26 * 26;
};

#endif /* CALCSHEETMODEL_H */
