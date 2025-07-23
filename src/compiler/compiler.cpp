#include "compiler.hpp"
#include <iostream>

namespace compiler {

Compiler::Compiler(std::string_view source)
    : source(source) {
    compile();
}

void Compiler::emitAssembly() {
    // Placeholder for assembly emission logic
    std::cout << "NOT IMPLEMENTED" << std::endl;
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

void Compiler::compile() {
    // Step 1: Tokenize the source code
    lexer = std::make_unique<lexer::Lexer>(source);
    
    // Step 2: Parse the tokens into an AST
    parser = std::make_unique<parser::Parser>(*lexer);
}

} // namespace compiler