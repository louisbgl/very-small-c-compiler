#pragma once

#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <variant>

/* Forward declarations */

struct NodeProgram;
struct NodeFunction;
struct NodeCompoundStatement;
struct NodeStatement;
struct NodeExpression;

struct NodeExpressionPrimary {
    std::variant<int, std::unique_ptr<NodeExpression>> value;
};

struct NodeExpressionBinary {
    enum class BinaryOperator { Add, Subtract, Multiply, Divide };
    BinaryOperator op;
    std::unique_ptr<NodeExpression> left;
    std::unique_ptr<NodeExpression> right;
};

struct NodeExpression {
    std::variant<NodeExpressionPrimary, NodeExpressionBinary> value;
};

struct NodeStatement {
    enum class StatementType { Empty, Return };
    StatementType type;
    std::optional<NodeExpression> expression;
};

struct NodeCompoundStatement {
    std::vector<NodeStatement> statements;
};

struct NodeFunction {
    enum class FunctionType { Int };
    FunctionType type;
    std::string name;
    NodeCompoundStatement body;
};

struct NodeProgram {
    std::unique_ptr<NodeFunction> main;
};
