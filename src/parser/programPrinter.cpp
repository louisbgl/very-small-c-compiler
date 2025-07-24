#include "programPrinter.hpp"
#include <variant>

namespace parser {

// Static methods implementation
void ProgramPrinter::printProgram(const NodeProgram& program) {
    if (!program.main) {
        std::cout << "No main function defined." << std::endl;
        return;
    }

    std::cout << "Program with main function:" << std::endl;
    printFunction(*program.main);
}

void ProgramPrinter::printFunction(const NodeFunction& function) {
    std::cout << "Function: " << function.name << std::endl;
    std::cout << "Type: Int" << std::endl;
    printCompoundStatement(function.body);
}

void ProgramPrinter::printCompoundStatement(const NodeCompoundStatement& compound) {
    std::cout << "Compound Statement with " << compound.statements.size() << " statements:" << std::endl;
    for (const auto& statement : compound.statements) {
        printStatement(statement);
    }
}

void ProgramPrinter::printStatement(const NodeStatement& statement) {
    switch (statement.type) {
        case NodeStatement::StatementType::Empty:
            std::cout << "  Empty statement." << std::endl;
            break;
        case NodeStatement::StatementType::Return:
            std::cout << "  Return statement with expression: ";
            if (statement.expression) {
                printExpression(*statement.expression);
            } else {
                std::cout << "No expression." << std::endl;
            }
            break;
        default:
            std::cout << "  Unknown statement type." << std::endl;
            break;
    }
}

void ProgramPrinter::printExpression(const NodeExpression& expression) {
    std::visit([](const auto& expr) {
        using T = std::decay_t<decltype(expr)>;
        if constexpr (std::is_same_v<T, NodeExpressionPrimary>) {
            ProgramPrinter::printExpressionPrimary(expr);
        }
        else if constexpr (std::is_same_v<T, NodeExpressionBinary>) {
            ProgramPrinter::printExpressionBinary(expr);
        }
    }, expression.value);
}

void ProgramPrinter::printExpressionPrimary(const NodeExpressionPrimary& primary) {
    std::visit([](const auto& value) {
        using ValueType = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<ValueType, int>) {
            std::cout << "Integer Literal: " << value << std::endl;
        } else if constexpr (std::is_same_v<ValueType, std::unique_ptr<NodeExpression>>) {
            std::cout << "Grouped Expression: " << std::endl;
            ProgramPrinter::printExpression(*value);
        }
    }, primary.value);
}

void ProgramPrinter::printExpressionBinary(const NodeExpressionBinary& binary) {
    std::cout << "Binary Expression: ";
    switch (binary.op) {
        case NodeExpressionBinary::BinaryOperator::Add:
            std::cout << "Add" << std::endl;
            break;
        case NodeExpressionBinary::BinaryOperator::Subtract:
            std::cout << "Subtract" << std::endl;
            break;
    }
    std::cout << "  Left: ";
    if (binary.left) {
        ProgramPrinter::printExpression(*binary.left);
    } else {
        std::cout << "null" << std::endl;
    }
    std::cout << "  Right: ";
    if (binary.right) {
        ProgramPrinter::printExpression(*binary.right);
    } else {
        std::cout << "null" << std::endl;
    }
}

// Instance methods implementation (delegate to static methods)
void ProgramPrinter::print(const NodeProgram& program) const {
    printProgram(program);
}

void ProgramPrinter::print(const NodeFunction& function) const {
    printFunction(function);
}

void ProgramPrinter::print(const NodeCompoundStatement& compound) const {
    printCompoundStatement(compound);
}

void ProgramPrinter::print(const NodeStatement& statement) const {
    printStatement(statement);
}

void ProgramPrinter::print(const NodeExpression& expression) const {
    printExpression(expression);
}

// Helper method implementation
void ProgramPrinter::printIndented(const std::string& text, int indentLevel) {
    for (int i = 0; i < indentLevel; ++i) {
        std::cout << "  ";
    }
    std::cout << text << std::endl;
}

} // namespace parser
