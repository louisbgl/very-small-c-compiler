#include <iostream>
#include "codegen.hpp"

namespace codegen {

CodeGenerator::CodeGenerator(const std::unique_ptr<NodeProgram>& program)
    : ast(program) {
    generateCode();
}

std::string CodeGenerator::emitAssembly() const {
    return asmOutput.str();
}

void CodeGenerator::generateCode() {
    if (!ast || !ast->main) {
        std::cerr << "Error: No main function defined in the program." << std::endl;
        return;
    }

    writeAsm(".section .text\n");
    writeAsm("    global main\n");
    writeAsm("\n");
    writeAsm("main:\n");

    visitCompoundStatement(ast->main->body);
}

void CodeGenerator::writeAsm(std::string code) {
    asmOutput << code;
}

void CodeGenerator::visitFunction(const NodeFunction& function) {
    (void)function;
    throw std::runtime_error("[CodeGenerator::visitFunction] not implemented yet.");
}

void CodeGenerator::visitCompoundStatement(const NodeCompoundStatement& compound) {
    for (const auto& statement : compound.statements) {
        visitStatement(statement);
    }
}

void CodeGenerator::visitStatement(const NodeStatement& statement) {
    switch (statement.type) {
        case NodeStatement::StatementType::Empty:
            break; // No action for empty statements
        case NodeStatement::StatementType::Return:
            if (statement.expression) {
                visitExpression(*statement.expression);
            }
            writeAsm("    ret\n");
            break;
        default:
            throw std::runtime_error("[CodeGenerator::visitStatement] Unknown statement type");
    }
}

void CodeGenerator::visitExpression(const NodeExpression& expression) {
    std::visit([this](const auto& expr) {
        using T = std::decay_t<decltype(expr)>;
        if constexpr (std::is_same_v<T, NodeExpressionPrimary>) {
            visitExpressionPrimary(expr);
        } else if constexpr (std::is_same_v<T, NodeExpressionBinary>) {
            visitExpressionBinary(expr);
        } else {
            throw std::runtime_error("[CodeGenerator::visitExpression] Unknown expression type");
        }
    }, expression.value);
}

void CodeGenerator::visitExpressionPrimary(const NodeExpressionPrimary& primary) {
    writeAsm("    mov eax, " + std::to_string(primary.intValue) + "\n");
}

void CodeGenerator::visitExpressionBinary(const NodeExpressionBinary& binary) {
    // Visit left operand
    visitExpression(*binary.left);
    writeAsm("    push eax\n"); // Save left operand result

    // Visit right operand
    visitExpression(*binary.right);
    writeAsm("    mov ebx, eax\n"); // Move right operand result to ebx

    writeAsm("    pop eax\n"); // Restore left operand result

    // Apply binary operation: eax = eax (+|-) ebx
    switch (binary.op) {
        case NodeExpressionBinary::BinaryOperator::Add:
            writeAsm("    add eax, ebx\n");
            break;
        case NodeExpressionBinary::BinaryOperator::Subtract:
            writeAsm("    sub eax, ebx\n");
            break;
        default:
            throw std::runtime_error("[CodeGenerator::visitExpressionBinary] Unsupported operator");
    }
}

} // namespace codegen