#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <stdexcept>
#include "lexer.h"

struct ASTNode {
    std::string type;
    std::string value;
    std::vector<ASTNode> children;
    size_t line; // Adicionado para rastrear a linha do nó
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    ASTNode parse();

private:
    std::vector<Token> tokens;
    size_t position;

    Token peek();
    Token advance();
    ASTNode parseStatement();
    ASTNode parseExpression();
    ASTNode parseTerm();
    ASTNode parseFactor();
    ASTNode parsePrint();
    ASTNode parseInput();
    ASTNode parseIf();
    ASTNode parseVariableDeclaration();
    ASTNode parseAssignment();
};

#endif