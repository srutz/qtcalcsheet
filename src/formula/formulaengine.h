
#ifndef FORMULAENGINE_H
#define FORMULAENGINE_H

#include <QObject>
#include <QVariant>
#include <QHash>

class FormulaEngine : public QObject
{
    Q_OBJECT
public:
    explicit FormulaEngine(QObject *parent = nullptr);
    QVariant evaluate(const QString &formula, const QHash<QString, QVariant> &variables) const;

};

#endif // FORMULAENGINE_H
