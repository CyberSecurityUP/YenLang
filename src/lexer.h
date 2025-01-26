#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <stdexcept>

enum TokenType {
    NUMBER,
    IDENTIFIER,
    OPERATOR,
    KEYWORD,
    PUNCTUATION,
    STRING,
    END_OF_FILE
};

struct Token {
    TokenType type;
    std::string value;
    size_t line; // Adicionado para rastrear a linha do token
};

class Lexer {
public:
    Lexer(const std::vector<std::string>& sourceLines);
    std::vector<Token> tokenize();

private:
    std::vector<std::string> sourceLines;
    size_t currentLine;
    size_t position;

    char peek(size_t offset = 0); // Aceita um argumento opcional
    char advance();
    void skipWhitespace();
    Token readNumber();
    Token readIdentifier();
    Token readOperator();
    Token readPunctuation();
    Token readString(char delimiter);
};

#endif