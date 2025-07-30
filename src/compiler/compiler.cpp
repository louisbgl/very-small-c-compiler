#include "compiler.hpp"
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"
#include "../codegen/codegen.hpp"
#include <iostream>
#include <fstream>
#include <sys/wait.h>
#include <cstdio>

namespace compiler {

Compiler::Compiler(std::string_view source)
    : source(source) {
    compile();
}

void Compiler::emitAssembly() {
    printAssembly();
}

void Compiler::printTokens() const {
    if (lexer) {
        lexer->printTokens();
    }
}

void Compiler::printAST() const {
    if (parser) {
        parser->printProgram();
    }
}

void Compiler::printAssembly() const {
    if (codegen) {
        std::cout << codegen->generate();
    }
}

void Compiler::saveAssemblyToFile(const std::string& filename) const {
    if (!codegen) {
        std::cerr << "Error: No code generated to save." << std::endl;
        return;
    }
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing." << std::endl;
        return;
    }
    
    file << codegen->generate();
    file.close();
    
    std::cout << "Assembly saved to: " << filename << std::endl;
}

int Compiler::assembleAndExecute(const std::string& asmFilename, const std::string& exeFilename) const {
    // Save assembly to file
    saveAssemblyToFile(asmFilename);
    
    // Use GNU assembler (as) for x86-64 with Intel syntax
    std::string assembleCmd = "as --64 " + asmFilename + " -o " + exeFilename + ".o";
    std::string linkCmd = "ld " + exeFilename + ".o -o " + exeFilename;
    
    // Assemble
    std::cout << "Assembling: " << assembleCmd << std::endl;
    int assembleResult = system(assembleCmd.c_str());
    if (assembleResult != 0) {
        std::cerr << "Error: Assembly failed." << std::endl;
        return assembleResult;
    }
    
    // Link
    std::cout << "Linking: " << linkCmd << std::endl;
    int linkResult = system(linkCmd.c_str());
    if (linkResult != 0) {
        std::cerr << "Error: Linking failed." << std::endl;
        return linkResult;
    }
    
    // Execute
    std::string execCmd = "./" + exeFilename;
    std::cout << "Executing: " << execCmd << std::endl;
    int execResult = system(execCmd.c_str());
    
    std::cout << "Program exited with code: " << WEXITSTATUS(execResult) << std::endl;
    
    // Clean up temporary files
    std::remove(asmFilename.c_str());
    std::remove((exeFilename + ".o").c_str());
    std::remove(exeFilename.c_str());
    
    return WEXITSTATUS(execResult);
}

void Compiler::compile() {
    // Step 1: Tokenize the source code
    lexer = std::make_unique<lexer::Lexer>(source);
    
    // Step 2: Parse the tokens into an AST
    parser = std::make_unique<parser::Parser>(*lexer);

    // Step 3: Generate code from the AST
    codegen = std::make_unique<codegen::CodeGenerator>(parser->getProgram());
}

} // namespace compiler