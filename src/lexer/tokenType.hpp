#pragma once

#include <string>

enum class TokenType {
    // Special tokens
    Unknown,
    EndOfFile,

    // Keywords
    Keyword_int,
    Keyword_return,
    Keyword_if,
    Keyword_else,
    Keyword_elseif,

    // Identifiers and literals
    Identifier,
    String,
    Number,

    // Punctuation and delimiters
    Semicolon,
    OpenParen, CloseParen,
    OpenBrace, CloseBrace,

    // Operators
    Equal,
    Plus,
    Minus,
    Star,
    Slash,
};

inline const std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::Unknown: return "\033[31mUnknown\033[0m"; // Red for unknown tokens
        case TokenType::EndOfFile: return "EndOfFile";
        case TokenType::Keyword_int: return "Keyword_int";
        case TokenType::Keyword_return: return "Keyword_return";
        case TokenType::Keyword_if: return "Keyword_if";
        case TokenType::Keyword_else: return "Keyword_else";
        case TokenType::Keyword_elseif: return "Keyword_elseif";
        case TokenType::Identifier: return "Identifier";
        case TokenType::String: return "String";
        case TokenType::Number: return "Number";
        case TokenType::Semicolon: return "Semicolon";
        case TokenType::OpenParen: return "OpenParen";
        case TokenType::CloseParen: return "CloseParen";
        case TokenType::OpenBrace: return "OpenBrace";
        case TokenType::CloseBrace: return "CloseBrace";
        case TokenType::Equal: return "Equal";
        case TokenType::Plus: return "Plus";
        case TokenType::Minus: return "Minus";
        case TokenType::Star: return "Star";
        case TokenType::Slash: return "Slash";
    }
    return "\033[31mUnknown\033[0m"; // Fallback, red for unknown tokens
}