
#include "calctableview.h"
#include <QKeyEvent>

CalcTableView::CalcTableView(QWidget *parent)
    : QTableView(parent) 
{
}

void CalcTableView::keyPressEvent(QKeyEvent *event) {
    emit keyPressed(event);
    QTableView::keyPressEvent(event);
}

