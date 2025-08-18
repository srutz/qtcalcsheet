#include "keypresslineedit.h"

KeyPressLineEdit::KeyPressLineEdit(QWidget *parent)
    : QLineEdit(parent) {}

void KeyPressLineEdit::keyPressEvent(QKeyEvent *event) {
    emit keyPressed(event);
    QLineEdit::keyPressEvent(event);
}

