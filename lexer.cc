#include <cctype>

#include "lexer.h"

IdentifierToken::IdentifierToken(const std::string& identifier)
    : m_identifier(identifier) {}

const std::string& IdentifierToken::get_identifier() const {
    return m_identifier;
}

NumberToken::NumberToken(double val) : m_val(val) {}

double NumberToken::get_val() const {
    return m_val;
}

CharToken::CharToken(char val) : m_val(val) {}

char CharToken::get_val() const {
    return m_val;
}

Token get_identifier_token(std::istream& input) {
    std::string identifier;
    while (std::isalnum(input.peek()) || input.peek() == '_') {
        identifier += input.get();
    }

    if (identifier == "def") {
        return DefToken(identifier);
    }

    if (identifier == "extern") {
        return ExternToken(identifier);
    }

    return IdentifierToken(identifier);
}

Token get_number_token(std::istream& input) {
    double num;
    input >> num;
    return NumberToken(num);
}

Token handle_comment(std::istream& input) {
    std::string comment;
    std::getline(input, comment);
    return get_token(input);
}

Token get_token(std::istream& input) {
    input >> std::ws;

    if (std::isalpha(input.peek())) {
        return get_identifier_token(input);
    }

    if (std::isdigit(input.peek())) {
        return get_number_token(input);
    }

    if (input.peek() == '#') {
        return handle_comment(input);
    }

    if (input.peek() == std::char_traits<char>::eof()) {
        return EofToken();
    }

    return EofToken();
}
