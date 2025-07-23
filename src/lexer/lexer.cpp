#include <iostream>
#include "lexer.hpp"

namespace lexer {

// Define a map of keywords to their token types
struct KeywordEntry {
    std::string_view keyword;
    TokenType type;
};

// Static array of keywords for fast lookup
static const KeywordEntry keywords[] = {
    {"int", TokenType::Keyword_int},
    {"return", TokenType::Keyword_return},
    // Add more keywords here as needed
};

// Returns a safe EOF token when needed
static const Token& getEofToken() {
    static const Token eofToken(TokenType::EndOfFile, "", 1, 1);
    return eofToken;
}

Lexer::Lexer(std::string_view source)
    : source(source), tokens(), tokenIndex(0), readPosition(0) {
    tokenize();
}


const Token &Lexer::nextToken() {
    if (tokens.empty()) {
        return getEofToken();
    }
    
    if (tokenIndex < tokens.size()) {
        auto& token = tokens[tokenIndex];
        tokenIndex++;
        return token;
    }
    return tokens.back(); // Return EndOfFile token
}

const Token &Lexer::peekToken(int index) const {
    if (tokens.empty()) {
        return getEofToken();
    }
    
    if (tokenIndex + index < tokens.size()) {
        return tokens[tokenIndex + index];
    }
    return tokens.back(); // Return EndOfFile token
}

void Lexer::printTokens() const {
    for (const auto& token : tokens) {
        std::cout << token.ToString() << std::endl;
    }
}

void Lexer::tokenize() {
    size_t line = 1;
    size_t column = 1;
    Token token(TokenType::Unknown, "", line, column);

    // Always ensure the token list will have at least one EndOfFile token
    tokens.reserve(32); // Preallocate for better performance
    
    while (!isAtEnd()) {
        skipWhitespace(line, column);
        skipComment(line, column);

        if (isAtEnd()) break;
        
        // Update token position
        token.line = line;
        token.column = column;
        
        if (tryGetSingleCharToken(token) || 
            tryGetLiteralToken(token) || 
            tryGetKeywordToken(token) || 
            tryGetIdentifierToken(token)) {
            tokens.push_back(token);
            advance(token.lexeme.size());
            advanceLineColumn(line, column, token.lexeme);
        } else {
            // Handle unknown characters
            createToken(token, TokenType::Unknown, std::string_view(&source[readPosition], 1));
            tokens.push_back(token);
            advance();
            column++;
        }
    }
    
    // Add EndOfFile token
    tokens.emplace_back(TokenType::EndOfFile, "", line, column);
}

char Lexer::currentChar() const {
    if (isAtEnd()) return '\0';
    return source[readPosition];
}

char Lexer::peekChar(int offset) const {
    if (readPosition + offset >= source.size()) return '\0';
    return source[readPosition + offset];
}

bool Lexer::isAtEnd() const {
    return readPosition >= source.size();
}

void Lexer::skipWhitespace(size_t& line, size_t& column) {
    while (!isAtEnd() && std::isspace(currentChar())) {
        if (currentChar() == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        readPosition++;
    }
}

void Lexer::skipComment(size_t& line, size_t& column) {
    if (!(currentChar() == '/' && peekChar(1) == '/')) return; // Not a comment
    
    // Move past the '//'
    readPosition += 2;
    column += 2;
    
    while (!isAtEnd() && currentChar() != '\n') {
        readPosition++;
        column++;
    }
    
    // Handle the newline at the end of the comment
    if (!isAtEnd() && currentChar() == '\n') {
        readPosition++;
        line++;
        column = 1;
    }
}

bool Lexer::tryGetSingleCharToken(Token& token) {
    char c = currentChar();
    switch (c) {
        case ';': createToken(token, TokenType::Semicolon,  ";"); return true;
        case '(': createToken(token, TokenType::OpenParen,  "("); return true;
        case ')': createToken(token, TokenType::CloseParen, ")"); return true;
        case '{': createToken(token, TokenType::OpenBrace,  "{"); return true;
        case '}': createToken(token, TokenType::CloseBrace, "}"); return true;
        case '+': createToken(token, TokenType::Plus,       "+"); return true;
        case '-': createToken(token, TokenType::Minus,      "-"); return true;
        case '*': createToken(token, TokenType::Star,       "*"); return true;
        case '/': createToken(token, TokenType::Slash,      "/"); return true;
        default:  return false;
    }
}

bool Lexer::tryGetLiteralToken(Token& token) {
    // First, try to match a number
    if (std::isdigit(currentChar())) {
        return tryGetNumberLiteral(token);
    }
    
    // Then, try to match a string
    if (currentChar() == '"') {
        return tryGetStringLiteral(token);
    }
    
    // Character literals could be added here in the future
    // if (currentChar() == '\'') {
    //     return tryGetCharLiteral(token);
    // }
    
    return false;
}

bool Lexer::tryGetNumberLiteral(Token& token) {
    std::string_view lexeme = currentLexeme();
    size_t index = 0;
    
    // Parse integer part
    while (index < lexeme.size() && std::isdigit(lexeme[index])) {
        index++;
    }
    
    // Future expansion: Handle floating point, hex, octal numbers, etc.
    
    if (index > 0) {
        createToken(token, TokenType::Number, lexeme.substr(0, index));
        return true;
    }
    return false;
}

bool Lexer::tryGetStringLiteral(Token& token) {
    std::string_view lexeme = currentLexeme();
    size_t index = 0;
    
    if (lexeme[index] == '"') {
        index++; // Skip the opening quote
        
        while (index < lexeme.size() && lexeme[index] != '"') {
            if (lexeme[index] == '\\' && index + 1 < lexeme.size()) {
                // Handle escape sequences safely
                index += 2;
            } else {
                index++;
            }
        }
        
        if (index < lexeme.size() && lexeme[index] == '"') {
            index++; // Skip the closing quote
            createToken(token, TokenType::String, lexeme.substr(0, index));
            return true;
        }
    }
    return false;
}

bool Lexer::tryGetKeywordToken(Token& token) {
    std::string_view lexeme = currentLexeme();
    
    // Check against our keyword lookup table
    for (const auto& entry : keywords) {
        if (isStandaloneKeyword(lexeme, entry.keyword)) {
            createToken(token, entry.type, entry.keyword);
            return true;
        }
    }
    
    return false;
}

bool Lexer::tryGetIdentifierToken(Token& token) {
    std::string_view lexeme = currentLexeme();
    
    std::string_view word = extractWord(lexeme, 0);
    
    if (!word.empty()) {
        createToken(token, TokenType::Identifier, word);
        return true;
    }
    return false;
}

bool Lexer::isStandaloneKeyword(const std::string_view& lexeme, const std::string_view& keyword) const {
    if (!lexeme.starts_with(keyword)) {
        return false;
    }
    
    // Check if the keyword is a standalone word (not part of a longer identifier)
    // A keyword is standalone if it's at the end of the lexeme or followed by a non-identifier character
    size_t keywordLength = keyword.size();
    return lexeme.size() == keywordLength || 
           (lexeme.size() > keywordLength && 
            !std::isalnum(lexeme[keywordLength]) && 
            lexeme[keywordLength] != '_');
}

void Lexer::advanceLineColumn(size_t& line, size_t& column, const std::string_view& lexeme) {
    for (char c : lexeme) {
        if (c == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
    }
}

std::string_view Lexer::extractWord(const std::string_view& source, size_t start) {
    size_t index = start;
    
    // First character must be a letter or underscore
    if (index < source.size() && (std::isalpha(source[index]) || source[index] == '_')) {
        index++;
        // Subsequent characters can be alphanumeric or underscore
        while (index < source.size() && (std::isalnum(source[index]) || source[index] == '_')) {
            index++;
        }
    }
    
    return source.substr(start, index - start);
}

void Lexer::createToken(Token& token, TokenType type, std::string_view lexeme) {
    token = Token(type, lexeme, token.line, token.column);
}

std::string_view Lexer::currentLexeme() const {
    return source.substr(readPosition);
}

void Lexer::advance(size_t count) {
    readPosition += count;
}

} // namespace lexer