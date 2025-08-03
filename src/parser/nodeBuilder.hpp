#pragma once

#include "nodes.hpp"
#include <memory>
#include <vector>
#include <optional>

namespace parser {

class NodeBuilder {
public:
    // Expression builders
    static NodeExpression createPrimaryExpression(int value);
    static NodeExpression createPrimaryExpression(const std::string& identifier);
    static NodeExpression createPrimaryExpression(NodeExpression expression);
    static NodeExpression createBinaryExpression(NodeExpressionBinary::BinaryOperator op,
                                                std::unique_ptr<NodeExpression> left,
                                                std::unique_ptr<NodeExpression> right);
    static NodeExpression createComparisonExpression(NodeExpressionComparison::ComparisonOperator op,
                                                    std::unique_ptr<NodeExpression> left,
                                                    std::unique_ptr<NodeExpression> right);
    static NodeExpression createFunctionCallExpression(const std::string& functionName);

    // Statement builders
    static NodeStatement createEmptyStatement();
    static NodeStatement createReturnStatement(std::optional<NodeExpression> expression = std::nullopt);
    static NodeStatement createVariableDeclaration(const std::string& identifier, std::optional<NodeExpression> initializer = std::nullopt);
    static NodeStatement createAssignment(const std::string& identifier, NodeExpression expression);
    static NodeStatement createIfStatement(NodeExpression condition, std::unique_ptr<NodeCompoundStatement> body, std::optional<std::unique_ptr<NodeCompoundStatement>> elseBody = std::nullopt);
    static NodeStatement createWhileStatement(NodeExpression condition, std::unique_ptr<NodeCompoundStatement> body);
    
    // Compound statement builders
    static NodeCompoundStatement createCompoundStatement();
    static NodeCompoundStatement createCompoundStatement(std::vector<NodeStatement> statements);
    
    // Function builders
    static NodeFunction createFunction(NodeFunction::FunctionType type, 
                                     const std::string& name, 
                                     NodeCompoundStatement body);
    
    // Program builders
    static std::unique_ptr<NodeProgram> createProgram(std::vector<NodeFunction> functions);
};

} // namespace parser
