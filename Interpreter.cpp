/*
 * A simple interpreter for a toy language Kaleidoscope
 * Compile:
 *  clang++ -g -O3 Interpreter.cpp Scanner.cpp Parser.cpp `llvm-config --cxxflags`
 * Run:
 *  
 */
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <cctype>
#include <cstdio>
#include <cstdlib>

#include "Parser.hpp"
#include "Scanner.hpp"

using namespace std;

static void HandleDefinition() {
  auto defExpr = parseDefinition();
  if (defExpr) {
    std::cout << "Parsed a function definition." << std::endl;
    defExpr->debugMessage(0);
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleExtern() {
  auto externExpr = parseExtern();
  if (externExpr) {
    std::cout << "Parsed an extern function." << std::endl;
    externExpr->debugMessage(0);
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  auto expr = parseTopLevelExpression();
  if (expr) {
    std::cout << "Parsed a top level expression." << std::endl;
    expr->debugMessage(0);
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

int main() {
    // Main interpreter loop
    std::cout << "Welcome to Kaleidoscope Interpreter." << std::endl;

    // setup operatorPrecedence mapping
    setOperatorPrecedence();

    getNextToken();

    while (true) {
        std::cout << "ready> ";
        switch (currToken) {
            case token_eof:
                return 0;
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
    return 0;
}