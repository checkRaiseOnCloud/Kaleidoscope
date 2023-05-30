
#include <string>
#include <cctype>
#include "Scanner.hpp"

string identifierStr;
double numVal;
int currToken;

/// gettok - Return the next token from standard input.
static int gettok() {
    static int LastChar = ' ';

    // Skip any whitespace.
    while (isspace(LastChar)){
        LastChar = getchar();
    }

    // Check if it's identifier, identifier starts with [a-zA-Z]
    // identifier: [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(LastChar)) {
        identifierStr = LastChar;
        while (isalnum((LastChar = getchar()))){
            identifierStr += LastChar;
        }
        // Check special characters
        if (identifierStr == "def")
            return token_def;
        if (identifierStr == "extern")
            return token_extern;
        
        return token_identifier;
    }

    // Number: [0-9.]+
    if (isdigit(LastChar) || LastChar == '.') { 
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        numVal = strtod(NumStr.c_str(), nullptr);
        return token_num;
    }

    // Skip over comments
    if (LastChar == '#') {
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        // Process whatever is behind the comment
        if (LastChar != EOF)
            return gettok();
    }

    // Check for end of file.  Don't eat the EOF.
    if (LastChar == EOF)
        return token_eof;

    // Otherwise, just return the character as its ascii value.
    // We use this for special characters like parenthesis and operators
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

int getNextToken() {
  return currToken = gettok();
}
