#include "visitorAnalyzer.hpp"

VisitorAnalyzer::VisitorAnalyzer() 
    : rootScope(std::make_unique<ScopeNode>(nullptr)), currentScope(rootScope.get()) {}

VisitorAnalyzer::VisitorAnalyzer(ScopeNode* globalScope) 
    : rootScope(std::make_unique<ScopeNode>(globalScope)), currentScope(rootScope.get()) {}

ScopeNode* VisitorAnalyzer::getRootScope() const {
    return rootScope.get();
}

std::unique_ptr<ScopeNode> VisitorAnalyzer::releaseRootScope() {
    return std::move(rootScope);
}

void VisitorAnalyzer::analyze(const NodeProgram& ast) {
    if (!rootScope) {
        throw std::runtime_error("[VisitorAnalyzer::analyze] Root scope is not initialized");
    }

    visitFunction(*ast.main);
}

void VisitorAnalyzer::visitFunction(const NodeFunction& function) {
    visitCompoundStatement(function.body);
}

void VisitorAnalyzer::visitCompoundStatement(const NodeCompoundStatement& compound) {
    currentScope = currentScope->pushChild();

    for (const auto& statement : compound.statements) {
        visitStatement(statement);
    }

    currentScope = currentScope->getParent();
}

void VisitorAnalyzer::visitStatement(const NodeStatement& statement) {
    std::visit([this](const auto& stmt) {
        using T = std::decay_t<decltype(stmt)>;
        if constexpr (std::is_same_v<T, NodeStatementEmpty>) {
            // IGNORE empty statements while analyzing
        } else if constexpr (std::is_same_v<T, NodeStatementReturn>) {
            // OPTIONAL type checking for return statements
        } else if constexpr (std::is_same_v<T, NodeStatementVarDecl>) {
            visitStatementVarDecl(stmt);
        } else if constexpr (std::is_same_v<T, NodeStatementAssignment>) {
            // OPTIONAL type checking for assignments
        } else if constexpr (std::is_same_v<T, NodeStatementIf>) {
            visitStatementIf(stmt);
        } else {
            throw std::runtime_error("[VisitorAnalyzer::visitStatement] Unknown statement type");
        }
    }, statement.value);
}

void VisitorAnalyzer::visitExpression(const NodeExpression& expression) {
    std::visit([this](const auto& expr) {
        using T = std::decay_t<decltype(expr)>;
        if constexpr (std::is_same_v<T, NodeExpressionPrimary>) {
            visitExpressionPrimary(expr);
        } else if constexpr (std::is_same_v<T, NodeExpressionBinary>) {
            visitExpressionBinary(expr);
        } else if constexpr (std::is_same_v<T, NodeExpressionComparison>) {
            visitExpressionComparison(expr);
        } else {
            throw std::runtime_error("[VisitorAnalyzer::visitExpression] Unknown expression type");
        }
    }, expression.value);
}

void VisitorAnalyzer::visitStatementVarDecl(const NodeStatementVarDecl& varDecl) {
    const std::string& name = varDecl.identifier;
    Type type = Type::Int;

    int size = 8; // Use 64-bit integers

    if (currentScope->getOffset(name)) {
        throw std::runtime_error("[VisitorAnalyzer::visitStatementVarDecl] Variable '" + name + "' already declared in this scope");
    }

    currentScope->addVariable(name, type, size);

    if (varDecl.initializer) {
        visitExpression(*varDecl.initializer);
    }
}

void VisitorAnalyzer::visitStatementIf(const NodeStatementIf& ifStmt) {
    visitExpression(ifStmt.condition);
    if (ifStmt.body) {
        visitCompoundStatement(*ifStmt.body);
    } else {
        throw std::runtime_error("[VisitorAnalyzer::visitStatementIf] If statement body is null");
    }

    if (ifStmt.elseBody.has_value()) {
        visitCompoundStatement(*ifStmt.elseBody.value());
    }
}

void VisitorAnalyzer::visitExpressionPrimary(const NodeExpressionPrimary& primary) {
    if (std::holds_alternative<std::string>(primary.value)) {
        const std::string& varName = std::get<std::string>(primary.value);

        if (!currentScope->getOffsetRecursive(varName)) {
            throw std::runtime_error("[VisitorAnalyzer::visitExpressionPrimary] Use of undeclared variable '" + varName + "'");
        }
    }
}

void VisitorAnalyzer::visitExpressionBinary(const NodeExpressionBinary& binary) {
    visitExpression(*binary.left);
    visitExpression(*binary.right);
}

void VisitorAnalyzer::visitExpressionComparison(const NodeExpressionComparison& comparison) {
    visitExpression(*comparison.left);
    visitExpression(*comparison.right);

    // Optional: Check types of left and right expressions if needed
    // For now, we assume they are compatible for comparison
}