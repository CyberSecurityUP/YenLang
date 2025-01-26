#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "lexer.h"
#include "parser.h"
#include "compiler.h"

std::vector<std::string> readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Erro ao abrir o arquivo: " + filename);
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
        std::cerr << "Uso: " << argv[0] << " <arquivo.yen>" << std::endl;
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

        std::cout << "Compilação concluída com sucesso!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}