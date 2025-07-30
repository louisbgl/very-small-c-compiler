#include "programPrinter.hpp"
#include <variant>

namespace parser {

// Static methods implementation
void ProgramPrinter::printProgram(const NodeProgram& program) {
    if (!program.main) {
        std::cout << "Program: (empty)" << std::endl;
        return;
    }

    std::cout << "Program:" << std::endl;
    printFunction(*program.main, 1);
}

void ProgramPrinter::printFunction(const NodeFunction& function, int indent) {
    printIndented("Function: " + function.name + " -> int", indent);
    printCompoundStatement(function.body, indent + 1);
}

void ProgramPrinter::printCompoundStatement(const NodeCompoundStatement& compound, int indent) {
    if (compound.statements.empty()) {
        printIndented("Compound Statement: (empty)", indent);
    } else {
        printIndented("Compound Statement:", indent);
        for (const auto& statement : compound.statements) {
            printStatement(statement, indent + 1);
        }
    }
}

void ProgramPrinter::printStatement(const NodeStatement& statement, int indent) {
    std::visit([indent](const auto& stmt) {
        using T = std::decay_t<decltype(stmt)>;
        if constexpr (std::is_same_v<T, NodeStatementEmpty>) {
            ProgramPrinter::printStatementEmpty(stmt, indent);
        }
        else if constexpr (std::is_same_v<T, NodeStatementReturn>) {
            ProgramPrinter::printStatementReturn(stmt, indent);
        }
        else if constexpr (std::is_same_v<T, NodeStatementVarDecl>) {
            ProgramPrinter::printStatementVarDecl(stmt, indent);
        }
        else if constexpr (std::is_same_v<T, NodeStatementAssignment>) {
            ProgramPrinter::printStatementAssignment(stmt, indent);
        }
        else if constexpr (std::is_same_v<T, NodeStatementIf>) {
            ProgramPrinter::printStatementIf(stmt, indent);
        }
        else {
            ProgramPrinter::printIndented("??? Unknown statement", indent);
        }
    }, statement.value);
}

void ProgramPrinter::printExpression(const NodeExpression& expression, int indent) {
    std::visit([indent](const auto& expr) {
        using T = std::decay_t<decltype(expr)>;
        if constexpr (std::is_same_v<T, NodeExpressionPrimary>) {
            ProgramPrinter::printExpressionPrimary(expr, indent);
        }
        else if constexpr (std::is_same_v<T, NodeExpressionBinary>) {
            ProgramPrinter::printExpressionBinary(expr, indent);
        }
    }, expression.value);
}

void ProgramPrinter::printExpressionPrimary(const NodeExpressionPrimary& primary, int indent) {
    std::visit([indent](const auto& value) {
        using ValueType = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<ValueType, int>) {
            ProgramPrinter::printIndented("Literal: " + std::to_string(value), indent);
        } else if constexpr (std::is_same_v<ValueType, std::string>) {
            ProgramPrinter::printIndented("Variable: " + value, indent);
        } else if constexpr (std::is_same_v<ValueType, std::unique_ptr<NodeExpression>>) {
            ProgramPrinter::printIndented("Grouped Expression:", indent);
            ProgramPrinter::printExpression(*value, indent + 1);
        }
    }, primary.value);
}

void ProgramPrinter::printExpressionBinary(const NodeExpressionBinary& binary, int indent) {
    std::string opName;
    switch (binary.op) {
        case NodeExpressionBinary::BinaryOperator::Add: opName = "Addition"; break;
        case NodeExpressionBinary::BinaryOperator::Subtract: opName = "Subtraction"; break;
        case NodeExpressionBinary::BinaryOperator::Multiply: opName = "Multiplication"; break;
        case NodeExpressionBinary::BinaryOperator::Divide: opName = "Division"; break;
    }
    
    printIndented("Binary Expression: " + opName, indent);
    printIndented("left:", indent + 1);
    if (binary.left) {
        printExpression(*binary.left, indent + 2);
    } else {
        printIndented("(null)", indent + 2);
    }
    printIndented("right:", indent + 1);
    if (binary.right) {
        printExpression(*binary.right, indent + 2);
    } else {
        printIndented("(null)", indent + 2);
    }
}

void ProgramPrinter::printStatementEmpty([[maybe_unused]] const NodeStatementEmpty& empty, int indent) {
    printIndented("Empty Statement", indent);
}

void ProgramPrinter::printStatementReturn(const NodeStatementReturn& returnStmt, int indent) {
    if (returnStmt.expression.has_value()) {
        printIndented("Return Statement:", indent);
        printExpression(returnStmt.expression.value(), indent + 1);
    } else {
        printIndented("Return Statement: (void)", indent);
    }
}

void ProgramPrinter::printStatementVarDecl(const NodeStatementVarDecl& varDecl, int indent) {
    if (varDecl.initializer.has_value()) {
        printIndented("Variable Declaration: int " + varDecl.identifier + " =", indent);
        printExpression(varDecl.initializer.value(), indent + 1);
    } else {
        printIndented("Variable Declaration: int " + varDecl.identifier, indent);
    }
}

void ProgramPrinter::printStatementAssignment(const NodeStatementAssignment& assignment, int indent) {
    printIndented("Assignment: " + assignment.identifier + " =", indent);
    printExpression(assignment.expression, indent + 1);
}

void ProgramPrinter::printStatementIf(const NodeStatementIf& ifStmt, int indent) {
    printIndented("If Statement:", indent);
    printIndented("condition:", indent + 1);
    printExpression(ifStmt.condition, indent + 2);
    printIndented("body:", indent + 1);
    if (ifStmt.body) {
        printCompoundStatement(*ifStmt.body, indent + 2);
    } else {
        printIndented("(null)", indent + 2);
    }
}

// Instance methods implementation (delegate to static methods)
void ProgramPrinter::print(const NodeProgram& program) const {
    printProgram(program);
}

void ProgramPrinter::print(const NodeFunction& function) const {
    printFunction(function, 0);
}

void ProgramPrinter::print(const NodeCompoundStatement& compound) const {
    printCompoundStatement(compound, 0);
}

void ProgramPrinter::print(const NodeStatement& statement) const {
    printStatement(statement, 0);
}

void ProgramPrinter::print(const NodeExpression& expression) const {
    printExpression(expression, 0);
}

// Helper method implementation
void ProgramPrinter::printIndented(const std::string& text, int indentLevel) {
    for (int i = 0; i < indentLevel; ++i) {
        std::cout << "  ";
    }
    std::cout << text << std::endl;
}

} // namespace parser
