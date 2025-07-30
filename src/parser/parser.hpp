#pragma once

#include "nodes.hpp"
#include "nodeBuilder.hpp"
#include "../lexer/token.hpp"

namespace lexer {
    class Lexer; // Forward declaration of Lexer class
}
enum class TokenType; // Forward declaration of TokenType enum

namespace parser {

class Parser {
public:
    explicit Parser(lexer::Lexer& lexer);

    const std::unique_ptr<NodeProgram>& getProgram() const;

    void printProgram() const;
    
private:
    lexer::Lexer& lexer;
    Token currentToken;
    std::unique_ptr<NodeProgram> program;
    
    /* Parsing functions */
    std::unique_ptr<NodeProgram> parseProgram();
    NodeFunction parseFunction();
    NodeCompoundStatement parseCompoundStatement();
    NodeStatement parseStatement();
    NodeStatement parseReturnStatement();
    NodeStatement parseVariableDeclaration();
    NodeStatement parseAssignmentStatement();
    NodeStatement parseIfStatement();
    NodeExpression parseExpression();
    NodeExpression parseAddSubExpression();
    NodeExpression parseMultDivExpression();
    NodeExpression parsePrimaryExpression();

    /* Helper functions */
    void expectAndConsumeToken(TokenType expected);
    void expectToken(TokenType expected);
    void consumeToken();

    // Looks at the token after CurrentToken without consuming it (with optional offset to look ahead even more)
    const Token& peekToken(int offset = 0) const;

    bool isAddSubBinaryOperator(const Token& token) const;
    bool isMultDivBinaryOperator(const Token& token) const;
    NodeExpressionBinary::BinaryOperator getBinaryOperator(const Token& token) const;
};

} // namespace parser