%{
#include <iostream>
#include <string>
#include <map>
#include <cctype>

using namespace std;

// Include the parser header (generated from the .y file)
#include "excelnode.h"
#include "formulagrammer.h"

// Line number tracking

// German function names (case-insensitive)
extern map<string, int> germanFunctions; 

// Convert string to uppercase for case-insensitive comparison
string toUpper(const string& str) {
    string result = str;
    for (char& c : result) {
        c = toupper(c);
    }
    return result;
}

// Check if a string is a German function name
bool isGermanFunction(const string& str) {
    string upper = toUpper(str);
    return germanFunctions.find(upper) != germanFunctions.end();
}

%}

%option noyywrap
%option yylineno

/* German decimal separator handling */
DIGIT           [0-9]
INTEGER         {DIGIT}+
DECIMAL         {INTEGER}[,]{INTEGER}
NUMBER          {INTEGER}|{DECIMAL}

/* Cell reference patterns */
COLUMN          [A-Za-z]+
ROW             {DIGIT}+
ABSOLUTE        \$
CELL_REF        ({ABSOLUTE}?{COLUMN}{ABSOLUTE}?{ROW})

/* String literals */
STRING          \"([^\"\n]|\"\")*\"

/* Function names and identifiers */
LETTER          [A-Za-zÄÖÜäöüß]
IDENTIFIER      {LETTER}({LETTER}|{DIGIT}|_)*

/* Whitespace */
WHITESPACE      [ \t]+

%%

    /* Numbers - handle German decimal format */
{NUMBER} {
    // Convert German decimal format (comma) to international format (dot)
    string numStr = yytext;
    size_t commaPos = numStr.find(',');
    if (commaPos != string::npos) {
        numStr[commaPos] = '.';
    }
    yylval.str = new string(numStr);
    return NUMBER;
}

    /* String literals */
{STRING} {
    string str = yytext;
    // Remove outer quotes
    str = str.substr(1, str.length() - 2);
    // Handle escaped quotes ("")
    size_t pos = 0;
    while ((pos = str.find("\"\"", pos)) != string::npos) {
        str.replace(pos, 2, "\"");
        pos += 1;
    }
    yylval.str = new string(str);
    return STRING;
}

    /* Cell references */
{CELL_REF} {
    yylval.str = new string(yytext);
    return CELL_REF;
}

    /* Function names (case-insensitive) */
{IDENTIFIER} {
    if (isGermanFunction(yytext)) {
        yylval.str = new string(toUpper(yytext));
        return FUNCTION_NAME;
    } else {
        // If not a function, treat as cell reference or error
        yylval.str = new string(yytext);
        return CELL_REF;
    }
}

    /* Operators */
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULTIPLY; }
"/"             { return DIVIDE; }
"^"             { return POWER; }
"&"             { return CONCAT; }

    /* Comparison operators */
"="             { return EQ; }
"<>"            { return NE; }
"<="            { return LE; }
">="            { return GE; }
"<"             { return LT; }
">"             { return GT; }

    /* Delimiters */
"("             { return LPAREN; }
")"             { return RPAREN; }
";"             { return SEMICOLON; }
","             { return COMMA; }
":"             { return COLON; }

    /* Formula start */
"="             { return '='; }

    /* Line endings */
\n              { return NEWLINE; }
\r\n            { return NEWLINE; }
\r              { return NEWLINE; }

    /* Whitespace (ignore) */
{WHITESPACE}    { /* ignore */ }

    /* Anything else is an error */
.               {
    cerr << "Unexpected character: '" << yytext[0] << "' at line " << yylineno << endl;
    return yytext[0];
}

%%

/* Additional C code */

void resetLexer() {
    yylineno = 1;
}

/* Function to set input string for parsing */
void setInputString(const string& input) {
    YY_BUFFER_STATE buffer = yy_scan_string(input.c_str());
    yy_switch_to_buffer(buffer);
}

/* Function to clean up buffer */
void cleanupLexer() {
    yy_delete_buffer(YY_CURRENT_BUFFER);
}

