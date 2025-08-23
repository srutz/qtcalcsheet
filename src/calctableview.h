#ifndef CALCTABLEVIEW_H
#define CALCTABLEVIEW_H

#include <QTableView>
#include <QKeyEvent>

class CalcTableView : public QTableView
{
    Q_OBJECT

public:
    CalcTableView(QWidget *parent = nullptr);

signals:
    void keyPressed(QKeyEvent *event) const;

protected:
    void keyPressEvent(QKeyEvent *event) override;

};

#endif // CALCTABLEVIEW_H


