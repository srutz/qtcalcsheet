
#ifndef FORMULAENGINE_H
#define FORMULAENGINE_H

#include <QObject>
#include <QVariant>
#include <QHash>
#include <memory>

struct ExcelNode;

class FormulaEngine : public QObject
{
    Q_OBJECT
public:
    explicit FormulaEngine(QObject *parent = nullptr);
    std::shared_ptr<ExcelNode> parse(const QString &formula, const QHash<QString, QVariant> &variables) const;
    QVariant evaluate(const QString &formula, const QHash<QString, QVariant> &variables) const;
    void dumpNode(std::shared_ptr<ExcelNode> node, int indent) const ;

private:
    QVariant evaluateNode(std::shared_ptr<ExcelNode> node, const QHash<QString, QVariant> &variables) const;
};

#endif // FORMULAENGINE_H
