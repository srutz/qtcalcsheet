
#include "formulaengine.h"
#include "excelnode.h"
#include "../guard.h"
#include <QDebug>
#include <QtMath>

extern int yyparse();
extern std::shared_ptr<ExcelNode> parseResult;

// External lexer functions
extern void setInputString(const std::string& input);
extern void cleanupLexer();
extern void resetLexer();


FormulaEngine::FormulaEngine(QObject *parent)
    : QObject(parent)
{
}

std::shared_ptr<ExcelNode> FormulaEngine::parse(const QString &formula, const QHash<QString, QVariant> &variables) const
{
    Guard guard([]() { cleanupLexer(); });
    // Reset parser state
    parseResult = nullptr;
    resetLexer();
    
    // Set up the input string for the lexer
    std::string formulaStr = formula.toStdString();
    setInputString(formulaStr);
    
    // Parse the formula
    int parseStatus = yyparse();
    if (parseStatus != 0) {
        qDebug() << "Parse error in formula:" << formula;
        return nullptr;
    }
    if (!parseResult) {
        qDebug() << "No parse result for formula:" << formula;
        return nullptr;
    }
    // parsed AST in parseResult
    return parseResult;
}

QVariant FormulaEngine::evaluate(const QString &formula, const QHash<QString, QVariant> &variables) const
{
    auto node = parse(formula, variables);
    return evaluateNode(node, variables);
}

QVariant FormulaEngine::evaluateNode(std::shared_ptr<ExcelNode> node, const QHash<QString, QVariant> &variables) const
{
    if (!node) {
        return QVariant();
    }
    
    switch (node->type) {
        case NODE_NUMBER: {
            bool ok;
            double value = QString::fromStdString(node->value).toDouble(&ok);
            return ok ? QVariant(value) : QVariant();
        }
        
        case NODE_STRING:
            return QVariant(QString::fromStdString(node->value));
            
        case NODE_CELL_REF: {
            QString cellRef = QString::fromStdString(node->value);
            return variables.value(cellRef, QVariant());
        }
        
        case NODE_BINARY_OP: {
            if (node->children.size() != 2) {
                return QVariant();
            }
            
            QVariant left = evaluateNode(node->children[0], variables);
            QVariant right = evaluateNode(node->children[1], variables);
            
            QString op = QString::fromStdString(node->op);
            
            // Handle numeric operations
            if (left.canConvert<double>() && right.canConvert<double>()) {
                double leftVal = left.toDouble();
                double rightVal = right.toDouble();
                
                if (op == "+") return leftVal + rightVal;
                if (op == "-") return leftVal - rightVal;
                if (op == "*") return leftVal * rightVal;
                if (op == "/") return rightVal != 0 ? leftVal / rightVal : QVariant();
                if (op == "^") return qPow(leftVal, rightVal);
                if (op == "=") return leftVal == rightVal;
                if (op == "<>") return leftVal != rightVal;
                if (op == "<") return leftVal < rightVal;
                if (op == "<=") return leftVal <= rightVal;
                if (op == ">") return leftVal > rightVal;
                if (op == ">=") return leftVal >= rightVal;
            }
            // Handle string concatenation
            if (op == "&") {
                return left.toString() + right.toString();
            }
            return QVariant();
        }
        
        case NODE_UNARY_OP: {
            if (node->children.size() != 1) {
                return QVariant();
            }
            
            QVariant operand = evaluateNode(node->children[0], variables);
            QString op = QString::fromStdString(node->op);
            
            if (operand.canConvert<double>()) {
                double value = operand.toDouble();
                if (op == "+") return value;
                if (op == "-") return -value;
            }
            
            return QVariant();
        }
        
        case NODE_FUNCTION: {
            QString funcName = QString::fromStdString(node->value);
            qDebug() << "Function not implemented:" << funcName;
            return QVariant();
        }
        
        case NODE_RANGE: {
            QString range = QString::fromStdString(node->value);
            qDebug() << "Range not implemented:" << range;
            return QVariant();
        }
        
        default:
            return QVariant();
    }
}

void FormulaEngine::dumpNode(std::shared_ptr<ExcelNode> node, int indent = 0) const {
    if (!node) return;
    auto indentStr = QString::fromStdString(string(indent * 2, ' '));
    
    switch (node->type) {
        case NODE_NUMBER:
            qDebug() << indentStr << "NUMBER: " << QString::fromStdString(node->value);
            break;
        case NODE_STRING:
            qDebug() << indentStr << "STRING: " << QString::fromStdString(node->value);
            break;
        case NODE_CELL_REF:
            qDebug() << indentStr << "CELL: " << QString::fromStdString(node->value);
            break;
        case NODE_RANGE:
            qDebug() << indentStr << "RANGE: " << QString::fromStdString(node->value);
            break;
        case NODE_FUNCTION:
            qDebug() << indentStr << "FUNCTION: " << QString::fromStdString(node->value);
            for (auto& child : node->children) {
                dumpNode(child, indent + 1);
            }
            break;
        case NODE_BINARY_OP:
            qDebug() << indentStr << "BINARY_OP: " << QString::fromStdString(node->op);
            for (auto& child : node->children) {
                dumpNode(child, indent + 1);
            }
            break;
        case NODE_UNARY_OP:
            qDebug() << indentStr << "UNARY_OP: " << QString::fromStdString(node->op);
            for (auto& child : node->children) {
                dumpNode(child, indent + 1);
            }
            break;
    }
}
