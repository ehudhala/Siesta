#pragma once

#include <string>
#include <iostream>
#include <sstream>

#include "boost/variant.hpp"

class EofToken {};

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

    double val;
};

using open_paren = std::integral_constant<char, '('>;
using close_paren = std::integral_constant<char, ')'>;
using comma = std::integral_constant<char, ','>;
struct unknown_char {};

template<class value>
class CharToken {
public:
    CharToken(char val);

    char val;
};

using Token = boost::variant<
    EofToken, 
    DefToken, 
    ExternToken, 
    IdentifierToken, 
    NumberToken,
    CharToken<open_paren>,
    CharToken<close_paren>,
    CharToken<comma>,
    CharToken<unknown_char>>;

Token get_identifier_token(std::istream& input);
Token get_number_token(std::istream& input);
Token handle_comment(std::istream& input);
Token get_token(std::istream& inupt);
Token get_char_token(std::istream& input);

class Lexer {
public:
    Lexer(std::istream& input);
    Lexer(std::string input);
    Token next_token();

    Token curr_token;
private:
    std::istringstream m_string;
    std::istream& m_input;
};
