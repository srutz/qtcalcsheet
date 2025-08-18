#include "liveeditdelegate.h"
#include "keypresslineedit.h"
#include <QLineEdit>
#include <QDebug>

LiveEditDelegate::LiveEditDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {}

QWidget *LiveEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const {
    auto editor = new KeyPressLineEdit(parent);
    QObject::connect(editor, &KeyPressLineEdit::textChanged, [index, editor, this](const QString &text) {
        emit textEdited(index, text);
    });
    // connect to enter pressed
    QObject::connect(editor, &KeyPressLineEdit::returnPressed, [index, editor, this]() {
        emit editingFinished(index, editor->text());
    });
    // connect to keypresses
    QObject::connect(editor, &KeyPressLineEdit::keyPressed, [index, editor, this](QKeyEvent *event) {
        emit keyPressEvent(index, event);
    });
    return editor;
}

void LiveEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    KeyPressLineEdit *lineEdit = qobject_cast<KeyPressLineEdit*>(editor);
    if (lineEdit) {
        lineEdit->setText(index.data().toString());
    }
}

void LiveEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    KeyPressLineEdit *lineEdit = qobject_cast<KeyPressLineEdit *>(editor);
    if (lineEdit) {
        model->setData(index, lineEdit->text());
    }
}
