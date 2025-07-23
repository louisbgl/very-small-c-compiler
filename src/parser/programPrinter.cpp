#include "programPrinter.hpp"

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
    switch (expression.type) {
        case NodeExpression::ExpressionType::IntegerLiteral:
            std::cout << "Integer Literal: " << expression.intValue << std::endl;
            break;
        default:
            std::cout << "Unknown expression type." << std::endl;
            break;
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
