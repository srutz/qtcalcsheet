#include "calcsheet.h"
#include "calcsheetmodel.h"
#include "util.h"
#include "liveeditdelegate.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>
#include <QCoreApplication>

CalcSheet::CalcSheet(QWidget *parent) : QWidget(parent),
    formulaEngine(new FormulaEngine(this))
{
    this->tableView = new QTableView(this);
    // Set up live edit delegate
    auto liveDelegate = new LiveEditDelegate(this);
    tableView->setItemDelegate(liveDelegate);
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
        tableView->setFocus();
        Util::selectNextTableRow(tableView);
    });

    // while typing in the table, also update the input field
    connect(liveDelegate, &LiveEditDelegate::textEdited, this, [this](const QModelIndex &index, const QString &text) {
        //qDebug() << "Live text change in table cell" << index << ":" << text;
        this->input->setText(text);
    });
    connect(liveDelegate, &LiveEditDelegate::editingFinished, this, [this](const QModelIndex &index, const QString &text) {
        Util::selectNextTableRow(tableView);
        // also stop editing in the table
        QTimer::singleShot(100, this, [this]() {
            //tableView->model()->revert();
            //tableView->clearFocus();
            // invoke esc key press on the tableview
            QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
            QCoreApplication::postEvent(tableView, event);
            qDebug() << "run escape key press event";
        });
    });

    // Track committed changes
    connect(model, &QAbstractItemModel::dataChanged, this, [this](const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
        //qDebug() << "Committed table change from" << topLeft << "to" << bottomRight;
    });

    /*
    connect(tableView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this](const QModelIndex &current) {
        auto v = current.data().toString();
        qDebug() << "Current table value:" << v;
        this->input->setText(v);
    });
    */
    // while typing in the inputfield also update the current table value
    connect(input, &QLineEdit::textChanged, this, [this](const QString &text) {
        QModelIndex current = tableView->currentIndex();
        qDebug() << "Input text changed:" << text << current.isValid();
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

