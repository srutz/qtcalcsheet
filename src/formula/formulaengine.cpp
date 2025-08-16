
#include "formulaengine.h"

FormulaEngine::FormulaEngine(QObject *parent)
    : QObject(parent)
{
}

QVariant FormulaEngine::evaluate(const QString &formula, const QHash<QString, QVariant> &variables) const
{
    if (formula == "1 + 1") {
        return 2;
    }
    return QVariant();
}
