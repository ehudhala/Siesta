#include <cctype>

#include "lexer.h"

IdentifierToken::IdentifierToken(const std::string& identifier)
    : m_identifier(identifier) {}

const std::string& IdentifierToken::get_identifier() const {
    return m_identifier;
}

NumberToken::NumberToken(double value) : val(value) {}

template <class v>
CharToken<v>::CharToken(char value) : val(value) {}

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

Token get_char_token(std::istream& input) {
    char c = input.get();
    switch (c) {
    default:
        return CharToken<chars::unknown_char>(c);
    case '(':
        return CharToken<chars::open_paren>(c);
    case ')':
        return CharToken<chars::close_paren>(c);
    case ',':
        return CharToken<chars::comma>(c);
    case '+':
        return CharToken<chars::plus>(c);
    case '-':
        return CharToken<chars::minus>(c);
    case '*':
        return CharToken<chars::mul>(c);
    case '/':
        return CharToken<chars::div>(c);
    case '<':
        return CharToken<chars::lt>(c);
    case '>':
        return CharToken<chars::gt>(c);
    }
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

    return get_char_token(input);
}

Lexer::Lexer(std::istream& input) : m_string(""), m_input(input) {
    next_token();
}
Lexer::Lexer(std::string input) : m_string(input), m_input(m_string) {
    next_token();
}

Token Lexer::next_token() {
    return curr_token = get_token(m_input);
}
