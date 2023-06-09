/*
 **************************************** Scanner ****************************************
 * Turn input into valid tokens
 */

#ifndef SCANNER_H_
#define SCANNER_H_

#include <string>

using namespace std;

enum Token {
    token_eof = 0,
    token_extern = 1,
    token_def = 2,
    token_identifier = 3,
    token_num = 4
};

/* Scanner values */
extern string identifierStr;
extern double numVal;
extern int currToken;

/** @brief Scanner helper function to get token from stdin
 *  @return the next token from standard input
 */
int getNextToken();

#endif 
