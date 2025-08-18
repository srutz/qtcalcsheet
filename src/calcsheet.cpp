#include "calcsheet.h"
#include "calcsheetmodel.h"
#include "util.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QLabel>
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

    auto topPanel = new QWidget(this);
    mainLayout->addWidget(topPanel);
    auto topLayout = new QHBoxLayout(topPanel);
    topLayout->setContentsMargins(0, 0, 0, 0);

    auto topLabel = new QLabel("f(x)", this);
    topLayout->addWidget(topLabel);

    this->input = new QLineEdit(this);
    topLayout->addWidget(input);

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
        //auto model = const_cast<QAbstractItemModel*>(current.model());
        //model->setData(current, input->text());
        // empty the input and advance selection one row
        //input->clear();
        Util::selectNextTableRow(tableView);
    });

    // while typing in the table, also update the input field
    connect(tableView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this](const QModelIndex &current) {
        auto v = current.data().toString();
        qDebug() << "Current table value:" << v;
        this->input->setText(v);
    });
    // while typing in the inputfield also update the current table value
    connect(input, &QLineEdit::textChanged, this, [this](const QString &text) {
        QModelIndex current = tableView->currentIndex();
        if (!current.isValid()) {
            return;
        }
        // update the model
        auto model = const_cast<QAbstractItemModel*>(current.model());
        model->setData(current, text);
    });
}

CalcSheet::~CalcSheet()
{
}

