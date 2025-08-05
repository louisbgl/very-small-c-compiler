#pragma once

#include "astVisitor.hpp"
#include "scopeNode.hpp"

class VisitorAnalyzer : public AstVisitor<VisitorAnalyzer> {
public:
    VisitorAnalyzer();
    VisitorAnalyzer(ScopeNode* globalScope);

    ScopeNode* getRootScope() const;
    std::unique_ptr<ScopeNode> releaseRootScope();

    void analyze(const NodeProgram& ast);
    
private:
    inline void assertMainExists(const NodeProgram& ast) const;

    void visitFunction(const NodeFunction& function);
    void visitCompoundStatement(const NodeCompoundStatement& compound);
    void visitStatement(const NodeStatement& statement);
    void visitExpression(const NodeExpression& expression);

    void visitStatementVarDecl(const NodeStatementVarDecl& varDecl);
    void visitStatementIf(const NodeStatementIf& ifStmt);
    void visitStatementWhile(const NodeStatementWhile& whileStmt);
    
    void visitExpressionPrimary(const NodeExpressionPrimary& primary);
    void visitExpressionBinary(const NodeExpressionBinary& binary);
    void visitExpressionComparison(const NodeExpressionComparison& comparison);
    void visitExpressionFunctionCall(const NodeExpressionFunctionCall& funcCall);

    std::unique_ptr<ScopeNode> rootScope;
    ScopeNode* currentScope;
};