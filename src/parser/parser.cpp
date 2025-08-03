#include <iostream>
#include "parser.hpp"
#include "programPrinter.hpp"
#include "../lexer/lexer.hpp"

namespace parser {

Parser::Parser(lexer::Lexer& lexer)
    : lexer(lexer), currentToken(lexer.nextToken()) {
    program = parseProgram();
}

const std::unique_ptr<NodeProgram>& Parser::getProgram() const {
    return program;
}

void Parser::printProgram() const {
    ProgramPrinter::printProgram(*program);
}

std::unique_ptr<NodeProgram> Parser::parseProgram() {
    auto mainFunction = parseFunction();
    return NodeBuilder::createProgram(std::move(mainFunction));
}

NodeFunction Parser::parseFunction() {
    expectAndConsumeToken(TokenType::Keyword_int);

    expectToken(TokenType::Identifier);
    std::string functionName = static_cast<std::string>(currentToken.lexeme);
    consumeToken(); // Consume identifier token

    expectAndConsumeToken(TokenType::OpenParen);
    expectAndConsumeToken(TokenType::CloseParen);

    auto body = parseCompoundStatement();
    return NodeBuilder::createFunction(NodeFunction::FunctionType::Int, functionName, std::move(body));
}

NodeCompoundStatement Parser::parseCompoundStatement() {
    expectAndConsumeToken(TokenType::OpenBrace);

    NodeCompoundStatement compoundStatement;
    while (currentToken.type != TokenType::EndOfFile &&
           currentToken.type != TokenType::CloseBrace) {
        compoundStatement.statements.push_back(parseStatement());
    }

    expectAndConsumeToken(TokenType::CloseBrace);
    return compoundStatement;
}

NodeStatement Parser::parseStatement() {
    if (currentToken.type == TokenType::Semicolon) {
        return NodeBuilder::createEmptyStatement();
    }
    else if (currentToken.type == TokenType::Keyword_return) {
        return parseReturnStatement();
    }
    else if (currentToken.type == TokenType::Keyword_int) {
        return parseVariableDeclaration();
    }
    else if (currentToken.type == TokenType::Keyword_if) {
        return parseIfStatement();
    }
    else if (currentToken.type == TokenType::Identifier) {
        return parseAssignmentStatement();
    }
    else {
        throw std::runtime_error("[Parser::parseStatement] Unexpected token: " + currentToken.ToString());
    }
}

NodeStatement Parser::parseReturnStatement() {
    expectAndConsumeToken(TokenType::Keyword_return);

    auto expression = parseExpression();
    expectAndConsumeToken(TokenType::Semicolon);
    
    return NodeBuilder::createReturnStatement(std::move(expression));
}

NodeStatement Parser::parseVariableDeclaration() {
    expectAndConsumeToken(TokenType::Keyword_int);

    expectToken(TokenType::Identifier);
    std::string identifier = static_cast<std::string>(currentToken.lexeme);
    consumeToken(); // Consume identifier token

    std::optional<NodeExpression> initializer;
    if (currentToken.type == TokenType::Equal) {
        consumeToken(); // Consume '='
        initializer = parseExpression();
    }

    expectAndConsumeToken(TokenType::Semicolon);
    
    return NodeBuilder::createVariableDeclaration(identifier, std::move(initializer));
}

NodeStatement Parser::parseAssignmentStatement() {
    expectToken(TokenType::Identifier);
    std::string identifier = static_cast<std::string>(currentToken.lexeme);
    consumeToken(); // Consume identifier token

    expectAndConsumeToken(TokenType::Equal);
    auto expression = parseExpression();
    
    expectAndConsumeToken(TokenType::Semicolon);
    
    return NodeBuilder::createAssignment(identifier, std::move(expression));
}

NodeStatement Parser::parseIfStatement() {
    expectAndConsumeToken(TokenType::Keyword_if);
    expectAndConsumeToken(TokenType::OpenParen);

    auto condition = parseExpression();
    expectAndConsumeToken(TokenType::CloseParen);

    auto body = parseCompoundStatement();

    if (currentToken.type == TokenType::Keyword_else) {
        consumeToken(); // Consume 'else'
        auto elseBody = parseCompoundStatement();
        return NodeBuilder::createIfStatement(std::move(condition), std::make_unique<NodeCompoundStatement>(std::move(body)), std::make_unique<NodeCompoundStatement>(std::move(elseBody)));
    }
    
    return NodeBuilder::createIfStatement(std::move(condition), std::make_unique<NodeCompoundStatement>(std::move(body)));
}

NodeExpression Parser::parseExpression() {
    return parseAddSubExpression();
}

NodeExpression Parser::parseAddSubExpression() {
    auto left = parseMultDivExpression();

    while (isAddSubBinaryOperator(currentToken)) {
        auto op = getBinaryOperator(currentToken);
        consumeToken(); // Consume the operator
        
        auto right = parsePrimaryExpression();
        
        // Create binary expression with current left and right
        auto leftPtr = std::make_unique<NodeExpression>(std::move(left));
        auto rightPtr = std::make_unique<NodeExpression>(std::move(right));
        left = NodeBuilder::createBinaryExpression(op, std::move(leftPtr), std::move(rightPtr));
    }
    
    return left;
}

NodeExpression Parser::parseMultDivExpression() {
    auto left = parsePrimaryExpression();

    while (isMultDivBinaryOperator(currentToken)) {
        auto op = getBinaryOperator(currentToken);
        consumeToken(); // Consume the operator
        
        auto right = parsePrimaryExpression();
        
        // Create binary expression with current left and right
        auto leftPtr = std::make_unique<NodeExpression>(std::move(left));
        auto rightPtr = std::make_unique<NodeExpression>(std::move(right));
        left = NodeBuilder::createBinaryExpression(op, std::move(leftPtr), std::move(rightPtr));
    }
    
    return left;
}

NodeExpression Parser::parsePrimaryExpression() {
    if (currentToken.type == TokenType::Number) {
        int value = std::stoi(std::string(currentToken.lexeme));
        expectAndConsumeToken(TokenType::Number);
        return NodeBuilder::createPrimaryExpression(value);
    }
    else if (currentToken.type == TokenType::Identifier) {
        std::string identifier = static_cast<std::string>(currentToken.lexeme);
        expectAndConsumeToken(TokenType::Identifier);
        return NodeBuilder::createPrimaryExpression(identifier);
    }
    else if (currentToken.type == TokenType::OpenParen) {
        expectAndConsumeToken(TokenType::OpenParen);
        auto expression = parseExpression();
        expectAndConsumeToken(TokenType::CloseParen);
        return NodeBuilder::createPrimaryExpression(std::move(expression));
    }
    else {
        throw std::runtime_error("[Parser::parsePrimaryExpression] Expected an integer literal, an identifier or an expression in parentheses");
    }
}

void Parser::expectAndConsumeToken(TokenType expected) {
    expectToken(expected);
    consumeToken();
}

void Parser::expectToken(TokenType expected) {
    if (currentToken.type != expected) {
        throw std::runtime_error("[Parser::expectToken] Expected token type " + tokenTypeToString(expected) +
                                 ", but got " + tokenTypeToString(currentToken.type));
    }
}

void Parser::consumeToken() {
    currentToken = lexer.nextToken();
}

const Token& Parser::peekToken(int offset) const {
    return lexer.peekToken(offset);
}

bool Parser::isAddSubBinaryOperator(const Token& token) const {
    return token.type == TokenType::Plus || token.type == TokenType::Minus;
}

bool Parser::isMultDivBinaryOperator(const Token& token) const {
    return token.type == TokenType::Star || token.type == TokenType::Slash;
}

NodeExpressionBinary::BinaryOperator Parser::getBinaryOperator(const Token& token) const {
    switch (token.type) {
        case TokenType::Plus: return NodeExpressionBinary::BinaryOperator::Add;
        case TokenType::Minus: return NodeExpressionBinary::BinaryOperator::Subtract;
        case TokenType::Star: return NodeExpressionBinary::BinaryOperator::Multiply;
        case TokenType::Slash: return NodeExpressionBinary::BinaryOperator::Divide;
        default:
            throw std::runtime_error("[Parser::getBinaryOperator] Not a binary operator");
    }
}

} // namespace parser