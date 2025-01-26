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
        // Declaração de variável ou atribuição
        return parseAssignment();
    }
    else if (token.type == PUNCTUATION && token.value == ";") {
        // Ponto e vírgula (ignorar)
        advance();
        return { "EMPTY_STATEMENT", "", {}, token.line };
    }
    return parseExpression();
}

ASTNode Parser::parseAssignment() {
    Token token = peek();
    ASTNode assignmentNode = { "ASSIGNMENT", "", {}, token.line };

    // Lê o identificador (nome da variável)
    assignmentNode.children.push_back({ "IDENTIFIER", token.value, {}, token.line });
    advance();

    // Verifica se há um operador de atribuição (=)
    if (peek().value != "=") {
        throw std::runtime_error("Esperado operador de atribuição '=' na linha " + std::to_string(token.line));
    }
    advance(); // Consome o operador de atribuição

    // Lê a expressão à direita do '='
    assignmentNode.children.push_back(parseExpression());

    // Verifica se há um ponto e vírgula no final
    if (peek().value != ";") {
        throw std::runtime_error("Esperado ';' na linha " + std::to_string(token.line));
    }
    advance(); // Consome o ponto e vírgula

    return assignmentNode;
}

ASTNode Parser::parseExpression() {
    ASTNode left = parseTerm();
    while (peek().type == OPERATOR && (peek().value == "+" || peek().value == "-" || peek().value == "=")) {
        std::string op = advance().value;

        // Verifica se é uma concatenação de strings
        if (op == "+" && (left.type == "STRING" || left.type == "IDENTIFIER")) {
            ASTNode right = parseTerm();
            if (right.type == "STRING" || right.type == "IDENTIFIER") {
                left = { "STRING_CONCAT", op, {left, right}, peek().line };
            }
            else {
                throw std::runtime_error("Operandos inválidos para concatenação de strings na linha " + std::to_string(peek().line));
            }
        }
        else {
            // Caso contrário, trata como uma operação binária genérica
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
            throw std::runtime_error("Esperado ')' na linha " + std::to_string(token.line));
        }
        return expr;
    }
    else {
        throw std::runtime_error("Fator inesperado na linha " + std::to_string(token.line) + ": " + token.value);
    }
}

ASTNode Parser::parsePrint() {
    Token token = peek();
    ASTNode printNode = { "PRINT", "", {}, token.line };
    advance(); // Consumir 'print'
    if (advance().value != "(") {
        throw std::runtime_error("Esperado '(' na linha " + std::to_string(token.line));
    }
    printNode.children.push_back(parseExpression()); // Argumento
    if (peek().value != ")") {
        throw std::runtime_error("Esperado ')' na linha " + std::to_string(token.line));
    }
    advance(); // Consumir ')'
    return printNode;
}

ASTNode Parser::parseInput() {
    Token token = advance(); // Consumir 'input'
    ASTNode inputNode = { "INPUT", "", {}, token.line };
    if (advance().value != "(") {
        throw std::runtime_error("Esperado '(' na linha " + std::to_string(token.line));
    }

    // Verifica se há um argumento
    if (peek().type == STRING) {
        inputNode.children.push_back(parseExpression()); // Lê o argumento
    }

    if (advance().value != ")") {
        throw std::runtime_error("Esperado ')' na linha " + std::to_string(token.line));
    }

    return inputNode;
}

ASTNode Parser::parseIf() {
    Token token = peek();
    ASTNode ifNode = { "IF", "", {}, token.line };
    advance(); // Consumir 'if'

    // Verifica se há um '(' após o 'if'
    if (advance().value != "(") {
        throw std::runtime_error("Esperado '(' na linha " + std::to_string(token.line));
    }

    // Lê a condição do 'if'
    ifNode.children.push_back(parseExpression());

    // Verifica se há um ')' após a condição
    if (advance().value != ")") {
        throw std::runtime_error("Esperado ')' na linha " + std::to_string(token.line));
    }

    // Verifica se há um '{' para o bloco verdadeiro
    if (advance().value != "{") {
        throw std::runtime_error("Esperado '{' na linha " + std::to_string(token.line));
    }

    // Lê o bloco verdadeiro
    ifNode.children.push_back(parseStatement());

    // Verifica se há um '}' para fechar o bloco verdadeiro
    if (advance().value != "}") {
        throw std::runtime_error("Esperado '}' na linha " + std::to_string(token.line));
    }

    // Verifica se há um 'else'
    if (peek().value == "else") {
        advance(); // Consumir 'else'

        // Verifica se há um '{' para o bloco falso
        if (advance().value != "{") {
            throw std::runtime_error("Esperado '{' na linha " + std::to_string(token.line));
        }

        // Lê o bloco falso
        ifNode.children.push_back(parseStatement());

        // Verifica se há um '}' para fechar o bloco falso
        if (advance().value != "}") {
            throw std::runtime_error("Esperado '}' na linha " + std::to_string(token.line));
        }
    }

    return ifNode;
}
ASTNode Parser::parseVariableDeclaration() {
    Token token = advance(); // Consumir 'var'
    ASTNode varNode = { "VAR", "", {}, token.line };

    // Lê o nome da variável
    if (peek().type != IDENTIFIER) {
        throw std::runtime_error("Esperado identificador na linha " + std::to_string(token.line));
    }
    varNode.children.push_back({ "IDENTIFIER", peek().value, {}, token.line });
    advance();

    // Lê o tipo da variável
    if (advance().value != ":") {
        throw std::runtime_error("Esperado ':' na linha " + std::to_string(token.line));
    }
    if (peek().type != IDENTIFIER) {
        throw std::runtime_error("Esperado tipo na linha " + std::to_string(token.line));
    }
    varNode.children.push_back({ "TYPE", peek().value, {}, token.line });
    advance();

    // Lê o valor da variável
    if (advance().value != "=") {
        throw std::runtime_error("Esperado '=' na linha " + std::to_string(token.line));
    }
    varNode.children.push_back(parseExpression()); // Valor

    // Verifica se há um ponto e vírgula no final
    if (peek().value != ";") {
        throw std::runtime_error("Esperado ';' na linha " + std::to_string(token.line));
    }
    advance(); // Consome o ponto e vírgula

    return varNode;
}