/*
 * A simple interpreter for a toy language Kaleidoscope
 * Start docker container
 *      docker build -t ubuntu20 .
 *      docker run -it -v $(pwd):/Kaleidoscope ubuntu20
 * Compile:
 * 
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
#include <llvm/Support/raw_ostream.h>

#include "Parser.hpp"
#include "Scanner.hpp"

using namespace std;

static void HandleDefinition() {
  auto defExpr = parseDefinition();
  if (defExpr) {
    std::cout << "Parsed a function definition." << std::endl;
    defExpr->debugMessage(0);
    std::cout << "LLVM IR:" << std::endl;
    auto *defIR = defExpr->codegen();
    defIR->print(errs());
    std::cout << "End of LLVM IR" << std::endl;
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
    std::cout << "LLVM IR:" << std::endl;
    auto *externIR = externExpr->codegen();
    externIR->print(errs());
    std::cout << "End of LLVM IR" << std::endl;
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  auto topLevelExpr = parseTopLevelExpression();
  if (topLevelExpr) {
    std::cout << "Parsed a top level expression." << std::endl;
    topLevelExpr->debugMessage(0);
    std::cout << "LLVM IR:" << std::endl;
    auto *topLevelIR = topLevelExpr->codegen();
    topLevelIR->print(errs());
    std::cout << "End of LLVM IR" << std::endl;
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

int main() {
    // setup operatorPrecedence mapping
    setOperatorPrecedence();

    std::cout << "ready> ";
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