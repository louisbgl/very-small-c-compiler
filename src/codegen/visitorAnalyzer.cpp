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

    assertMainExists(ast);

    for (const auto& function : ast.functions) {
        visitFunction(function);
    }
}

void VisitorAnalyzer::assertMainExists(const NodeProgram& ast) const {
    bool mainFound = false;
    for (const auto& function : ast.functions) {
        if (function.name == "main" && function.type == NodeFunction::FunctionType::Int) {
            mainFound = true;
            break;
        }
    }

    if (!mainFound) {
        throw std::runtime_error("[VisitorAnalyzer::assertMainExists] 'main' function not found in program");
    }
}

void VisitorAnalyzer::visitFunction(const NodeFunction& function) {
    currentScope = getRootScope();
    currentScope = currentScope->pushChild();
    for (const auto& param : function.parameters) {
        currentScope->addVariable(param.name, Type::Int, 8); // Assuming all parameters are int
    }
    visitCompoundStatement(function.body);
    currentScope = currentScope->getParent();
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
        } else if constexpr (std::is_same_v<T, NodeStatementWhile>) {
            visitStatementWhile(stmt);
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
        } else if constexpr (std::is_same_v<T, NodeExpressionFunctionCall>) {
            visitExpressionFunctionCall(expr);
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

void VisitorAnalyzer::visitStatementWhile(const NodeStatementWhile& whileStmt) {
    visitExpression(whileStmt.condition);
    if (whileStmt.body) {
        visitCompoundStatement(*whileStmt.body);
    } else {
        throw std::runtime_error("[VisitorAnalyzer::visitStatementWhile] While statement body is null");
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

void VisitorAnalyzer::visitExpressionFunctionCall(const NodeExpressionFunctionCall& funcCall) {
    // Basic validation for function calls
    if (funcCall.arguments.size() > 6) {
        throw std::runtime_error("[VisitorAnalyzer::visitExpression] Function call has too many arguments (max 6)");
    }
    // Visit arguments for validation
    for (const auto& arg : funcCall.arguments) {
        visitExpression(arg);
    }
}
