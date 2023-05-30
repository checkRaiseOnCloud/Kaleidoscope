/*
 * A simple interpreter for a toy language Kaleidoscope
 */

#include <cstdio>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include "Expression.hpp"

using namespace std;

/*
 **************************************** Parser ****************************************
 */

/* current token to be consumed by the parser */
static int currToken;

/* consume a token from the stream of input */
static int getNextToken() { return currToken = getToken(); }

static void HandleDefinition() {
  if (ParseDefinition()) {
    fprintf(stderr, "Parsed a function definition.\n");
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleExtern() {
  if (ParseExtern()) {
    fprintf(stderr, "Parsed an extern\n");
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (ParseTopLevelExpr()) {
    fprintf(stderr, "Parsed a top-level expr\n");
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

int main() {
    // Main interpreter loop
    fprintf(stdout, "Welcome to Kaleidoscope Interpreter.\n");

    // setup operatorPrecedence mapping
    setOperatorPrecedence();

    // Kickstart the Interpreter
    getNextToken();

    while (true) {
        printExpressionTree();
        fprintf(std::cin, "ready> ");
        switch (CurTok) {
            case token_eof:
                return;
            case ';': // ignore top-level semicolons for now.
                getNextToken();
                break;
            case token_def:
                HandleDefinition();
                break;
            case token_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}