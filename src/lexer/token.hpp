#pragma once

#include <string>
#include <string_view>
#include "tokenType.hpp"

struct Token {
    TokenType type;
    std::string_view lexeme;
    size_t line;
    size_t column;

    Token(TokenType type, std::string_view lexeme, size_t line, size_t column)
        : type(type), lexeme(lexeme), line(line), column(column) {}

    Token(TokenType type, char lexeme, size_t line, size_t column)
        : type(type), lexeme(std::string_view(&lexeme, 1)), line(line), column(column) {}

    std::string ToString() const {
        std::string lineStr = std::to_string(this->line);
        std::string columnStr = std::to_string(this->column);
        std::string typeStr = tokenTypeToString(this->type);
        std::string lexemeStr = std::string(this->lexeme);
        return "[" + lineStr + ":" + columnStr + "] " + typeStr + " '" + lexemeStr + "'";
    }
};