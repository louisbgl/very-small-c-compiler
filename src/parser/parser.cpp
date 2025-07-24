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
    if (currentToken.type != TokenType::Number) {
        throw std::runtime_error("[Parser::parseExpression] Expected an integer literal");
    }

    int value = std::stoi(std::string(currentToken.lexeme));
    expectAndConsumeToken(TokenType::Number);

    if (isBinaryOperator(currentToken) && peekToken().type == TokenType::Number) {
        auto op = getBinaryOperator(currentToken);
        consumeToken(); // Consume the operator token

        if (currentToken.type != TokenType::Number) {
            throw std::runtime_error("[Parser::parseExpression] Expected a number after binary operator");
        }
        int rightValue = std::stoi(std::string(currentToken.lexeme));
        expectAndConsumeToken(TokenType::Number);

        auto leftExpr = NodeBuilder::makePrimaryExpression(value);
        auto rightExpr = NodeBuilder::makePrimaryExpression(rightValue);
        
        return NodeBuilder::createBinaryExpression(op, std::move(leftExpr), std::move(rightExpr));
    }

    return NodeBuilder::createPrimaryExpression(value);
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

bool Parser::isBinaryOperator(const Token& token) const {
    return token.type == TokenType::Plus || token.type == TokenType::Minus;
}

NodeExpressionBinary::BinaryOperator Parser::getBinaryOperator(const Token& token) const {
    switch (token.type) {
        case TokenType::Plus: return NodeExpressionBinary::BinaryOperator::Add;
        case TokenType::Minus: return NodeExpressionBinary::BinaryOperator::Subtract;
        default:
            throw std::runtime_error("[Parser::getBinaryOperator] Not a binary operator");
    }
}

} // namespace parser