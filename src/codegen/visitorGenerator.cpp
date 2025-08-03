#include "visitorGenerator.hpp"
#include <assert.h>
#include <iostream>

VisitorGenerator::VisitorGenerator(ScopeNode* rootScope)
    : asmOutput(""), currentScope(rootScope), childScopeIndexes(), labelCounter(0) {}

std::string VisitorGenerator::generate(const NodeProgram& ast) {
    writeAsm(".intel_syntax noprefix");
    writeAsm(".section .text");
    writeAsm("    .globl _start");
    writeAsm("");
    writeAsm("_start:");
    writeAsm("    call main");
    writeAsm("    mov rdi, rax");
    writeAsm("    mov rax, 60");
    writeAsm("    syscall");
    writeAsm("");
    
    childScopeIndexes.push_back(0);
    for (const auto& function : ast.functions) {
        visitFunction(function);
    }
    childScopeIndexes.pop_back();
    return asmOutput;
}

void VisitorGenerator::visitFunction(const NodeFunction& function) {
    currentScope = &currentScope->getChild(childScopeIndexes.back()++);

    writeAsm(".globl " + function.name);
    writeAsm(function.name + ":");
    writeAsm("push rbp");
    writeAsm("mov rbp, rsp");
    
    childScopeIndexes.push_back(0);
    visitCompoundStatement(function.body);
    childScopeIndexes.pop_back();
    
    currentScope = currentScope->getParent();
}

void VisitorGenerator::visitCompoundStatement(const NodeCompoundStatement& compound) {
    currentScope = &currentScope->getChild(childScopeIndexes.back()++);
    childScopeIndexes.push_back(0);

    int frameSize = currentScope->getFrameSize();
    if (frameSize > 0) {
        writeAsm("sub rsp, " + std::to_string(frameSize));
    }

    for (const auto& statement : compound.statements) {
        visitStatement(statement);
    }

    currentScope = currentScope->getParent();
    childScopeIndexes.pop_back();
}

void VisitorGenerator::visitStatement(const NodeStatement& statement) {
    std::visit([this](const auto& stmt) {
        using T = std::decay_t<decltype(stmt)>;
        if constexpr (std::is_same_v<T, NodeStatementEmpty>) {
            // IGNORE empty statements while generating code
        } else if constexpr (std::is_same_v<T, NodeStatementReturn>) {
            visitStatementReturn(stmt);
        } else if constexpr (std::is_same_v<T, NodeStatementVarDecl>) {
            visitStatementVarDecl(stmt);
        } else if constexpr (std::is_same_v<T, NodeStatementAssignment>) {
            visitStatementAssignment(stmt);
        } else if constexpr (std::is_same_v<T, NodeStatementIf>) {
            visitStatementIf(stmt);
        } else if constexpr (std::is_same_v<T, NodeStatementWhile>) {
            visitStatementWhile(stmt);
        } else {
            throw std::runtime_error("[VisitorGenerator::visitStatement] Unknown statement type");
        }
    }, statement.value);
}

void VisitorGenerator::visitExpression(const NodeExpression& expression) {
    std::visit([this](const auto& expr) {
        using T = std::decay_t<decltype(expr)>;
        if constexpr (std::is_same_v<T, NodeExpressionPrimary>) {
            visitExpressionPrimary(expr);
        } else if constexpr (std::is_same_v<T, NodeExpressionBinary>) {
            visitExpressionBinary(expr);
        } else if constexpr (std::is_same_v<T, NodeExpressionComparison>) {
            visitExpressionComparison(expr);
        } else if constexpr (std::is_same_v<T, NodeExpressionFunctionCall>) {
            visitExpressionFunctionCall(expr);
        } else {
            throw std::runtime_error("[VisitorGenerator::visitExpression] Unknown expression type");
        }
    }, expression.value);
}

void VisitorGenerator::visitStatementVarDecl(const NodeStatementVarDecl& varDecl) {
    auto offsetOpt = currentScope->getOffset(varDecl.identifier);
    if (!offsetOpt.has_value()) {
        throw std::runtime_error("[VisitorGenerator::visitStatementVarDecl] Variable '" + varDecl.identifier + "' not found in scope");
    }
    int offset = offsetOpt.value();

    // If there's an initializer, compute its value and store it
    if (varDecl.initializer.has_value()) {
        visitExpression(varDecl.initializer.value());
        writeAsm("mov [rbp - " + std::to_string(offset) + "], rax");
    }
    else {
        writeAsm("mov qword ptr [rbp - " + std::to_string(offset) + "], 0");
    }
}

void VisitorGenerator::visitStatementReturn(const NodeStatementReturn& returnStmt) {
    if (returnStmt.expression.has_value()) {
        visitExpression(returnStmt.expression.value());
    } else {
        writeAsm("mov rax, 0");
    }
    writeAsm("leave");
    writeAsm("ret\n");
}

void VisitorGenerator::visitStatementAssignment(const NodeStatementAssignment& assignment) {
    auto offsetOpt = currentScope->getOffsetRecursive(assignment.identifier);
    if (!offsetOpt.has_value()) {
        throw std::runtime_error("[VisitorGenerator::visitStatementAssignment] Variable '" + assignment.identifier + "' not found in scope");
    }
    int offset = offsetOpt.value();

    visitExpression(assignment.expression);

    writeAsm("mov [rbp - " + std::to_string(offset) + "], rax");
}

void VisitorGenerator::visitStatementIf(const NodeStatementIf& ifStmt) {
    const std::string elseLabel = "else_label_" + std::to_string(labelCounter++);
    const std::string endLabel = "end_label_" + std::to_string(labelCounter++);

    visitExpression(ifStmt.condition);

    writeAsm("test rax, rax");
    writeAsm("jz " + elseLabel);

    visitCompoundStatement(*ifStmt.body);
    writeAsm("jmp " + endLabel);

    writeAsm(elseLabel + ":");
    if (ifStmt.elseBody.has_value()) {
        visitCompoundStatement(*ifStmt.elseBody.value());
    }
    writeAsm(endLabel + ":");
}

void VisitorGenerator::visitStatementWhile(const NodeStatementWhile& whileStmt) {
    const std::string startLabel = "while_start_" + std::to_string(labelCounter++);
    const std::string endLabel = "while_end_" + std::to_string(labelCounter++);

    writeAsm(startLabel + ":");

    visitExpression(whileStmt.condition);
    writeAsm("test rax, rax");
    writeAsm("jz " + endLabel);

    visitCompoundStatement(*whileStmt.body);
    
    writeAsm("jmp " + startLabel);
    writeAsm(endLabel + ":");
}

void VisitorGenerator::visitExpressionPrimary(const NodeExpressionPrimary& primary) {
    std::visit([this](const auto& value) {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, int>) {
            writeAsm("mov rax, " + std::to_string(value));
        } else if constexpr (std::is_same_v<T, std::string>) {
            auto offsetOpt = currentScope->getOffsetRecursive(value);
            if (!offsetOpt.has_value()) {
                throw std::runtime_error("[VisitorGenerator::visitExpressionPrimary] Unknown identifier: " + value);
            }
            int offset = offsetOpt.value();
            writeAsm("mov rax, [rbp - " + std::to_string(offset) + "]");
        } else if constexpr (std::is_same_v<T, std::unique_ptr<NodeExpression>>) {
            visitExpression(*value);
        } else {
            throw std::runtime_error("[VisitorGenerator::visitExpressionPrimary] Unknown primary type");
        }
    }, primary.value);
}

void VisitorGenerator::visitExpressionBinary(const NodeExpressionBinary& binary) {
    visitExpression(*binary.left);
    writeAsm("push rax");

    visitExpression(*binary.right);
    writeAsm("mov rbx, rax");

    writeAsm("pop rax");

    switch (binary.op) {
        case NodeExpressionBinary::BinaryOperator::Add:
            writeAsm("add rax, rbx");
            break;
        case NodeExpressionBinary::BinaryOperator::Subtract:
            writeAsm("sub rax, rbx");
            break;
        case NodeExpressionBinary::BinaryOperator::Multiply:
            writeAsm("imul rax, rbx");
            break;
        case NodeExpressionBinary::BinaryOperator::Divide:
            writeAsm("cqo");
            writeAsm("idiv rbx");
            break;
        default:
            throw std::runtime_error("[VisitorGenerator::visitExpressionBinary] Unknown binary operator");
    }
}

void VisitorGenerator::visitExpressionComparison(const NodeExpressionComparison& comparison) {
    visitExpression(*comparison.left);
    writeAsm("push rax");

    visitExpression(*comparison.right);
    writeAsm("mov rbx, rax");

    writeAsm("pop rax");

    switch (comparison.op) {
        case NodeExpressionComparison::ComparisonOperator::Equal:
            writeAsm("cmp rax, rbx");
            writeAsm("sete al");
            break;
        case NodeExpressionComparison::ComparisonOperator::NotEqual:
            writeAsm("cmp rax, rbx");
            writeAsm("setne al");
            break;
        case NodeExpressionComparison::ComparisonOperator::LessThan:
            writeAsm("cmp rax, rbx");
            writeAsm("setl al");
            break;
        case NodeExpressionComparison::ComparisonOperator::LessThanEqual:
            writeAsm("cmp rax, rbx");
            writeAsm("setle al");
            break;
        case NodeExpressionComparison::ComparisonOperator::GreaterThan:
            writeAsm("cmp rax, rbx");
            writeAsm("setg al");
            break;
        case NodeExpressionComparison::ComparisonOperator::GreaterThanEqual:
            writeAsm("cmp rax, rbx");
            writeAsm("setge al");
            break;
        default:
            throw std::runtime_error("[VisitorGenerator::visitExpressionComparison] Unknown comparison operator");
    }

    // Convert the result to a 64-bit integer
    writeAsm("movzx rax, al");
}

void VisitorGenerator::visitExpressionFunctionCall(const NodeExpressionFunctionCall& funcCall) {
    writeAsm("call " + funcCall.functionName);
}

void VisitorGenerator::writeAsm(const std::string& code) {
    asmOutput += code + "\n";
}