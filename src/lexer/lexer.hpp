#pragma once

#include <string_view>
#include <vector>
#include "token.hpp"

namespace lexer {

// C language lexer that tokenizes source code
class Lexer {
public:
    explicit Lexer(std::string_view source);

    // Advances to and returns the next token
    const Token &nextToken();

    // Looks ahead at tokens without advancing (index=0 is current token)
    const Token &peekToken(int index = 0) const;

    // Prints the tokens for debugging
    void printTokens() const;

private:
    std::string_view source;
    std::vector<Token> tokens;
    size_t tokenIndex = 0;
    size_t readPosition = 0;

    // Main tokenization method called from constructor
    void tokenize();

    // Character and position utilities
    char currentChar() const;
    char peekChar(int offset) const;
    bool isAtEnd() const;
    void skipWhitespace(size_t& line, size_t& column);
    bool skipComment(size_t& line, size_t& column);
    
    // Token matching methods
    bool tryGetSimpleToken(Token& token);
    bool tryGetSingleCharToken(Token& token);
    bool tryGetMultiCharToken(Token& token);
    bool tryGetLiteralToken(Token& token);
    bool tryGetNumberLiteral(Token& token);
    bool tryGetStringLiteral(Token& token);
    bool tryGetKeywordToken(Token& token);
    bool tryGetIdentifierToken(Token& token);
    
    // Helper functions
    bool isStandaloneKeyword(const std::string_view& lexeme, const std::string_view& keyword) const;
    void advanceLineColumn(size_t& line, size_t& column, const std::string_view& lexeme);
    std::string_view extractWord(const std::string_view& source, size_t start);
    void createToken(Token& token, TokenType type, std::string_view lexeme);
    std::string_view currentLexeme() const;
    void advance(size_t count = 1);
};

} // namespace lexer