#include "lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::vector<std::string>& sourceLines)
    : sourceLines(sourceLines), currentLine(0), position(0) {
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (currentLine < sourceLines.size()) {
        while (position < sourceLines[currentLine].length()) {
            char current = peek();
            if (std::isspace(current)) {
                skipWhitespace();
            }
            else if (std::isdigit(current)) {
                tokens.push_back(readNumber());
            }
            else if (std::isalpha(current)) {
                tokens.push_back(readIdentifier());
            }
            else if (current == '+' || current == '-' || current == '*' || current == '/' || current == '=' || current == '!' || current == '<' || current == '>') {
                tokens.push_back(readOperator());
            }
            else if (current == ';' || current == '(' || current == ')' || current == '{' || current == '}' || current == ':') {
                tokens.push_back(readPunctuation());
            }
            else if (current == '"' || current == '\'') {
                tokens.push_back(readString(current));
            }
            else {
                throw std::runtime_error("Unexpected character on line " + std::to_string(currentLine + 1) + ": " + std::string(1, current));
            }
        }
        currentLine++;
        position = 0;
    }
    tokens.push_back({ END_OF_FILE, "", currentLine });
    return tokens;
}

char Lexer::peek(size_t offset) {
    if (position + offset < sourceLines[currentLine].length()) {
        return sourceLines[currentLine][position + offset];
    }
    return '\0';
}

char Lexer::advance() {
    return sourceLines[currentLine][position++];
}

void Lexer::skipWhitespace() {
    while (position < sourceLines[currentLine].length() && std::isspace(peek())) {
        advance();
    }
}

Token Lexer::readNumber() {
    std::string value;
    while (position < sourceLines[currentLine].length() && (std::isdigit(peek()) || peek() == '.')) {
        value += advance();
    }
    return { NUMBER, value, currentLine };
}

Token Lexer::readIdentifier() {
    std::string value;
    while (position < sourceLines[currentLine].length() && std::isalnum(peek())) {
        value += advance();
    }
    // Check if it is a keyword
    if (value == "var" || value == "print" || value == "input" || value == "if" || value == "else") {
        return { KEYWORD, value, currentLine };
    }
    return { IDENTIFIER, value, currentLine };
}

Token Lexer::readOperator() {
    char current = advance();
    std::string op(1, current);

    // Check if it is a compound operator (==, !=, <=, >=)
    if (position < sourceLines[currentLine].length()) {
        char next = peek();
        if ((current == '=' && next == '=') || // ==
            (current == '!' && next == '=') || // !=
            (current == '<' && next == '=') || // <=
            (current == '>' && next == '=')) { // >=
            op += next;
            advance();
        }
    }

    return { OPERATOR, op, currentLine };
}

Token Lexer::readPunctuation() {
    char current = advance();
    std::string value(1, current);

    // Check if it is a compound operator (::)
    if (current == ':' && peek() == ':') {
        value += advance();
    }

    return { PUNCTUATION, value, currentLine };
}

Token Lexer::readString(char delimiter) {
    std::string value;
    advance(); // Consume the opening delimiter
    while (position < sourceLines[currentLine].length() && peek() != delimiter) {
        value += advance();
    }
    if (peek() == delimiter) {
        advance(); // Consume the closing delimiter
    }
    else {
        throw std::runtime_error("Unclosed string on line " + std::to_string(currentLine + 1));
    }
    return { STRING, value, currentLine };
}
