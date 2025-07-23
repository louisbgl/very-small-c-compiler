#pragma once

#include "nodes.hpp"
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
    NodeExpression parseExpression();

    /* Helper functions */
    void expectAndConsumeToken(TokenType expected);
    void expectToken(TokenType expected);
    void consumeToken();
    const Token& peekToken(int offset) const;
    NodeStatement::StatementType findStatementType();
    NodeStatement parseReturnStatement();
};

} // namespace parser