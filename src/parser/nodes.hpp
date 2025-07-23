#pragma once

#include <string>
#include <memory>
#include <vector>
#include <optional>

/* Forward declarations */

struct NodeProgram;
struct NodeFunction;
struct NodeCompoundStatement;
struct NodeStatement;
struct NodeExpression;

struct NodeExpression {
    enum class ExpressionType { IntegerLiteral };
    ExpressionType type;
    int intValue;
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
