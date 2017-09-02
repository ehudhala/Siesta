#include "lexer.h"


IdentifierToken::IdentifierToken(const std::string& identifier)
    : m_identifier(identifier) {}

const std::string& IdentifierToken::get_identifier() const {
    return m_identifier;
}

NumberToken::NumberToken(int val) : m_val(val) {}

int NumberToken::get_val() const {
    return m_val;
}

Token get_token(std::istream& inupt) {
    return EofToken();
}
