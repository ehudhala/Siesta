#include <iostream>

#include "gtest/gtest.h"

#include "parser.h"

TEST(parse_primary, parsing_non_expression_token_fails) {
    Lexer l("");
    std::ostringstream s;
    ASSERT_FALSE(parse_primary(l, s));
    ASSERT_EQ(s.str(), "Unexpected token when expecting an expression");
}

TEST(parse_primary, parsing_non_parentheses_char_fails) {
    Lexer l("$");
    std::ostringstream s;
    ASSERT_FALSE(parse_primary(l, s));
    ASSERT_EQ(s.str(), "Unexpected token when expecting an expression");
}

TEST(parse_primary, parsing_number_token) {
    Lexer l("12345");
    std::ostringstream s;
    optional<ExprAst> expr = parse_primary(l, s);
    ASSERT_EQ(12345, boost::get<NumberExprAst>(*expr).val);
}

TEST(parse_paren, returning_inner_token) {
    Lexer l("(12345)");
    std::ostringstream s;
    optional<ExprAst> expr = parse_primary(l, s);
    ASSERT_EQ(12345, boost::get<NumberExprAst>(*expr).val);
}

TEST(parse_paren, returns_error_on_error) {
    Lexer l("($)");
    std::ostringstream s;
    ASSERT_FALSE(parse_primary(l, s));
}

TEST(parse_paren, raises_error_when_not_closed) {
    Lexer l("(12345");
    std::ostringstream s;
    ASSERT_FALSE(parse_primary(l, s));
    ASSERT_EQ(s.str(), "Expected ')'");
}

TEST(parse_identifier, variable) {
    Lexer l("variable");
    std::ostringstream s;
    auto var = boost::get<VariableExprAst>(*parse_primary(l, s));
    ASSERT_EQ("variable", var.name);
}
