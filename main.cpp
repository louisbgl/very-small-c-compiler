#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "src/lexer/lexer.hpp"
#include "src/parser/parser.hpp"

std::string readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) {
    // Check if a file was provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }
    
    // Read the source file
    std::string sourceCode = readFile(argv[1]);
    if (sourceCode.empty()) {
        return 1;
    }
    
    // Create lexer and tokenize
    lexer::Lexer lexer(sourceCode);
    
    // Print all tokens
    std::cout << "=> FILE: " << argv[1] << std::endl;
    std::cout << "====== Start of Tokens ======" << std::endl;
    lexer.printTokens();
    std::cout << "====== End of Tokens ========\n" << std::endl;

    // Create parser and parse the tokens
    parser::Parser parser(lexer);
    std::cout << "====== Parsing Program ======" << std::endl;
    parser.printProgram();
    std::cout << "====== End of Parsing =======\n" << std::endl;
    return 0;
}