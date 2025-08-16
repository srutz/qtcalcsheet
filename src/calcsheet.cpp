#include "calcsheet.h"
#include "calcsheetmodel.h"
#include <QVBoxLayout>
#include <QHeaderView>

CalcSheet::CalcSheet(QWidget *parent) : QWidget(parent),
    formulaEngine(new FormulaEngine(this))
{
    this->tableView = new QTableView(this);
    auto layout = new QVBoxLayout(this);
    layout->addWidget(tableView);

    auto model = new CalcSheetModel(this);
    tableView->setModel(model);
    tableView->verticalHeader()->setVisible(true);
}

CalcSheet::~CalcSheet()
{
}

