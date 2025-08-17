#ifndef EXCELNODE_H
#define EXCELNODE_H

#include <string>
#include <vector>
#include <memory>

using namespace std;

// AST node types
enum NodeType {
    NODE_NUMBER,
    NODE_STRING,
    NODE_CELL_REF,
    NODE_RANGE,
    NODE_FUNCTION,
    NODE_BINARY_OP,
    NODE_UNARY_OP
};

// AST node structure
struct ExcelNode {
    NodeType type;
    string value;
    string op;
    vector<shared_ptr<ExcelNode>> children;
    
    ExcelNode(NodeType t) : type(t) {}
    ExcelNode(NodeType t, const string& v) : type(t), value(v) {}
    ExcelNode(NodeType t, const string& operation, shared_ptr<ExcelNode> left, shared_ptr<ExcelNode> right) 
        : type(t), op(operation) {
        children.push_back(left);
        children.push_back(right);
    }
};

#endif // EXCELNODE_H
