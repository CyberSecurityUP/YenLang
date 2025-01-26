#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <stdexcept>

// Enumeration for token types
enum TokenType {
    NUMBER,         // Numeric values
    IDENTIFIER,     // Variable or function names
    OPERATOR,       // Operators like +, -, *, /
    KEYWORD,        // Reserved keywords like var, print, if
    PUNCTUATION,    // Symbols like ;, :, {, }
    STRING,         // String literals
    END_OF_FILE     // End of file marker
};

// Structure to represent a token
struct Token {
    TokenType type;         // Type of the token
    std::string value;      // Token value
    size_t line;            // Tracks the line number of the token
};

class Lexer {
public:
    // Constructor to initialize the lexer with source lines
    Lexer(const std::vector<std::string>& sourceLines);

    // Tokenize the input source lines into a vector of tokens
    std::vector<Token> tokenize();

private:
    std::vector<std::string> sourceLines; // Source code lines
    size_t currentLine;                   // Current line number being processed
    size_t position;                      // Current position in the line

    // Peek at the current character with an optional offset
    char peek(size_t offset = 0);

    // Advance to the next character and return it
    char advance();

    // Skip whitespace characters
    void skipWhitespace();

    // Read and tokenize a numeric value
    Token readNumber();

    // Read and tokenize an identifier or keyword
    Token readIdentifier();

    // Read and tokenize an operator
    Token readOperator();

    // Read and tokenize a punctuation symbol
    Token readPunctuation();

    // Read and tokenize a string literal
    Token readString(char delimiter);
};

#endif
