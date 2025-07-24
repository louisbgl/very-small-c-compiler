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
    writeAsm("    .globl _start\n");
    writeAsm("\n");
    writeAsm("_start:\n");
    writeAsm("    call main\n");
    writeAsm("    movq %rax, %rdi\n");    // Move return value to exit code (first argument)
    writeAsm("    movq $60, %rax\n");     // sys_exit system call number (64-bit)
    writeAsm("    syscall\n");            // Invoke system call
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
    writeAsm("    movq $" + std::to_string(primary.intValue) + ", %rax\n");
}

void CodeGenerator::visitExpressionBinary(const NodeExpressionBinary& binary) {
    // Visit left operand
    visitExpression(*binary.left);
    writeAsm("    pushq %rax\n"); // Save left operand result

    // Visit right operand
    visitExpression(*binary.right);
    writeAsm("    movq %rax, %rbx\n"); // Move right operand result to rbx

    writeAsm("    popq %rax\n"); // Restore left operand result

    // Apply binary operation: rax = rax (+|-) rbx
    switch (binary.op) {
        case NodeExpressionBinary::BinaryOperator::Add:
            writeAsm("    addq %rbx, %rax\n");
            break;
        case NodeExpressionBinary::BinaryOperator::Subtract:
            writeAsm("    subq %rbx, %rax\n");
            break;
        default:
            throw std::runtime_error("[CodeGenerator::visitExpressionBinary] Unsupported operator");
    }
}

} // namespace codegen