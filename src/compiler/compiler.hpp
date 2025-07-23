#pragma once

#include <string_view>
#include <memory>
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"

namespace compiler {

class Compiler {
public:
    explicit Compiler(std::string_view source);

    /* Emit the final assembly code */
    void emitAssembly();

    /* Print tokens and AST for debugging */

    void printTokens() const;
    void printAST() const;
    
private:
    std::string_view source;
    std::unique_ptr<lexer::Lexer> lexer;
    std::unique_ptr<parser::Parser> parser;

    /* Compile the source code, called from the constructor */
    void compile();
};

} // namespace compiler