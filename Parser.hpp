/*
 **************************************** AST ****************************************
 * Class definitions for expressions/nodes used in AST (Abstract Syntax Tree)
 * Also known as non-terminal symbols in compiler language
 * Expressions:
 *      Expr
 *      NumberExpr
 *      VariableExpr
 *      BinaryExpr
 *      ProtoExpr
 *      FunctionExpr
 *      CallExpr
 * 
 *      PrimaryExpr
 *      TopLevelExpr
 *      IdentifierExpr
 *      ParenExpr
 *      RHS
 */

#include <string>
#include <memory>
#include <vector>
#include <map>

using namespace std;

namespace {

/* Base class for expression nodes */
class ASTBaseExpr {
public:
    virtual ~ASTBaseExpr() = default;
};

/* Number Expression */
class ASTNumberExpr : public ASTBaseExpr {
    double value;
public:
    ASTNumberExpr(double value) : value(value) {};
};

/* Variable Expression */
class ASTVariableExpr : public ASTBaseExpr {
    string identifier;
public:
    ASTVariableExpr(string identifier) : identifier(identifier) {};
};

/* Binary Expression */
class ASTBinaryExpr : public ASTBaseExpr {
    char op;
    unique_ptr<ASTBaseExpr> LHS;
    unique_ptr<ASTBaseExpr> RHS;
public:
    ASTBinaryExpr(char op, unique_ptr<ASTBaseExpr> LHS, unique_ptr<ASTBaseExpr> RHS) :
        op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {};
};

/* Function Prototype (Declaration) Expression */
class ASTProtoExpr : public ASTBaseExpr {
    string name;
    vector<string> args;
public:
    ASTProtoExpr(string name, vector<string> args) : name(std::move(name)), args(std::move(args)) {};
};

/* Function Definition Expression */
class ASTFunctionExpr : public ASTBaseExpr {
    unique_ptr<ASTProtoExpr> prototype;
    unique_ptr<ASTBaseExpr> body;
public:
    ASTFunctionExpr(unique_ptr<ASTProtoExpr> prototype, unique_ptr<ASTBaseExpr> body)
    : prototype(std::move(prototype)), body(std::move(body)) {};
};

/* Function Call Expression */
class ASTCallExpr : public ASTBaseExpr {
    string callee;
    vector<unique_ptr<ASTBaseExpr>> arguments;
public:
    ASTCallExpr(string callee, vector<unique_ptr<ASTBaseExpr>> arguments)
    : callee(std::move(callee)), arguments(std::move(arguments)) {};
};

} // anonymous namespace

/** @brief Parser helper function to parse a function definition
 *  @return Expression that represents a function definition
 */
std::unique_ptr<ASTFunctionExpr> ParseDefinition();

/** @brief Parser helper function to parse an extern expression
 *  @return Expression that represent a function prototype that is defined elsewhere
 */
std::unique_ptr<ASTProtoExpr> parseExtern();

/** @brief Parser helper function to parse a top level expression
 *  @return Expression that represents a top level expression
 */
std::unique_ptr<ASTFunctionExpr> parseTopLevelExpression();
