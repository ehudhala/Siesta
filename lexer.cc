#include <cctype>

#include "lexer.h"

IdentifierToken::IdentifierToken(const std::string& identifier)
    : m_identifier(identifier) {}

const std::string& IdentifierToken::get_identifier() const {
    return m_identifier;
}

NumberToken::NumberToken(int val) : m_val(val) {}

double NumberToken::get_val() const {
    return m_val;
}

Token get_identifier_token(std::istream& input) {
    std::string identifier;
    input >> identifier;

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

Token get_token(std::istream& input) {
    input >> std::ws;

    if (std::isalpha(input.peek())) {
        return get_identifier_token(input);
    }

    if (std::isdigit(input.peek())) {
        return get_number_token(input);
    }

    return EofToken();
}
