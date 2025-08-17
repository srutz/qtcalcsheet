#include "calcsheet.h"
#include "calcsheetmodel.h"
#include "util.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QLabel>

CalcSheet::CalcSheet(QWidget *parent) : QWidget(parent),
    formulaEngine(new FormulaEngine(this))
{
    this->tableView = new QTableView(this);
    auto mainLayout = new QVBoxLayout(this);

    auto topLayout = new QHBoxLayout(this);
    topLayout->setContentsMargins(0, 0, 0, 0);

    auto topLabel = new QLabel("f(x)", this);
    topLayout->addWidget(topLabel);

    this->input = new QLineEdit(this);
    topLayout->addWidget(input);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(tableView);

    // setup table
    auto model = new CalcSheetModel(this);
    tableView->setModel(model);
    tableView->verticalHeader()->setVisible(true);

    // make table editable
    tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed | QAbstractItemView::AnyKeyPressed | QAbstractItemView::SelectedClicked);
    tableView->verticalHeader()->setSectionsMovable(true);
    tableView->horizontalHeader()->setSectionsMovable(true);

    // navigate in table in excel style using keyboard
    tableView->installEventFilter(this);

    // wire up table selection to input
    connect(tableView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this](const QModelIndex &current) {
        this->input->setText(current.data().toString());
    });


    // wire up enter press in the input
    connect(input, &QLineEdit::returnPressed, this, [this]() {
        QModelIndex current = tableView->currentIndex();
        if (!current.isValid()) {
            return;
        }
        // update the model
        auto model = const_cast<QAbstractItemModel*>(current.model());
        model->setData(current, input->text());
        // empty the input and advance selection one row
        input->clear();
        Util::selectNextTableRow(tableView);
    });
}

CalcSheet::~CalcSheet()
{
}

