#ifndef CALCSHEET_H
#define CALCSHEET_H

#include "formula/formulaengine.h"
#include "calcsheetmodel.h"
#include "calctableview.h"
#include <QLineEdit>
#include <QWidget>
#include <QTableView>

class CalcSheet : public QWidget
{
    Q_OBJECT
    FormulaEngine *m_formulaEngine;
    QLineEdit *m_input;
    CalcSheetModel *m_model;
    CalcTableView *m_tableView;
    bool m_changingInput = false;

public:
    CalcSheet(QWidget *parent = nullptr);
    ~CalcSheet() override;

    CalcSheetModel *model() const { return m_model; }

protected:

private slots:

};

#endif // CALCSHEET_H
