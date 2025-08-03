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
    std::variant<int, std::string, std::unique_ptr<NodeExpression>> value;
};

struct NodeExpressionBinary {
    enum class BinaryOperator { Add, Subtract, Multiply, Divide };
    BinaryOperator op;
    std::unique_ptr<NodeExpression> left;
    std::unique_ptr<NodeExpression> right;
};

struct NodeExpressionComparison {
    enum class ComparisonOperator {
        Equal, NotEqual,
        LessThan, LessThanEqual,
        GreaterThan, GreaterThanEqual
    };
    ComparisonOperator op;
    std::unique_ptr<NodeExpression> left;
    std::unique_ptr<NodeExpression> right;
};

struct NodeExpression {
    std::variant<NodeExpressionPrimary, NodeExpressionBinary, NodeExpressionComparison> value;
};

struct NodeStatementEmpty { };

struct NodeStatementReturn {
    std::optional<NodeExpression> expression;
};

struct NodeStatementVarDecl {
    std::string identifier;
    std::optional<NodeExpression> initializer;
};

struct NodeStatementAssignment {
    std::string identifier;
    NodeExpression expression;
};

struct NodeStatementIf {
    NodeExpression condition;
    std::unique_ptr<NodeCompoundStatement> body;
    std::optional<std::unique_ptr<NodeCompoundStatement>> elseBody;
};

struct NodeStatement {
    std::variant<NodeStatementEmpty,
                 NodeStatementReturn,
                 NodeStatementVarDecl,
                 NodeStatementAssignment,
                 NodeStatementIf> value;
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
