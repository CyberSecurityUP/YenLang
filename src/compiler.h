#ifndef COMPILER_H
#define COMPILER_H

#include "parser.h"
#include <map>
#include <string>

class Compiler {
public:
    void compile(const ASTNode& ast);

private:
    std::map<std::string, std::string> variables; // Stores variables and their values

    void compileExpression(const ASTNode& node);
    void compilePrint(const ASTNode& node);
    void compileInput(const ASTNode& node);
    void compileIf(const ASTNode& node);
    void compileVariableDeclaration(const ASTNode& node);

    std::string evaluateExpression(const ASTNode& node);
};

#endif
