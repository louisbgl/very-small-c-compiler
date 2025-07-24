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
    static NodeExpression createBinaryExpression(NodeExpressionBinary::BinaryOperator op,
                                                std::unique_ptr<NodeExpression> left,
                                                std::unique_ptr<NodeExpression> right);
    static std::unique_ptr<NodeExpression> makePrimaryExpression(int value);
    static std::unique_ptr<NodeExpression> makeBinaryExpression(NodeExpressionBinary::BinaryOperator op,
                                                               std::unique_ptr<NodeExpression> left,
                                                               std::unique_ptr<NodeExpression> right);
    
    // Statement builders
    static NodeStatement createEmptyStatement();
    static NodeStatement createReturnStatement(NodeExpression expression);
    static NodeStatement createReturnStatement(std::optional<NodeExpression> expression = std::nullopt);
    
    // Compound statement builders
    static NodeCompoundStatement createCompoundStatement();
    static NodeCompoundStatement createCompoundStatement(std::vector<NodeStatement> statements);
    
    // Function builders
    static NodeFunction createFunction(NodeFunction::FunctionType type, 
                                     const std::string& name, 
                                     NodeCompoundStatement body);
    
    // Program builders
    static std::unique_ptr<NodeProgram> createProgram(std::unique_ptr<NodeFunction> mainFunction);
    static std::unique_ptr<NodeProgram> createProgram(NodeFunction mainFunction);
};

} // namespace parser
