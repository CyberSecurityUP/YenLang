#include "compiler.h"
#include <iostream>
#include <map>
#include <stdexcept>
#include <sstream>

// Implementa��o dos m�todos da classe Compiler

void Compiler::compile(const ASTNode& ast) {
    if (ast.type == "PROGRAM") {
        for (const auto& child : ast.children) {
            compileExpression(child);
        }
    }
    else {
        throw std::runtime_error("N� AST desconhecido: " + ast.type);
    }
}

void Compiler::compileExpression(const ASTNode& node) {
    if (node.type == "BINARY_OP") {
        std::string result = evaluateExpression(node);
        // Se a express�o n�o for usada em um comando, o resultado � descartado
    }
    else if (node.type == "STRING_CONCAT") {
        std::string result = evaluateExpression(node);
        // Se a express�o n�o for usada em um comando, o resultado � descartado
    }
    else if (node.type == "PRINT") {
        compilePrint(node);
    }
    else if (node.type == "INPUT") {
        compileInput(node);
    }
    else if (node.type == "IF") {
        compileIf(node);
    }
    else if (node.type == "VAR") {
        compileVariableDeclaration(node);
    }
    else if (node.type == "EMPTY_STATEMENT") {
        // Ignora n�s do tipo EMPTY_STATEMENT
        return;
    }
    else {
        throw std::runtime_error("N� AST desconhecido: " + node.type);
    }
}

std::string Compiler::evaluateExpression(const ASTNode& node) {
    if (node.type == "NUMBER") {
        return node.value;
    }
    else if (node.type == "STRING") {
        return node.value;
    }
    else if (node.type == "IDENTIFIER") {
        if (variables.find(node.value) != variables.end()) {
            return variables[node.value];
        }
        throw std::runtime_error("Vari�vel n�o definida: " + node.value);
    }
    else if (node.type == "BINARY_OP") {
        std::string left = evaluateExpression(node.children[0]);
        std::string right = evaluateExpression(node.children[1]);
        if (node.value == "+") {
            // Concatena��o de strings ou adi��o de n�meros
            return left + right;
        }
        // Adicione outros operadores aqui, se necess�rio
    }
    else if (node.type == "STRING_CONCAT") {
        std::string left = evaluateExpression(node.children[0]);
        std::string right = evaluateExpression(node.children[1]);
        return left + right; // Concatena��o de strings
    }
    throw std::runtime_error("Express�o inv�lida: " + node.type);
}

void Compiler::compilePrint(const ASTNode& node) {
    std::string result = evaluateExpression(node.children[0]);
    std::cout << result << std::endl; // Exibe o resultado
}

void Compiler::compileInput(const ASTNode& node) {
    std::string prompt = "";
    if (!node.children.empty()) {
        prompt = evaluateExpression(node.children[0]);
    }
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    // Armazena a entrada em uma vari�vel, se necess�rio
}

void Compiler::compileIf(const ASTNode& node) {
    std::string condition = evaluateExpression(node.children[0]);
    if (condition != "0" && condition != "") { // Condi��o verdadeira
        compileExpression(node.children[1]);
    }
    else if (node.children.size() > 2) { // Bloco else
        compileExpression(node.children[2]);
    }
}

void Compiler::compileVariableDeclaration(const ASTNode& node) {
    std::string varName = node.children[0].value;
    std::string value = evaluateExpression(node.children[2]);
    variables[varName] = value; // Armazena a vari�vel e seu valor
}