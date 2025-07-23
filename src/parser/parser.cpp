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
    auto program = std::make_unique<NodeProgram>();
    program->main = std::make_unique<NodeFunction>(parseFunction());
    return program;
}

NodeFunction Parser::parseFunction() {
    expectAndConsumeToken(TokenType::Keyword_int);

    expectToken(TokenType::Identifier);
    std::string functionName = static_cast<std::string>(currentToken.lexeme);
    consumeToken(); // Consume identifier token

    expectAndConsumeToken(TokenType::OpenParen);
    expectAndConsumeToken(TokenType::CloseParen);

    NodeFunction function;
    function.type = NodeFunction::FunctionType::Int;
    function.name = std::move(functionName);
    function.body = parseCompoundStatement();
    return function;
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
            NodeStatement statement;
            statement.type = NodeStatement::StatementType::Empty;
            return statement;
        }
        default:
            throw std::runtime_error("[Parser::parseStatement] Unknown statement type");
    }
}

NodeExpression Parser::parseExpression() {
    if (currentToken.type == TokenType::Number) {
        int value = std::stoi(std::string(currentToken.lexeme));
        consumeToken(); // Consume number token

        NodeExpression expression;
        expression.type = NodeExpression::ExpressionType::IntegerLiteral;
        expression.intValue = value;
        return expression;
    }

    throw std::runtime_error("[Parser::parseExpression] Expected an integer literal");
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

    NodeStatement returnStatement;
    returnStatement.type = NodeStatement::StatementType::Return;
    returnStatement.expression = parseExpression();
    
    expectAndConsumeToken(TokenType::Semicolon);
    return returnStatement;
}

} // namespace parser