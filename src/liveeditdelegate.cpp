#include "liveeditdelegate.h"
#include <QLineEdit>
#include <QDebug>

LiveEditDelegate::LiveEditDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

QWidget *LiveEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const {
    QLineEdit *editor = new QLineEdit(parent);
    QObject::connect(editor, &QLineEdit::textChanged, [index, editor, this](const QString &text) {
        emit textEdited(index, text);
        qDebug() << "Live text in cell" << index << ":" << text;
    });
    // connect to enter pressed
    QObject::connect(editor, &QLineEdit::returnPressed, [index, editor, this]() {
        emit editingFinished(index, editor->text());
    });
    return editor;
}

void LiveEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    if (lineEdit)
        lineEdit->setText(index.data().toString());
}

void LiveEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const {
    QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor);
    if (lineEdit)
        model->setData(index, lineEdit->text());
}
