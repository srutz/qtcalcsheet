#ifndef KEYPRESSLINEEDIT_H
#define KEYPRESSLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>

class KeyPressLineEdit : public QLineEdit {
    Q_OBJECT
public:
    explicit KeyPressLineEdit(QWidget *parent = nullptr);

signals:
    void keyPressed(QKeyEvent *event);

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // KEYPRESSLINEEDIT_H