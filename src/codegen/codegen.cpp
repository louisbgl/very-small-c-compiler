#include <assert.h>
#include "codegen.hpp"

namespace codegen {

CodeGenerator::CodeGenerator(const std::unique_ptr<NodeProgram>& program)
    : ast(program), generated(false) {}

std::string CodeGenerator::generate() {
    if (!generated) {
        analyze();       // Phase 1: Analyze the AST and build the symbol table
        generateCode();  // Phase 2: Generate assembly code based on the analyzed AST
        generated = true;
    }
    return asmOutput;
}

void CodeGenerator::analyze() {
    VisitorAnalyzer analyzer;
    analyzer.analyze(*ast);

    // Transfer ownership of the analyzed scope structure
    globalScope = analyzer.releaseRootScope();
}

void CodeGenerator::generateCode() {
    VisitorGenerator generator(globalScope.get());
    asmOutput = generator.generate(*ast);
}

} // namespace codegen