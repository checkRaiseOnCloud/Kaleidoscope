/*
 **************************************** Grammar ****************************************
 * Grammars used in the expression
 * Primary expressions are number literals, parenthesis, variable identifier or function call
 * identifiers
 * 
 *      Expr
 *      NumberExpr
 *      VariableExpr
 *      BinaryExpr
 *      ProtoExpr
 *      FunctionExpr
 *      CallExpr
 * 
 * NumberExpr ::= number_token
 * VariableExpr ::= identifier_token
 * CallExpr ::= identifier_token (Expr, Expr, Expr)
 * ParenExpr ::= (Expr)
 * PrimExpr ::= NumberExpr
 * PrimExpr ::= VariableExpr
 * PrimExpr ::= CallExpr
 * PrimExpr ::= ParenExpr 
 * Expr ::= PrimExpr
 * Expr ::= Expr + Expr
 * Expr ::= Expr - Expr
 * Expr ::= Expr - Expr
 * Expr ::= Expr / Expr
 * 
 * ProtoExpr ::= identifier_token ( identifier_token identifier_token identifier_token )
 * FunctionExpr ::= def ProtoExpr Expr 
 * Extern ::= extern ProtoExpr
 * TopLevelExpr is stored as an anonymous FunctionExpr
 */

#include <map>

#include "Parser.hpp"
#include "Scanner.hpp"

static std::unique_ptr<ASTBaseExpr> parseExpression();

static std::unique_ptr<ASTBaseExpr> parseBinaryRHS(int exprPrec, std::unique_ptr<ASTBaseExpr> LHS);

/*
 * Operator Precedence
 */

/* mapping from operator to precedence */
map<char, int> operatorPrecedence;

void setOperatorPrecedence(){
    operatorPrecedence['<'] = 10;
    operatorPrecedence['+'] = 20;
    operatorPrecedence['-'] = 30;
    operatorPrecedence['*'] = 40;
}

int getOperatorPrecedence() {
    if (!isascii(currToken)) {
        return -1;
    }

  // Make sure it's a declared binop.
  int tokenPrec = operatorPrecedence[currToken];
  if (tokenPrec <= 0){
    return -1;
  }
  return tokenPrec;
}

/*
 * Parsing function for different non-terminal symbols
 */

std::unique_ptr<ASTBaseExpr> LogError(const char *Str) {
  std::cerr << "Error: %s" << std::endl;
  return nullptr;
}
std::unique_ptr<ASTProtoExpr> LogErrorP(const char *Str) {
  LogError(Str);
  return nullptr;
}

/** @brief Parser helper function to parse NumberExpr
 *  @return NumberExpr
 */
static std::unique_ptr<ASTBaseExpr> parseNumberExpr() {
  auto result = std::make_unique<ASTNumberExpr>(numVal);
  //consume current token
  getNextToken();
  return std::move(result);
}

/** @brief Parser helper function to parse VariableExpr or CallExpr
 *  @return Expression that represents whatever is included in the parenthesis
 * We don't need to keep track of the parenthesis after parsing
 */
static std::unique_ptr<ASTBaseExpr> parseParenExpr() {
    // consume '(' token
    getNextToken();
    // parse the expression inside '()'
    auto subExpr = parseExpression();
    if(!subExpr) {
        return nullptr;
    }
    if (currToken != ')') {
        return LogError("Missing \')\'");
    }
    return subExpr;
}

/** @brief Parser helper function to parse VariableExpr or CallExpr
 *  @return Expression that represents an identifier, could be a variable or a function call
 */
static std::unique_ptr<ASTBaseExpr> parseIdentifierExpr() {
    string identiferName = identifierStr;

    // eat identifier
    getNextToken();

    // identifier not followed by '(' is a character
    if(currToken != '('){
        return std::make_unique<ASTVariableExpr>(identiferName);
    }

    // identifier followed by '(' is a function call
    // function prototype and definition is preceded with keyword 'def'

    // parse function call
    // eat '('
    vector<std::unique_ptr<ASTBaseExpr>> arguments;
    getNextToken();

    while(currToken != ')') {
        if (auto arg = parseExpression()){
            arguments.push_back(std::move(arg));
        } else {
            return nullptr;
        }

        if(currToken == ')'){
            break;
        }

        // argument should be separated by comma
        if(currToken != ',') {
            return LogError("Expected ')' or ',' in the argument list");
        }
        
        //eat the comma
        getNextToken();
    }

    // Eat the ')'
    getNextToken();

    return std::make_unique<ASTCallExpr>(identiferName, std::move(arguments));
}

/** @brief Parser helper function to parse PrimaryExpr
 *  @return Expression that represents a primary expression, could be an identifier, a number
 * or a parenthesis expression
 */
static std::unique_ptr<ASTBaseExpr> parsePrimary() {
    switch (currToken) {
        case token_identifier:
            return parseIdentifierExpr();
        case token_num:
            return parseNumberExpr();
        case '(':
            return parseParenExpr();
        default:
            return LogError("unkown token when expecting a primary expression");
    }
}

/** @brief Parser helper function to parse an expression
 *  @return Expression that reprets the outcome of a binary operator
 */
std::unique_ptr<ASTBaseExpr> parseExpression() {
    // Parse the left hand side
    auto LHS = parsePrimary();
    if(!LHS) {
        return nullptr;
    }
    return parseBinaryRHS(0, std::move(LHS));
}

/** @brief Parser helper function to parse an expression
 *  @return Expression that represents the right hand side of a binary operator
 */
std::unique_ptr<ASTBaseExpr> parseBinaryRHS(int exprPrec, std::unique_ptr<ASTBaseExpr> LHS) {
    while(true) {
        int tokenPrec = getOperatorPrecedence();

        // If this is binary operator has lower precedence then LHS, return the LHS
        if(tokenPrec < exprPrec) {
            return LHS;
        }

        // eat it
        getNextToken();
        int binaryOp = currToken;

        // Get the primary expression after RHS
        auto RHS = parsePrimary();
        if(!RHS) {
            return nullptr;
        }

        // If current binary operator binds less tightly with RHS than the operator after RHS, let
        // the pending operator take RHS as its LHS.
        int NextPrec = getOperatorPrecedence();
        if (tokenPrec  < NextPrec) {
            RHS = parseBinaryRHS(tokenPrec + 1, std::move(RHS));
            if (!RHS) {
                return nullptr;
            }
        }

        // Merge LHS/RHS.
        LHS =
            std::make_unique<ASTBinaryExpr>(binaryOp, std::move(LHS), std::move(RHS));
    }
}

/** @brief Parser helper function to parse an ProtoExpr
 *  @return Expression that represents a function prototype
 */
static std::unique_ptr<ASTProtoExpr> parsePrototype() {
    if(currToken != token_identifier) {
        return LogErrorP("Expect function name in function prototype");
    }
    // eat function name
    std::string functionName = identifierStr;
    getNextToken();

    std::vector<std::string> argumentNames;
    while (getNextToken() == token_identifier){
        argumentNames.push_back(identifierStr);
    }
    if (currToken != ')'){
        return LogErrorP("Expected ')' in function prototype");
    }
    // eat final ')'
    getNextToken();

    return std::make_unique<ASTProtoExpr>(functionName, std::move(argumentNames));
}

std::unique_ptr<ASTFunctionExpr> parseDefinition() {
    getNextToken();
    auto functionProto = parsePrototype();
    if (!functionProto) return nullptr;

    // Parse the body of the function
    if (auto functionBody = parseExpression())
        return std::make_unique<ASTFunctionExpr>(std::move(functionProto), std::move(functionBody));
    return nullptr;
}

std::unique_ptr<ASTProtoExpr> parseExtern() {
    getNextToken();
    return parsePrototype();
}

std::unique_ptr<ASTFunctionExpr> parseTopLevelExpression() {
    if (auto expr = parseExpression()) {
        // Make an anonymous prototype
        auto anonProto = std::make_unique<ASTProtoExpr>("", std::vector<std::string>());
        return std::make_unique<ASTFunctionExpr>(std::move(anonProto), std::move(expr));
    }
    return nullptr;
}
