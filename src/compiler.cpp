#include "compiler.h"
#include <iostream>
#include <map>
#include <stdexcept>
#include <sstream>

// Implementation of the Compiler class methods

void Compiler::compile(const ASTNode& ast) {
    if (ast.type == "PROGRAM") {
        for (const auto& child : ast.children) {
            compileExpression(child);
        }
    }
    else {
        throw std::runtime_error("Unknown AST node: " + ast.type);
    }
}

void Compiler::compileExpression(const ASTNode& node) {
    if (node.type == "BINARY_OP") {
        std::string result = evaluateExpression(node);
        // If the expression is not used in a command, the result is discarded
    }
    else if (node.type == "STRING_CONCAT") {
        std::string result = evaluateExpression(node);
        // If the expression is not used in a command, the result is discarded
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
        // Ignore nodes of type EMPTY_STATEMENT
        return;
    }
    else {
        throw std::runtime_error("Unknown AST node: " + node.type);
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
        throw std::runtime_error("Undefined variable: " + node.value);
    }
    else if (node.type == "BINARY_OP") {
        std::string left = evaluateExpression(node.children[0]);
        std::string right = evaluateExpression(node.children[1]);
        if (node.value == "+") {
            // String concatenation or number addition
            return left + right;
        }
        // Add other operators here if necessary
    }
    else if (node.type == "STRING_CONCAT") {
        std::string left = evaluateExpression(node.children[0]);
        std::string right = evaluateExpression(node.children[1]);
        return left + right; // String concatenation
    }
    throw std::runtime_error("Invalid expression: " + node.type);
}

void Compiler::compilePrint(const ASTNode& node) {
    std::string result = evaluateExpression(node.children[0]);
    std::cout << result << std::endl; // Display the result
}

void Compiler::compileInput(const ASTNode& node) {
    std::string prompt = "";
    if (!node.children.empty()) {
        prompt = evaluateExpression(node.children[0]);
    }
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    // Store the input in a variable, if necessary
}

void Compiler::compileIf(const ASTNode& node) {
    std::string condition = evaluateExpression(node.children[0]);
    if (condition != "0" && condition != "") { // True condition
        compileExpression(node.children[1]);
    }
    else if (node.children.size() > 2) { // Else block
        compileExpression(node.children[2]);
    }
}

void Compiler::compileVariableDeclaration(const ASTNode& node) {
    std::string varName = node.children[0].value;
    std::string value = evaluateExpression(node.children[2]);
    variables[varName] = value; // Store the variable and its value
}
