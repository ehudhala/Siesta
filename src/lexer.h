#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <experimental/optional>

#include "boost/variant.hpp"

using std::experimental::optional;

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

namespace chars {
// TODO: refactor uses of chars.
// Currently, if we add a char we need to update the Token variant, get_char_token function and get_bin_op_precedence function.
    using open_paren = std::integral_constant<char, '('>;
    using close_paren = std::integral_constant<char, ')'>;
    using comma = std::integral_constant<char, ','>;
    using plus = std::integral_constant<char, '+'>;
    using minus = std::integral_constant<char, '-'>;
    using mul = std::integral_constant<char, '*'>;
    using div = std::integral_constant<char, '/'>;
    using lt = std::integral_constant<char, '<'>;
    using gt = std::integral_constant<char, '>'>;
    using semicolon = std::integral_constant<char, ';'>;
    struct unknown_char {};
}

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
    CharToken<chars::open_paren>,
    CharToken<chars::close_paren>,
    CharToken<chars::comma>,
    CharToken<chars::plus>,
    CharToken<chars::minus>,
    CharToken<chars::mul>,
    CharToken<chars::div>,
    CharToken<chars::lt>,
    CharToken<chars::gt>,
    CharToken<chars::semicolon>,
    CharToken<chars::unknown_char>>;

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

optional<char> get_char(Token);
