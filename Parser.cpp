/*
 **************************************** Grammar ****************************************
 * Grammars used in the expression
 * Primary expressions are number literals, parenthesis, variable identifier or function call
 * identifiers
 * 
 * ParenExpr ::= '(' Expr ')'
 * 
 * NumberExpr ::= number_token
 * 
 * VariableExpr ::= identifier_token
 * 
 * CallExpr ::= identifier_token '(' Expr* ')'
 * 
 * IdentifierExpr ::= VariableExpr
 * IdentifierExpr ::= CallExpr
 * 
 * PrimaryExpr ::= NumberExpr
 * PrimaryExpr ::= IdentifierExpr
 * PrimaryExpr ::= ParenExpr
 * 
 * Expr ::= PrimaryExpr RHS
 * RHS ::= ('+' PrimaryExpr)*
 * RHS ::= ('-' PrimaryExpr)*
 * RHS ::= ('*' PrimaryExpr)*
 * RHS ::= ('<' PrimaryExpr)*
 * 
 * ProtoExpr ::= identifier_token '(' identifier_token* ')'
 * FunctionExpr ::= 'def' ProtoExpr Expr 
 * External ::= 'extern' ProtoExpr
 * 
 * TopLevelExpr ::= Expr
 */

#include "Parser.hpp"


/** @brief Parser helper function to parse NumberExpr
 *  @return NumberExpr
 */
static std::unique_ptr<ASTBaseExpr> parseNumberExpression() {
}

/** @brief Parser helper function to parse VariableExpr or CallExpr
 *  @return Expression that represents whatever is included in the parenthesis
 * We don't need to keep track of the parenthesis after parsing
 */
static std::unique_ptr<ASTBaseExpr> parseParenExpression() {
}

/** @brief Parser helper function to parse VariableExpr or CallExpr
 *  @return Expression that represents an identifier, could be a variable or a function call
 */
static std::unique_ptr<ASTBaseExpr> parseIdentifierExpression() {

}

/** @brief Parser helper function to parse PrimaryExpr
 *  @return Expression that represents a primary expression, could be an identifier, a number
 * or a parenthesis expression
 */
static std::unique_ptr<ASTBaseExpr> parsePrimary() {

}

/** @brief Parser helper function to parse an expression
 *  @return Expression that reprets the outcome of a binary operator
 */
static std::unique_ptr<ASTBaseExpr> parseExpression() {

}

/** @brief Parser helper function to parse an expression
 *  @return Expression that represents the right hand side of a binary operator
 */
static std::unique_ptr<ASTBaseExpr> parseBinaryRHS() {

}

/** @brief Parser helper function to parse an ProtoExpr
 *  @return Expression that represents a function prototype
 */
static std::unique_ptr<ASTProtoExpr> parsePrototype() {

}