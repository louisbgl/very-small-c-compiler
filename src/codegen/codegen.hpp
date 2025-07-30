#pragma once

#include <sstream>
#include <memory>
#include "../parser/parser.hpp"
#include "scopeNode.hpp"
#include "visitorAnalyzer.hpp"
#include "visitorGenerator.hpp"

namespace codegen {

class CodeGenerator {
public:
    explicit CodeGenerator(const std::unique_ptr<NodeProgram>& program);

    std::string generate();

private:
    const std::unique_ptr<NodeProgram>& ast;
    std::string asmOutput;
    bool generated;

    std::unique_ptr<ScopeNode> globalScope;

    void analyze();       // Phase 1
    void generateCode();  // Phase 2
};

} // namespace codegen