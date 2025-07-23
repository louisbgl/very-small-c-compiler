#pragma once

#include <string>

enum class TokenType {
    // Special tokens
    Unknown,
    EndOfFile,

    // Keywords
    Keyword_int,
    Keyword_return,

    // Identifiers and literals
    Identifier,
    String,
    Number,

    // Punctuation and delimiters
    Semicolon,
    OpenParen, CloseParen,
    OpenBrace, CloseBrace,

    // Operators
    Plus,
    Minus,
    Star,
    Slash,
};

inline const std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::Unknown: return "Unknown";
        case TokenType::EndOfFile: return "EndOfFile";
        case TokenType::Keyword_int: return "Keyword_int";
        case TokenType::Keyword_return: return "Keyword_return";
        case TokenType::Identifier: return "Identifier";
        case TokenType::String: return "String";
        case TokenType::Number: return "Number";
        case TokenType::Semicolon: return "Semicolon";
        case TokenType::OpenParen: return "OpenParen";
        case TokenType::CloseParen: return "CloseParen";
        case TokenType::OpenBrace: return "OpenBrace";
        case TokenType::CloseBrace: return "CloseBrace";
        case TokenType::Plus: return "Plus";
        case TokenType::Minus: return "Minus";
        case TokenType::Star: return "Star";
        case TokenType::Slash: return "Slash";
    }
    return "Unknown"; // Fallback
}