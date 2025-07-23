#pragma once

#include <sstream>
#include <memory>
#include "../parser/parser.hpp"

namespace codegen {

class CodeGenerator {
public:
    explicit CodeGenerator(const std::unique_ptr<NodeProgram>& program);

    std::string emitAssembly() const;
    
private:
    const std::unique_ptr<NodeProgram>& ast;
    std::ostringstream asmOutput;

    void generateCode();

    /* Helper functions */

    void writeAsm(std::string code);
    void visitFunction(const NodeFunction& function);
    void visitCompoundStatement(const NodeCompoundStatement& compound);
    void visitStatement(const NodeStatement& statement);
    void visitExpression(const NodeExpression& expression);

};

} // namespace codegen