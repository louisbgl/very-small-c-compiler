#pragma once

#include <string_view>
#include <memory>
#include "../lexer/lexer.hpp"
#include "../parser/parser.hpp"
#include "../codegen/codegen.hpp"

namespace compiler {

class Compiler {
public:
    explicit Compiler(std::string_view source);

    /* Emit the final assembly code */
    void emitAssembly();

    /* Save assembly to file and optionally execute */
    void saveAssemblyToFile(const std::string& filename) const;
    int assembleAndExecute(const std::string& asmFilename = "output.s", 
                          const std::string& exeFilename = "output") const;

    /* Print tokens and AST for debugging */
    void printTokens() const;
    void printAST() const;
    void printAssembly() const;
    
private:
    std::string_view source;
    std::unique_ptr<lexer::Lexer> lexer;
    std::unique_ptr<parser::Parser> parser;
    std::unique_ptr<codegen::CodeGenerator> codegen;

    /* Compile the source code, called from the constructor */
    void compile();
};

} // namespace compiler