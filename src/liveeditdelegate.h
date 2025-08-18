#pragma once
#include <QStyledItemDelegate>
#include <QObject>

class LiveEditDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit LiveEditDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

signals:
    void textEdited(const QModelIndex &index, const QString &text) const;
    void editingFinished(const QModelIndex &index, const QString &text) const;
};
