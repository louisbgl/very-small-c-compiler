#pragma once

#include "astVisitor.hpp"
#include "scopeNode.hpp"

class VisitorGenerator : public AstVisitor<VisitorGenerator> {
public:
    VisitorGenerator(ScopeNode* rootScope);

    std::string generate(const NodeProgram& ast);
    
private:
    std::string asmOutput;
    ScopeNode* currentScope;
    std::vector<int> childScopeIndexes; // Acts like a stack of indexes for child scopes
    int labelCounter;
    
    void visitFunction(const NodeFunction& function);
    void visitCompoundStatement(const NodeCompoundStatement& compound);
    void visitStatement(const NodeStatement& statement);
    void visitExpression(const NodeExpression& expression);

    void visitStatementVarDecl(const NodeStatementVarDecl& varDecl);
    void visitStatementReturn(const NodeStatementReturn& returnStmt);
    void visitStatementAssignment(const NodeStatementAssignment& assignment);
    void visitStatementIf(const NodeStatementIf& ifStmt);
    void visitStatementWhile(const NodeStatementWhile& whileStmt);

    void visitExpressionPrimary(const NodeExpressionPrimary& primary);
    void visitExpressionBinary(const NodeExpressionBinary& binary);
    void visitExpressionComparison(const NodeExpressionComparison& comparison);

    void writeAsm(const std::string& code);
};
