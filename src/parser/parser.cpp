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
    NodeStatement::StatementType statementType = findStatementType();

    switch (statementType) {
        case NodeStatement::StatementType::Return:
            return parseReturnStatement();
        case NodeStatement::StatementType::Empty: {
            expectAndConsumeToken(TokenType::Semicolon);
            return NodeBuilder::createEmptyStatement();
        }
        default:
            throw std::runtime_error("[Parser::parseStatement] Unknown statement type");
    }
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
    else if (currentToken.type == TokenType::OpenParen) {
        expectAndConsumeToken(TokenType::OpenParen);
        auto expression = parseExpression();
        expectAndConsumeToken(TokenType::CloseParen);
        return NodeBuilder::createPrimaryExpression(std::move(expression));
    }
    else {
        throw std::runtime_error("[Parser::parsePrimaryExpression] Expected an integer literal or an expression in parentheses");
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

NodeStatement::StatementType Parser::findStatementType() {
    if (currentToken.type == TokenType::Semicolon) {
        return NodeStatement::StatementType::Empty;
    }

    if (currentToken.type == TokenType::Keyword_return) {
        return NodeStatement::StatementType::Return;
    }

    throw std::runtime_error("[Parser::findStatementType] Unknown statement type");
}

NodeStatement Parser::parseReturnStatement() {
    expectAndConsumeToken(TokenType::Keyword_return);

    auto expression = parseExpression();
    expectAndConsumeToken(TokenType::Semicolon);
    
    return NodeBuilder::createReturnStatement(std::move(expression));
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