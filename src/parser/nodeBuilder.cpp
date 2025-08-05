#include "nodeBuilder.hpp"

namespace parser {

// Expression builders
NodeExpression NodeBuilder::createPrimaryExpression(int value) {
    NodeExpressionPrimary primary;
    primary.value = value;
    return NodeExpression{std::move(primary)};
}

NodeExpression NodeBuilder::createPrimaryExpression(const std::string& identifier) {
    NodeExpressionPrimary primary;
    primary.value = identifier;
    return NodeExpression{std::move(primary)};
}

NodeExpression NodeBuilder::createPrimaryExpression(NodeExpression expression) {
    NodeExpressionPrimary primary;
    primary.value = std::make_unique<NodeExpression>(std::move(expression));
    return NodeExpression{std::move(primary)};
}

NodeExpression NodeBuilder::createBinaryExpression(NodeExpressionBinary::BinaryOperator op,
                                                  std::unique_ptr<NodeExpression> left,
                                                  std::unique_ptr<NodeExpression> right) {
    NodeExpressionBinary binary;
    binary.op = op;
    binary.left = std::move(left);
    binary.right = std::move(right);
    return NodeExpression{std::move(binary)};
}

NodeExpression NodeBuilder::createComparisonExpression(NodeExpressionComparison::ComparisonOperator op,
                                                        std::unique_ptr<NodeExpression> left,
                                                        std::unique_ptr<NodeExpression> right) {
    NodeExpressionComparison comparison;
    comparison.op = op;
    comparison.left = std::move(left);
    comparison.right = std::move(right);
    return NodeExpression{std::move(comparison)};
}

NodeExpression NodeBuilder::createFunctionCallExpression(const std::string& functionName, 
                                                        std::vector<NodeExpression> arguments) {
    NodeExpressionFunctionCall call;
    call.functionName = functionName;
    call.arguments = std::move(arguments);
    return NodeExpression{std::move(call)};
}

// Statement builders
NodeStatement NodeBuilder::createEmptyStatement() {
    NodeStatement statement;
    statement.value = NodeStatementEmpty{};
    return statement;
}

NodeStatement NodeBuilder::createReturnStatement(std::optional<NodeExpression> expression) {
    NodeStatementReturn returnStmt;
    if (expression.has_value()) {
        returnStmt.expression = std::move(expression.value());
    }
    
    NodeStatement statement;
    statement.value = std::move(returnStmt);
    return statement;
}

NodeStatement NodeBuilder::createVariableDeclaration(const std::string& identifier, 
                                                   std::optional<NodeExpression> initializer) {
    NodeStatementVarDecl varDecl;
    varDecl.identifier = identifier;
    if (initializer.has_value()) {
        varDecl.initializer = std::move(initializer.value());
    }
    
    NodeStatement statement;
    statement.value = std::move(varDecl);
    return statement;
}

NodeStatement NodeBuilder::createAssignment(const std::string& identifier, NodeExpression expression) {
    NodeStatementAssignment assignment;
    assignment.identifier = identifier;
    assignment.expression = std::move(expression);
    
    NodeStatement statement;
    statement.value = std::move(assignment);
    return statement;
}

NodeStatement NodeBuilder::createIfStatement(NodeExpression condition,
                                            std::unique_ptr<NodeCompoundStatement> body,
                                            std::optional<std::unique_ptr<NodeCompoundStatement>> elseBody) {
    NodeStatementIf ifStmt;
    ifStmt.condition = std::move(condition);
    ifStmt.body = std::move(body);
    if (elseBody.has_value()) {
        ifStmt.elseBody = std::move(elseBody.value());
    }

    NodeStatement statement;
    statement.value = std::move(ifStmt);
    return statement;
}

NodeStatement NodeBuilder::createWhileStatement(NodeExpression condition, std::unique_ptr<NodeCompoundStatement> body) {
    NodeStatementWhile whileStmt;
    whileStmt.condition = std::move(condition);
    whileStmt.body = std::move(body);

    NodeStatement statement;
    statement.value = std::move(whileStmt);
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
                                        const std::vector<FunctionParameter>& parameters,
                                        NodeCompoundStatement body) {
    NodeFunction function;
    function.type = type;
    function.name = name;
    function.parameters = parameters;
    function.body = std::move(body);
    return function;
}

// Program builders
std::unique_ptr<NodeProgram> NodeBuilder::createProgram(std::vector<NodeFunction> functions) {
    auto program = std::make_unique<NodeProgram>();
    program->functions = std::move(functions);
    return program;
}

} // namespace parser
