#ifndef CALCSHEET_H
#define CALCSHEET_H

#include "formula/formulaengine.h"
#include <QWidget>
#include <QTableView>

class CalcSheet : public QWidget
{
    Q_OBJECT
    FormulaEngine *formulaEngine;

public:
    CalcSheet(QWidget *parent = nullptr);
    ~CalcSheet() override;


protected:

private slots:

private:
    QTableView *tableView;

};

#endif // CALCSHEET_H
