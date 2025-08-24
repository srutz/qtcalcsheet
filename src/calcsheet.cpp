#include "calcsheet.h"
#include "calcsheetmodel.h"
#include "util.h"
#include "liveeditdelegate.h"
#include "guard.h"
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
    m_formulaEngine(new FormulaEngine(this))
{
    this->m_tableView = new CalcTableView(this);
    // Set up live edit delegate
    auto liveDelegate = new LiveEditDelegate(this);
    m_tableView->setItemDelegate(liveDelegate);
    auto mainLayout = new QVBoxLayout(this);

    auto topPanel = new QWidget(this);
    mainLayout->addWidget(topPanel);
    auto topLayout = new QHBoxLayout(topPanel);
    topLayout->setContentsMargins(0, 0, 0, 0);

    auto topLabel = new QLabel("f(x)", this);
    topLayout->addWidget(topLabel);

    this->m_input = new QLineEdit(this);
    topLayout->addWidget(m_input);

    mainLayout->addWidget(m_tableView);

    // setup table
    this->m_model = new CalcSheetModel(this);
    m_tableView->setModel(m_model);
    m_tableView->verticalHeader()->setVisible(true);
    // make table editable
    m_tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed | QAbstractItemView::AnyKeyPressed | QAbstractItemView::SelectedClicked);
    m_tableView->verticalHeader()->setSectionsMovable(true);
    m_tableView->horizontalHeader()->setSectionsMovable(true);

    // navigate in table in excel style using keyboard
    m_tableView->installEventFilter(this);

    // wire up table selection to input
    connect(m_tableView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this](const QModelIndex &current) {
        this->m_input->setText(current.data().toString());
    });

    // wire up enter press in the input
    connect(m_input, &QLineEdit::returnPressed, this, [this]() {
        QModelIndex current = m_tableView->currentIndex();
        if (!current.isValid()) {
            return;
        }
        m_tableView->setFocus();
        Util::selectNextTableRow(m_tableView);
    });

    // while typing in the table, also update the input field but prevent event cycles
    connect(liveDelegate, &LiveEditDelegate::textEdited, this, [this](const QModelIndex &index, const QString &text) {
        Guard guard([this]() { m_changingInput = false; });
        m_changingInput = true;
        this->m_input->setText(text);
    });

    // move to next row on enter in the table
    connect(liveDelegate, &LiveEditDelegate::editingFinished, this, [this](const QModelIndex &index, const QString &text) {
        Util::selectNextTableRow(m_tableView);
    });

    // Track committed changes
    connect(m_model, &QAbstractItemModel::dataChanged, this, [this](const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles) {
        //qDebug() << "Committed table change from" << topLeft << "to" << bottomRight;
    });

    // while typing in the inputfield also update the current table value
    connect(m_input, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (m_changingInput) {
            return;
        }
        QModelIndex current = m_tableView->currentIndex();
        //qDebug() << "Input text changed:" << text << current.isValid();
        if (!current.isValid()) {
            return;
        }
        // update the model
        auto model = const_cast<QAbstractItemModel*>(current.model());
        model->setData(current, text);
    });

    // handle cursor movements and cell delections in the table
    connect(liveDelegate, &LiveEditDelegate::keyPressEvent, this, [this](const QModelIndex &index, QKeyEvent *event) {
        if (event->key() == Qt::Key_Up) {
            Util::selectPreviousTableRow(m_tableView);
        } else if (event->key() == Qt::Key_Down) {
            Util::selectNextTableRow(m_tableView);
        }
    });

    // wire up key presses in the table for deletion and other keys
    connect(m_tableView, &CalcTableView::keyPressed, this, [this](QKeyEvent *event) {
        QModelIndex current = m_tableView->currentIndex();
        if (!current.isValid()) {
            return;
        }
        if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
            // clear all selected cells
            auto selection = m_tableView->selectionModel()->selectedIndexes();
            for (const QModelIndex &idx : selection) {
                this->m_model->setData(idx, "");
            }
            this->m_input->setText("");
        }
    });
}

CalcSheet::~CalcSheet()
{
}

