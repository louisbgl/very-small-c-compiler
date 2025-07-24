#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "src/compiler/compiler.hpp"

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
    
    std::cout << "=> FILE: " << argv[1] << std::endl;
    
    // Compile the source code
    compiler::Compiler compiler(sourceCode);
    
    // Print compilation results
    std::cout << "====== Start of Tokens ======" << std::endl;
    compiler.printTokens();
    std::cout << "====== End of Tokens ========\n" << std::endl;

    std::cout << "====== Parsing Program ======" << std::endl;
    compiler.printAST();
    std::cout << "====== End of Parsing =======\n" << std::endl;

    std::cout << "====== Emitting Assembly =====" << std::endl;
    compiler.emitAssembly();
    std::cout << "====== End of Assembly ======\n" << std::endl;

    std::cout << "====== Assembling and Executing =====" << std::endl;
    int exitCode = compiler.assembleAndExecute("output.s", "output");
    std::cout << "====== End of Execution =====\n" << std::endl;
    
    return exitCode;
}