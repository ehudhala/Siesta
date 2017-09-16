#include <iostream>

#include "gtest/gtest.h"

#include "parser.h"

TEST(parse_primary, parsing_non_expression_token_fails) {
    Lexer l("");
    std::ostringstream s;
    ASSERT_FALSE(parse_primary(l, s));
    ASSERT_EQ("Unexpected token when expecting an expression\n", s.str());
}

TEST(parse_primary, parsing_non_parentheses_char_fails) {
    Lexer l("$");
    std::ostringstream s;
    ASSERT_FALSE(parse_primary(l, s));
    ASSERT_EQ("Unexpected token when expecting an expression\n", s.str());
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
    ASSERT_EQ("Expected ')'\n", s.str());
}

TEST(parse_identifier, variable) {
    Lexer l("variable");
    std::ostringstream s;
    auto var = boost::get<VariableExprAst>(*parse_primary(l, s));
    ASSERT_EQ("variable", var.name);
}

TEST(parse_identifier_call, no_args) {
    Lexer l("call()");
    std::ostringstream s;
    auto call = boost::get<CallExprAst>(*parse_primary(l, s));
    ASSERT_EQ("call", call.callee);
    ASSERT_EQ(0, call.args.size());
}

TEST(parse_identifier_call, no_args_eats_closing_paren) {
    Lexer l("call() 12345");
    std::ostringstream s;
    parse_primary(l, s); // Eat call
    optional<ExprAst> expr = parse_primary(l, s);
    ASSERT_EQ(12345, boost::get<NumberExprAst>(*expr).val);
}

TEST(parse_identifier_call, one_arg) {
    Lexer l("call(1234)");
    std::ostringstream s;
    auto call = boost::get<CallExprAst>(*parse_primary(l, s));
    ASSERT_EQ("call", call.callee);
    ASSERT_EQ(1, call.args.size());
    ASSERT_EQ(1234, boost::get<NumberExprAst>(call.args.front()).val);
}

TEST(parse_identifier_call, many_args) {
    Lexer l("call(1234, variable, (12345))");
    std::ostringstream s;
    auto call = boost::get<CallExprAst>(*parse_primary(l, s));
    ASSERT_EQ("call", call.callee);
    ASSERT_EQ(3, call.args.size());
    ASSERT_EQ(1234, boost::get<NumberExprAst>(call.args[0]).val);
    ASSERT_EQ("variable", boost::get<VariableExprAst>(call.args[1]).name);
    ASSERT_EQ(12345, boost::get<NumberExprAst>(call.args[2]).val);
}

TEST(parse_identifier_call, no_comma) {
    Lexer l("call(1234 variable)");
    std::ostringstream s;
    ASSERT_FALSE(parse_primary(l, s));
    ASSERT_EQ("Expected ',' or ')' in argument list\n", s.str());
}

TEST(parse_identifier_call, arg_fails_to_parse) {
    Lexer l("call($)");
    std::ostringstream s;
    ASSERT_FALSE(parse_primary(l, s));
    ASSERT_EQ("Unexpected token when expecting an expression\n", s.str());
}

TEST(parse_identifier_call, comma_without_arg) {
    Lexer l("call(1234,)");
    std::ostringstream s;
    auto call = boost::get<CallExprAst>(*parse_primary(l, s));
    ASSERT_EQ("call", call.callee);
    ASSERT_EQ(1, call.args.size());
    ASSERT_EQ(1234, boost::get<NumberExprAst>(call.args.front()).val);
}

TEST(parse_identifier_call, no_closing_paren) {
    Lexer l("call(1234");
    std::ostringstream s;
    ASSERT_FALSE(parse_primary(l, s));
    ASSERT_EQ("Expected ',' or ')' in argument list\n", s.str());
}

TEST(prec_map, mul_equals_div) {
    auto prec = get_prec_map();
    ASSERT_EQ(prec['*'], prec['/']);
}

TEST(prec_map, plus_equals_minus) {
    auto prec = get_prec_map();
    ASSERT_EQ(prec['+'], prec['-']);
}

TEST(prec_map, lt_equals_gt) {
    auto prec = get_prec_map();
    ASSERT_EQ(prec['<'], prec['>']);
}

TEST(prec_map, mul_larger_than_plus) {
    auto prec = get_prec_map();
    ASSERT_GT(prec['*'], prec['+']);
}

TEST(prec_map, plus_larger_than_lt) {
    auto prec = get_prec_map();
    ASSERT_GT(prec['+'], prec['<']);
}

TEST(get_bin_op_precedence, not_char_token) {
    optional<int> eof = get_bin_op_precedence(EofToken(), std::map<char, int>{});
    ASSERT_FALSE(eof);
}

TEST(get_bin_op_precedence, char_token_not_in_map) {
    optional<int> plus = get_bin_op_precedence(CharToken<chars::plus>('+'), 
            std::map<char, int>{});
    ASSERT_FALSE(plus);
}

TEST(get_bin_op_precedence, char_token_in_map) {
    optional<int> plus = get_bin_op_precedence(CharToken<chars::plus>('+'), 
            std::map<char, int>{{'+', 1}});
    ASSERT_TRUE(bool(plus));
    ASSERT_EQ(*plus, 1);
}
