#ifndef CALCSHEET_H
#define CALCSHEET_H

#include "formula/formulaengine.h"
#include <QWidget>
#include <QTableView>

class CalcSheet : public QWidget
{
    Q_OBJECT
    FormulaEngine *formulaEngine;
    QTableView *tableView;
    QLineEdit *input;

public:
    CalcSheet(QWidget *parent = nullptr);
    ~CalcSheet() override;


protected:

private slots:

};

#endif // CALCSHEET_H
