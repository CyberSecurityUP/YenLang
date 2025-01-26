#include "compiler.h"
#include <iostream>
#include <map>
#include <stdexcept>
#include <sstream>

// Implementação dos métodos da classe Compiler

void Compiler::compile(const ASTNode& ast) {
    if (ast.type == "PROGRAM") {
        for (const auto& child : ast.children) {
            compileExpression(child);
        }
    }
    else {
        throw std::runtime_error("Nó AST desconhecido: " + ast.type);
    }
}

void Compiler::compileExpression(const ASTNode& node) {
    if (node.type == "BINARY_OP") {
        std::string result = evaluateExpression(node);
        // Se a expressão não for usada em um comando, o resultado é descartado
    }
    else if (node.type == "STRING_CONCAT") {
        std::string result = evaluateExpression(node);
        // Se a expressão não for usada em um comando, o resultado é descartado
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
        // Ignora nós do tipo EMPTY_STATEMENT
        return;
    }
    else {
        throw std::runtime_error("Nó AST desconhecido: " + node.type);
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
        throw std::runtime_error("Variável não definida: " + node.value);
    }
    else if (node.type == "BINARY_OP") {
        std::string left = evaluateExpression(node.children[0]);
        std::string right = evaluateExpression(node.children[1]);
        if (node.value == "+") {
            // Concatenação de strings ou adição de números
            return left + right;
        }
        // Adicione outros operadores aqui, se necessário
    }
    else if (node.type == "STRING_CONCAT") {
        std::string left = evaluateExpression(node.children[0]);
        std::string right = evaluateExpression(node.children[1]);
        return left + right; // Concatenação de strings
    }
    throw std::runtime_error("Expressão inválida: " + node.type);
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
    // Armazena a entrada em uma variável, se necessário
}

void Compiler::compileIf(const ASTNode& node) {
    std::string condition = evaluateExpression(node.children[0]);
    if (condition != "0" && condition != "") { // Condição verdadeira
        compileExpression(node.children[1]);
    }
    else if (node.children.size() > 2) { // Bloco else
        compileExpression(node.children[2]);
    }
}

void Compiler::compileVariableDeclaration(const ASTNode& node) {
    std::string varName = node.children[0].value;
    std::string value = evaluateExpression(node.children[2]);
    variables[varName] = value; // Armazena a variável e seu valor
}