#include "parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), position(0) {}

ASTNode Parser::parse() {
    ASTNode program = { "PROGRAM", "", {} };
    while (peek().type != END_OF_FILE) {
        program.children.push_back(parseStatement());
    }
    return program;
}

Token Parser::peek() {
    return tokens[position];
}

Token Parser::advance() {
    return tokens[position++];
}

ASTNode Parser::parseStatement() {
    Token token = peek();
    if (token.type == KEYWORD) {
        if (token.value == "print") {
            return parsePrint();
        }
        else if (token.value == "input") {
            return parseInput();
        }
        else if (token.value == "if") {
            return parseIf();
        }
        else if (token.value == "var") {
            return parseVariableDeclaration();
        }
    }
    else if (token.type == IDENTIFIER) {
        // Variable declaration or assignment
        return parseAssignment();
    }
    else if (token.type == PUNCTUATION && token.value == ";") {
        // Semicolon (ignore)
        advance();
        return { "EMPTY_STATEMENT", "", {}, token.line };
    }
    return parseExpression();
}

ASTNode Parser::parseAssignment() {
    Token token = peek();
    ASTNode assignmentNode = { "ASSIGNMENT", "", {}, token.line };

    // Read the identifier (variable name)
    assignmentNode.children.push_back({ "IDENTIFIER", token.value, {}, token.line });
    advance();

    // Check for an assignment operator (=)
    if (peek().value != "=") {
        throw std::runtime_error("Expected assignment operator '=' on line " + std::to_string(token.line));
    }
    advance(); // Consume the assignment operator

    // Read the expression to the right of '='
    assignmentNode.children.push_back(parseExpression());

    // Check for a semicolon at the end
    if (peek().value != ";") {
        throw std::runtime_error("Expected ';' on line " + std::to_string(token.line));
    }
    advance(); // Consume the semicolon

    return assignmentNode;
}

ASTNode Parser::parseExpression() {
    ASTNode left = parseTerm();
    while (peek().type == OPERATOR && (peek().value == "+" || peek().value == "-" || peek().value == "=")) {
        std::string op = advance().value;

        // Check if it is a string concatenation
        if (op == "+" && (left.type == "STRING" || left.type == "IDENTIFIER")) {
            ASTNode right = parseTerm();
            if (right.type == "STRING" || right.type == "IDENTIFIER") {
                left = { "STRING_CONCAT", op, {left, right}, peek().line };
            }
            else {
                throw std::runtime_error("Invalid operands for string concatenation on line " + std::to_string(peek().line));
            }
        }
        else {
            // Otherwise, treat as a generic binary operation
            ASTNode right = parseTerm();
            left = { "BINARY_OP", op, {left, right}, peek().line };
        }
    }
    return left;
}

ASTNode Parser::parseTerm() {
    ASTNode left = parseFactor();
    while (peek().type == OPERATOR && (peek().value == "*" || peek().value == "/")) {
        std::string op = advance().value;
        ASTNode right = parseFactor();
        left = { "BINARY_OP", op, {left, right}, peek().line };
    }
    return left;
}

ASTNode Parser::parseFactor() {
    Token token = advance();
    if (token.type == NUMBER) {
        return { "NUMBER", token.value, {}, token.line };
    }
    else if (token.type == IDENTIFIER) {
        return { "IDENTIFIER", token.value, {}, token.line };
    }
    else if (token.type == STRING) {
        return { "STRING", token.value, {}, token.line };
    }
    else if (token.type == PUNCTUATION && token.value == "(") {
        ASTNode expr = parseExpression();
        if (advance().value != ")") {
            throw std::runtime_error("Expected ')' on line " + std::to_string(token.line));
        }
        return expr;
    }
    else {
        throw std::runtime_error("Unexpected factor on line " + std::to_string(token.line) + ": " + token.value);
    }
}

ASTNode Parser::parsePrint() {
    Token token = peek();
    ASTNode printNode = { "PRINT", "", {}, token.line };
    advance(); // Consume 'print'
    if (advance().value != "(") {
        throw std::runtime_error("Expected '(' on line " + std::to_string(token.line));
    }
    printNode.children.push_back(parseExpression()); // Argument
    if (peek().value != ")") {
        throw std::runtime_error("Expected ')' on line " + std::to_string(token.line));
    }
    advance(); // Consume ')'
    return printNode;
}

ASTNode Parser::parseInput() {
    Token token = advance(); // Consume 'input'
    ASTNode inputNode = { "INPUT", "", {}, token.line };
    if (advance().value != "(") {
        throw std::runtime_error("Expected '(' on line " + std::to_string(token.line));
    }

    // Check if there is an argument
    if (peek().type == STRING) {
        inputNode.children.push_back(parseExpression()); // Read the argument
    }

    if (advance().value != ")") {
        throw std::runtime_error("Expected ')' on line " + std::to_string(token.line));
    }

    return inputNode;
}

ASTNode Parser::parseIf() {
    Token token = peek();
    ASTNode ifNode = { "IF", "", {}, token.line };
    advance(); // Consume 'if'

    // Check for '(' after 'if'
    if (advance().value != "(") {
        throw std::runtime_error("Expected '(' on line " + std::to_string(token.line));
    }

    // Read the condition of the 'if'
    ifNode.children.push_back(parseExpression());

    // Check for ')' after the condition
    if (advance().value != ")") {
        throw std::runtime_error("Expected ')' on line " + std::to_string(token.line));
    }

    // Check for '{' for the true block
    if (advance().value != "{") {
        throw std::runtime_error("Expected '{' on line " + std::to_string(token.line));
    }

    // Read the true block
    ifNode.children.push_back(parseStatement());

    // Check for '}' to close the true block
    if (advance().value != "}") {
        throw std::runtime_error("Expected '}' on line " + std::to_string(token.line));
    }

    // Check for 'else'
    if (peek().value == "else") {
        advance(); // Consume 'else'

        // Check for '{' for the false block
        if (advance().value != "{") {
            throw std::runtime_error("Expected '{' on line " + std::to_string(token.line));
        }

        // Read the false block
        ifNode.children.push_back(parseStatement());

        // Check for '}' to close the false block
        if (advance().value != "}") {
            throw std::runtime_error("Expected '}' on line " + std::to_string(token.line));
        }
    }

    return ifNode;
}

ASTNode Parser::parseVariableDeclaration() {
    Token token = advance(); // Consume 'var'
    ASTNode varNode = { "VAR", "", {}, token.line };

    // Read the variable name
    if (peek().type != IDENTIFIER) {
        throw std::runtime_error("Expected identifier on line " + std::to_string(token.line));
    }
    varNode.children.push_back({ "IDENTIFIER", peek().value, {}, token.line });
    advance();

    // Read the variable type
    if (advance().value != ":") {
        throw std::runtime_error("Expected ':' on line " + std::to_string(token.line));
    }
    if (peek().type != IDENTIFIER) {
        throw std::runtime_error("Expected type on line " + std::to_string(token.line));
    }
    varNode.children.push_back({ "TYPE", peek().value, {}, token.line });
    advance();

    // Read the variable value
    if (advance().value != "=") {
        throw std::runtime_error("Expected '=' on line " + std::to_string(token.line));
    }
    varNode.children.push_back(parseExpression()); // Value

    // Check for a semicolon at the end
    if (peek().value != ";") {
        throw std::runtime_error("Expected ';' on line " + std::to_string(token.line));
    }
    advance(); // Consume the semicolon

    return varNode;
}
