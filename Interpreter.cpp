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
#include <csignal>
#include <llvm/Support/raw_ostream.h>

#include "Parser.hpp"
#include "Scanner.hpp"

using namespace std;

static void HandleDefinition() {
  std::cout << "Parsing definintion!" << std::endl;
  auto defExpr = parseDefinition();
  if (defExpr) {
    std::cout << "Parsed a function definition." << std::endl;
    defExpr->debugMessage(0);
    std::cout << "LLVM IR:" << std::endl;
    auto *defIR = defExpr->codegen();
    defIR->print(outs());
    std::cout << "End of LLVM IR" << std::endl;
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleExtern() {
  std::cout << "Parsing extern!" << std::endl;
  auto externExpr = parseExtern();
  if (externExpr) {
    std::cout << "Parsed an extern function." << std::endl;
    externExpr->debugMessage(0);
    std::cout << "LLVM IR:" << std::endl;
    auto *externIR = externExpr->codegen();
    externIR->print(outs());
    std::cout << "End of LLVM IR" << std::endl;
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleTopLevelExpression() {
  std::cout << "Parsing top level expression!" << std::endl;
  // Evaluate a top-level expression into an anonymous function.
  auto topLevelExpr = parseTopLevelExpression();
  if (topLevelExpr) {
    std::cout << "Parsed a top level expression." << std::endl;
    topLevelExpr->debugMessage(0);
    std::cout << "LLVM IR:" << std::endl;
    auto *topLevelIR = topLevelExpr->codegen();
    topLevelIR->print(outs());
    std::cout << "End of LLVM IR" << std::endl;
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

void sigHandler(int signal){
  std::cout << "Interpreter exiting" << std::endl;
  // Print out all of the generated code.
  std::cout << "All parsed LLVM IR: " << std::endl;
  TheModule->print(outs(), nullptr);
  exit(1); 
}

void registerSigHandler() {
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = sigHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);
}

int main() {
    registerSigHandler();
    // setup operatorPrecedence mapping
    setOperatorPrecedence();
    setupParser();

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