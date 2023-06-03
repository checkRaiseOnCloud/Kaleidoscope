/*
 **************************************** AST ****************************************
 * Class definitions for expressions/nodes used in AST (Abstract Syntax Tree)
 * Also known as non-terminal symbols in compiler language
 * Expressions:
 *      Expr
 *      NumberExpr
 *      VariableExpr
 *      BinaryExpr
 *      CallExpr   
 * 
 *      FunctionExpr   def + top level expression
 *      ProtoExpr      extern
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <iostream>

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"

using namespace std;
using namespace llvm;

static void printIndentation(int level){
    for(int i = 0; i < level; i++) {
        std::cout << "----";
    }
}

/* Base class for expression nodes */
class ASTBaseExpr {
public:
    virtual ~ASTBaseExpr() = default;

    virtual void debugMessage(int level) {
        printIndentation(level);
        std::cout << "ASTBaseExpr" << std::endl;
    }

    virtual Value *codegen() = 0;
};

/* Number Expression */
class ASTNumberExpr : public ASTBaseExpr {
    double value;
public:
    ASTNumberExpr(double value) : value(value) {};

    void debugMessage(int level) override {
        printIndentation(level);
        std::cout << "Number(" << to_string(value) << ")" << std::endl;
    }

    Value *codegen() override;
};

/* Variable Expression */
class ASTVariableExpr : public ASTBaseExpr {
    string identifier;
public:
    ASTVariableExpr(string identifier) : identifier(identifier) {};

    void debugMessage(int level) override {
        printIndentation(level);
        std::cout << "Variable(" << identifier << ")" << std::endl;
    }

    Value *codegen() override;
};

/* Binary Expression */
class ASTBinaryExpr : public ASTBaseExpr {
    char op;
    unique_ptr<ASTBaseExpr> LHS;
    unique_ptr<ASTBaseExpr> RHS;
public:
    ASTBinaryExpr(char op, unique_ptr<ASTBaseExpr> LHS, unique_ptr<ASTBaseExpr> RHS) :
        op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {};
    
    void debugMessage(int level) override {
        printIndentation(level);
        std::cout << "BinaryExpr(" << op << ")" << std::endl;

        printIndentation(level);
        std::cout << "LHS" << std::endl;
        
        LHS->debugMessage(level + 1);

        printIndentation(level);
        std::cout << "RHS" << std::endl;

        RHS->debugMessage(level + 1);
    }

    Value *codegen() override;
};

/* Function Prototype (Declaration) Expression */
class ASTProtoExpr : public ASTBaseExpr {
    string name;
    vector<string> args;
public:
    ASTProtoExpr(string name, vector<string> args) : name(std::move(name)), args(std::move(args)) {};

    void debugMessage(int level) override {
        printIndentation(level);
        std::cout << "Prototype(" << name << ") Args:(";
        for(auto &arg: args){
            std::cout << arg << ", ";
        }
        std::cout << ")" << std::endl;
    }

    string getName() {
        return name;
    }

    Function *codegen() override;
};

/* Function Definition Expression */
class ASTFunctionExpr : public ASTBaseExpr {
    unique_ptr<ASTProtoExpr> prototype;
    unique_ptr<ASTBaseExpr> body;
public:
    ASTFunctionExpr(unique_ptr<ASTProtoExpr> prototype, unique_ptr<ASTBaseExpr> body)
    : prototype(std::move(prototype)), body(std::move(body)) {};

    void debugMessage(int level) override {
        printIndentation(level);
        std::cout << "Function" << std::endl;
        prototype->debugMessage(level + 1);
        body->debugMessage(level + 1);
    }

    Function *codegen() override;
};

/* Function Call Expression */
class ASTCallExpr : public ASTBaseExpr {
    string callee;
    vector<unique_ptr<ASTBaseExpr>> arguments;
public:
    ASTCallExpr(string callee, vector<unique_ptr<ASTBaseExpr>> arguments)
    : callee(std::move(callee)), arguments(std::move(arguments)) {};

    void debugMessage(int level) override {
        printIndentation(level);
        std::cout << "Call(" << callee << ") Args: " << std::endl;
        for(auto &arg: arguments){
            arg->debugMessage(level+1);
        }
    }

    Value *codegen() override;
};

void setOperatorPrecedence();

/** @brief Parser helper function to parse a function definition
 *  @return Expression that represents a function definition
 */
std::unique_ptr<ASTFunctionExpr> parseDefinition();

/** @brief Parser helper function to parse an extern expression
 *  @return Expression that represent a function prototype that is defined elsewhere
 */
std::unique_ptr<ASTProtoExpr> parseExtern();

/** @brief Parser helper function to parse a top level expression
 *  @return Expression that represents a top level expression
 */
std::unique_ptr<ASTFunctionExpr> parseTopLevelExpression();

// Function to setup parser 
void setupParser();

#endif
