#pragma once

#include <iostream>
#include "nodes.hpp"

namespace parser {

class ProgramPrinter {
public:
    // Static interface for easy usage
    static void printProgram(const NodeProgram& program);
    static void printFunction(const NodeFunction& function);
    static void printCompoundStatement(const NodeCompoundStatement& compound);
    static void printStatement(const NodeStatement& statement);
    static void printExpression(const NodeExpression& expression);
    
    // Helper functions for specific node types
    static void printExpressionPrimary(const NodeExpressionPrimary& primary);
    static void printExpressionBinary(const NodeExpressionBinary& binary);
    static void printStatementEmpty(const NodeStatementEmpty& empty);
    static void printStatementReturn(const NodeStatementReturn& returnStmt);
    static void printStatementVarDecl(const NodeStatementVarDecl& varDecl);
    static void printStatementAssignment(const NodeStatementAssignment& assignment);

    // Instance interface (if needed for future extensibility)
    ProgramPrinter() = default;
    
    void print(const NodeProgram& program) const;
    void print(const NodeFunction& function) const;
    void print(const NodeCompoundStatement& compound) const;
    void print(const NodeStatement& statement) const;
    void print(const NodeExpression& expression) const;

private:
    // Helper methods for consistent formatting
    static void printIndented(const std::string& text, int indentLevel = 0);
};

} // namespace parser