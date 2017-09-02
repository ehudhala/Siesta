#pragma once

#include <string>
#include <iostream>

#include "boost/variant.hpp"

class EofToken {
};

class IdentifierToken {
public:
    IdentifierToken(const std::string& identifier);

    const std::string& get_identifier() const;
private:
    std::string m_identifier;
};

class DefToken : public IdentifierToken {
    using IdentifierToken::IdentifierToken;
};

class ExternToken : public IdentifierToken {
    using IdentifierToken::IdentifierToken;
};

class NumberToken {
public:
    NumberToken(double val);

    double get_val() const;
private:
    double m_val;
};

class CharToken {
public:
    CharToken(char val);

    char get_val() const;
private:
    char m_val;
};

using Token = boost::variant<
    EofToken, 
    DefToken, 
    ExternToken, 
    IdentifierToken, 
    NumberToken,
    CharToken>;

Token get_identifier_token(std::istream& input);
Token get_number_token(std::istream& input);
Token handle_comment(std::istream& input);
Token get_token(std::istream& inupt);
