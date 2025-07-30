#pragma once

#include <iostream>
#include "nodes.hpp"

namespace parser {

class ProgramPrinter {
public:
    // Static interface for easy usage
    static void printProgram(const NodeProgram& program);
    static void printFunction(const NodeFunction& function, int indent = 0);
    static void printCompoundStatement(const NodeCompoundStatement& compound, int indent = 0);
    static void printStatement(const NodeStatement& statement, int indent = 0);
    static void printExpression(const NodeExpression& expression, int indent = 0);
    
    // Helper functions for specific node types
    static void printExpressionPrimary(const NodeExpressionPrimary& primary, int indent = 0);
    static void printExpressionBinary(const NodeExpressionBinary& binary, int indent = 0);
    static void printStatementEmpty(const NodeStatementEmpty& empty, int indent = 0);
    static void printStatementReturn(const NodeStatementReturn& returnStmt, int indent = 0);
    static void printStatementVarDecl(const NodeStatementVarDecl& varDecl, int indent = 0);
    static void printStatementAssignment(const NodeStatementAssignment& assignment, int indent = 0);
    static void printStatementIf(const NodeStatementIf& ifStmt, int indent = 0);

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