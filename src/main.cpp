#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "compiler.h"

// Function to read the contents of a file into a vector of strings
std::vector<std::string> readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file.yen>" << std::endl;
        return 1;
    }

    try {
        std::string filename = argv[1];
        std::vector<std::string> sourceLines = readFile(filename);

        // Lexer
        Lexer lexer(sourceLines);
        std::vector<Token> tokens = lexer.tokenize();

        // Parser
        Parser parser(tokens);
        ASTNode ast = parser.parse();

        // Compiler
        Compiler compiler;
        compiler.compile(ast);

        std::cout << "Build completed successfully!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
