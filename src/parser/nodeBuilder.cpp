#include "nodeBuilder.hpp"

namespace parser {

// Expression builders
NodeExpression NodeBuilder::createPrimaryExpression(int value) {
    NodeExpressionPrimary primary;
    primary.intValue = value;
    NodeExpression expr;
    expr.value = primary;
    return expr;
}

NodeExpression NodeBuilder::createBinaryExpression(NodeExpressionBinary::BinaryOperator op,
                                                  std::unique_ptr<NodeExpression> left,
                                                  std::unique_ptr<NodeExpression> right) {
    NodeExpressionBinary binary;
    binary.op = op;
    binary.left = std::move(left);
    binary.right = std::move(right);
    NodeExpression expr;
    expr.value = std::move(binary);
    return expr;
}

std::unique_ptr<NodeExpression> NodeBuilder::makePrimaryExpression(int value) {
    auto expr = std::make_unique<NodeExpression>();
    *expr = createPrimaryExpression(value);
    return expr;
}

std::unique_ptr<NodeExpression> NodeBuilder::makeBinaryExpression(NodeExpressionBinary::BinaryOperator op,
                                                                 std::unique_ptr<NodeExpression> left,
                                                                 std::unique_ptr<NodeExpression> right) {
    auto expr = std::make_unique<NodeExpression>();
    *expr = createBinaryExpression(op, std::move(left), std::move(right));
    return expr;
}

// Statement builders
NodeStatement NodeBuilder::createEmptyStatement() {
    NodeStatement statement;
    statement.type = NodeStatement::StatementType::Empty;
    return statement;
}

NodeStatement NodeBuilder::createReturnStatement(NodeExpression expression) {
    NodeStatement statement;
    statement.type = NodeStatement::StatementType::Return;
    statement.expression = std::move(expression);
    return statement;
}

NodeStatement NodeBuilder::createReturnStatement(std::optional<NodeExpression> expression) {
    NodeStatement statement;
    statement.type = NodeStatement::StatementType::Return;
    statement.expression = std::move(expression);
    return statement;
}

// Compound statement builders
NodeCompoundStatement NodeBuilder::createCompoundStatement() {
    return NodeCompoundStatement{};
}

NodeCompoundStatement NodeBuilder::createCompoundStatement(std::vector<NodeStatement> statements) {
    NodeCompoundStatement compound;
    compound.statements = std::move(statements);
    return compound;
}

// Function builders
NodeFunction NodeBuilder::createFunction(NodeFunction::FunctionType type, 
                                       const std::string& name, 
                                       NodeCompoundStatement body) {
    NodeFunction function;
    function.type = type;
    function.name = name;
    function.body = std::move(body);
    return function;
}

// Program builders
std::unique_ptr<NodeProgram> NodeBuilder::createProgram(std::unique_ptr<NodeFunction> mainFunction) {
    auto program = std::make_unique<NodeProgram>();
    program->main = std::move(mainFunction);
    return program;
}

std::unique_ptr<NodeProgram> NodeBuilder::createProgram(NodeFunction mainFunction) {
    auto program = std::make_unique<NodeProgram>();
    program->main = std::make_unique<NodeFunction>(std::move(mainFunction));
    return program;
}

} // namespace parser
