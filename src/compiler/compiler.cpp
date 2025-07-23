#include "compiler.hpp"
#include <iostream>

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
        std::cout << codegen->emitAssembly();
    }
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