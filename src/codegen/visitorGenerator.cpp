#include "visitorGenerator.hpp"

VisitorGenerator::VisitorGenerator(ScopeNode* rootScope)
    : asmOutput(""), currentScope(rootScope), childScopeIndex(0) {}

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
    
    visitFunction(*ast.main);
    return asmOutput;
}

void VisitorGenerator::visitFunction(const NodeFunction& function) {
    writeAsm(".globl " + function.name);
    writeAsm(function.name + ":");
    writeAsm("push rbp");
    writeAsm("mov rbp, rsp");
    
    visitCompoundStatement(function.body);
}

void VisitorGenerator::visitCompoundStatement(const NodeCompoundStatement& compound) {
    ScopeNode* originalScope = currentScope;
    
    try {
        currentScope = &currentScope->getChild(0);
    } catch (const std::out_of_range&) {
        throw std::runtime_error("Generator scope traversal mismatch - no child scope found for compound statement");
    }

    int frameSize = currentScope->getFrameSize();
    if (frameSize > 0) {
        writeAsm("sub rsp, " + std::to_string(frameSize));
    }

    for (const auto& statement : compound.statements) {
        visitStatement(statement);
    }

    currentScope = originalScope;
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
        } else {
            throw std::runtime_error("[VisitorAnaliser::visitStatement] Unknown statement type");
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
        } else {
            throw std::runtime_error("[VisitorGenerator::visitExpression] Unknown expression type");
        }
    }, expression.value);
}

void VisitorGenerator::visitStatementVarDecl(const NodeStatementVarDecl& varDecl) {
    auto offsetOpt = currentScope->getOffset(varDecl.identifier);
    if (!offsetOpt.has_value()) {
        throw std::runtime_error("Variable '" + varDecl.identifier + "' not found in scope");
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
    writeAsm("ret");
}

void VisitorGenerator::visitStatementAssignment(const NodeStatementAssignment& assignment) {
    auto offsetOpt = currentScope->getOffset(assignment.identifier);
    if (!offsetOpt.has_value()) {
        throw std::runtime_error("Variable '" + assignment.identifier + "' not found in scope");
    }
    int offset = offsetOpt.value();

    visitExpression(assignment.expression);

    writeAsm("mov [rbp - " + std::to_string(offset) + "], rax");
}

void VisitorGenerator::visitExpressionPrimary(const NodeExpressionPrimary& primary) {
    std::visit([this](const auto& value) {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, int>) {
            writeAsm("mov rax, " + std::to_string(value));
        } else if constexpr (std::is_same_v<T, std::string>) {
            auto offsetOpt = currentScope->getOffsetRecursive(value);
            if (!offsetOpt.has_value()) {
                throw std::runtime_error("Unknown identifier: " + value);
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

void VisitorGenerator::writeAsm(const std::string& code) {
    asmOutput += code + "\n";
}