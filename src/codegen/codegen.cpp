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

    writeAsm(".intel_syntax noprefix\n");
    writeAsm(".section .text\n");
    writeAsm("    .globl _start\n");
    writeAsm("\n");
    writeAsm("_start:\n");
    writeAsm("    call main\n");
    writeAsm("    mov rdi, rax\n");    // Move return value to exit code (first argument)
    writeAsm("    mov rax, 60\n");     // sys_exit system call number (64-bit)
    writeAsm("    syscall\n");         // Invoke system call
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
    std::visit([this](const auto& value) {
        using ValueType = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<ValueType, int>) {
            writeAsm("    mov rax, " + std::to_string(value) + "\n");
        } else if constexpr (std::is_same_v<ValueType, std::unique_ptr<NodeExpression>>) {
            visitExpression(*value);
        }
    }, primary.value);
}

void CodeGenerator::visitExpressionBinary(const NodeExpressionBinary& binary) {
    // Visit left operand
    visitExpression(*binary.left);
    writeAsm("    push rax\n"); // Save left operand result

    // Visit right operand
    visitExpression(*binary.right);
    writeAsm("    mov rbx, rax\n"); // Move right operand result to rbx

    writeAsm("    pop rax\n"); // Restore left operand result

    // Apply binary operation: rax = rax (+|-) rbx
    switch (binary.op) {
        case NodeExpressionBinary::BinaryOperator::Add:
            writeAsm("    add rax, rbx\n");
            break;
        case NodeExpressionBinary::BinaryOperator::Subtract:
            writeAsm("    sub rax, rbx\n");
            break;
        default:
            throw std::runtime_error("[CodeGenerator::visitExpressionBinary] Unsupported operator");
    }
}

} // namespace codegen