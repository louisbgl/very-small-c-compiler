#pragma once

#include "../parser/nodes.hpp"

// CRTP-style AST visitor base class
template <typename Derived>
class AstVisitor {
public:
    void visitFunction(const NodeFunction& function) {
        static_cast<Derived*>(this)->visitFunction(function);
    }

    void visitCompoundStatement(const NodeCompoundStatement& compound) {
        static_cast<Derived*>(this)->visitCompoundStatement(compound);
    }

    void visitStatement(const NodeStatement& statement) {
        static_cast<Derived*>(this)->visitStatement(statement);
    }

    void visitExpression(const NodeExpression& expression) {
        static_cast<Derived*>(this)->visitExpression(expression);
    }
};