%{
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cmath>

using namespace std;

// Forward declarations
struct ExcelValue;
struct ExcelNode;

extern int yylex();
void yyerror(const char* s);

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

// Global result
shared_ptr<ExcelNode> parseResult;

// German function name mapping
map<string, string> germanFunctions = {
    {"SUMME", "SUM"},
    {"MITTELWERT", "AVERAGE"},
    {"ANZAHL", "COUNT"},
    {"ANZAHL2", "COUNTA"},
    {"MAX", "MAX"},
    {"MIN", "MIN"},
    {"WENN", "IF"},
    {"UND", "AND"},
    {"ODER", "OR"},
    {"NICHT", "NOT"},
    {"SVERWEIS", "VLOOKUP"},
    {"WVERWEIS", "HLOOKUP"},
    {"INDEX", "INDEX"},
    {"VERGLEICH", "MATCH"},
    {"LÄNGE", "LEN"},
    {"LINKS", "LEFT"},
    {"RECHTS", "RIGHT"},
    {"TEIL", "MID"},
    {"GROSS", "UPPER"},
    {"KLEIN", "LOWER"},
    {"HEUTE", "TODAY"},
    {"JETZT", "NOW"},
    {"JAHR", "YEAR"},
    {"MONAT", "MONTH"},
    {"TAG", "DAY"},
    {"DATUM", "DATE"},
    {"ZEIT", "TIME"},
    {"RUNDEN", "ROUND"},
    {"AUFRUNDEN", "ROUNDUP"},
    {"ABRUNDEN", "ROUNDDOWN"},
    {"ABS", "ABS"},
    {"WURZEL", "SQRT"},
    {"POTENZ", "POWER"},
    {"EXP", "EXP"},
    {"LN", "LN"},
    {"LOG", "LOG"},
    {"SIN", "SIN"},
    {"COS", "COS"},
    {"TAN", "TAN"}
};

%}

%union {
    string* str;
    shared_ptr<ExcelNode>* node;
    vector<shared_ptr<ExcelNode>>* nodeList;
}

%token <str> NUMBER STRING CELL_REF FUNCTION_NAME
%token LPAREN RPAREN SEMICOLON COMMA COLON
%token PLUS MINUS MULTIPLY DIVIDE POWER
%token EQ NE LT LE GT GE
%token CONCAT
%token NEWLINE

%type <node> formula expression term factor function_call cell_reference range
%type <nodeList> argument_list

%left EQ NE LT LE GT GE
%left PLUS MINUS
%left MULTIPLY DIVIDE
%left CONCAT
%right POWER
%right UMINUS UPLUS

%start input

%%

input:
    formula NEWLINE { parseResult = *$1; }
    | formula { parseResult = *$1; }
    ;

formula:
    '=' expression { $$ = $2; }
    | expression { $$ = $1; }
    ;

expression:
    expression PLUS expression {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_BINARY_OP, "+", *$1, *$3)
        );
    }
    | expression MINUS expression {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_BINARY_OP, "-", *$1, *$3)
        );
    }
    | expression MULTIPLY expression {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_BINARY_OP, "*", *$1, *$3)
        );
    }
    | expression DIVIDE expression {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_BINARY_OP, "/", *$1, *$3)
        );
    }
    | expression POWER expression {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_BINARY_OP, "^", *$1, *$3)
        );
    }
    | expression CONCAT expression {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_BINARY_OP, "&", *$1, *$3)
        );
    }
    | expression EQ expression {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_BINARY_OP, "=", *$1, *$3)
        );
    }
    | expression NE expression {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_BINARY_OP, "<>", *$1, *$3)
        );
    }
    | expression LT expression {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_BINARY_OP, "<", *$1, *$3)
        );
    }
    | expression LE expression {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_BINARY_OP, "<=", *$1, *$3)
        );
    }
    | expression GT expression {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_BINARY_OP, ">", *$1, *$3)
        );
    }
    | expression GE expression {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_BINARY_OP, ">=", *$1, *$3)
        );
    }
    | PLUS expression %prec UPLUS {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_UNARY_OP, "+", *$2, nullptr)
        );
    }
    | MINUS expression %prec UMINUS {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_UNARY_OP, "-", *$2, nullptr)
        );
    }
    | term { $$ = $1; }
    ;

term:
    factor { $$ = $1; }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

factor:
    NUMBER {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_NUMBER, *$1)
        );
        delete $1;
    }
    | STRING {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_STRING, *$1)
        );
        delete $1;
    }
    | cell_reference { $ = $1; }
    | range { $ = $1; }
    | function_call { $ = $1; }
    ;

cell_reference:
    CELL_REF {
        $ = new shared_ptr<ExcelNode>(
            make_shared<ExcelNode>(NODE_CELL_REF, *$1)
        );
        delete $1;
    }
    ;

range:
    CELL_REF COLON CELL_REF {
        auto node = make_shared<ExcelNode>(NODE_RANGE);
        node->value = *$1 + ":" + *$3;
        $ = new shared_ptr<ExcelNode>(node);
        delete $1;
        delete $3;
    }
    ;

function_call:
    FUNCTION_NAME LPAREN argument_list RPAREN {
        auto node = make_shared<ExcelNode>(NODE_FUNCTION, *$1);
        node->children = *$3;
        $ = new shared_ptr<ExcelNode>(node);
        delete $1;
        delete $3;
    }
    | FUNCTION_NAME LPAREN RPAREN {
        auto node = make_shared<ExcelNode>(NODE_FUNCTION, *$1);
        $ = new shared_ptr<ExcelNode>(node);
        delete $1;
    }
    ;

argument_list:
    expression {
        $ = new vector<shared_ptr<ExcelNode>>();
        $->push_back(*$1);
        delete $1;
    }
    | argument_list SEMICOLON expression {
        $1->push_back(*$3);
        $ = $1;
        delete $3;
    }
    | argument_list COMMA expression {
        $1->push_back(*$3);
        $ = $1;
        delete $3;
    }
    ;

%%

void yyerror(const char* s) {
    cerr << "Parse error: " << s << endl;
}

// Helper function to print AST (for debugging)
void printAST(shared_ptr<ExcelNode> node, int indent = 0) {
    if (!node) return;
    
    string indentStr(indent * 2, ' ');
    
    switch (node->type) {
        case NODE_NUMBER:
            cout << indentStr << "NUMBER: " << node->value << endl;
            break;
        case NODE_STRING:
            cout << indentStr << "STRING: " << node->value << endl;
            break;
        case NODE_CELL_REF:
            cout << indentStr << "CELL: " << node->value << endl;
            break;
        case NODE_RANGE:
            cout << indentStr << "RANGE: " << node->value << endl;
            break;
        case NODE_FUNCTION:
            cout << indentStr << "FUNCTION: " << node->value << endl;
            for (auto& child : node->children) {
                printAST(child, indent + 1);
            }
            break;
        case NODE_BINARY_OP:
            cout << indentStr << "BINARY_OP: " << node->op << endl;
            for (auto& child : node->children) {
                printAST(child, indent + 1);
            }
            break;
        case NODE_UNARY_OP:
            cout << indentStr << "UNARY_OP: " << node->op << endl;
            for (auto& child : node->children) {
                printAST(child, indent + 1);
            }
            break;
    }
}

// Function to get English equivalent of German function
string getEnglishFunction(const string& germanFunc) {
    auto it = germanFunctions.find(germanFunc);
    return (it != germanFunctions.end()) ? it->second : germanFunc;
}

