#include  <iostream>

#include "gtest/gtest.h"

#include "lexer.h"

TEST(get_identifier_token, splits_identifier_by_whitespace) {
    std::istringstream stream("identifier not_identifier_anymore");
    auto token = boost::get<IdentifierToken>(get_identifier_token(stream));
    ASSERT_EQ("identifier", token.get_identifier());
}

// TODO:
//TEST(get_identifier_token, splits_identifier_by_symbols) {
//    std::istringstream stream("identifier;not_identifier_anymore");
//    auto token = boost::get<IdentifierToken>(get_identifier_token(stream));
//    ASSERT_EQ("identifier", token.get_identifier());
//}

TEST(get_identifier_token, returns_def_token) {
    std::istringstream stream("def foo();");
    auto token = boost::get<DefToken>(get_identifier_token(stream));
    ASSERT_EQ("def", token.get_identifier());
}

TEST(get_identifier_token, returns_extern_token) {
    std::istringstream stream("extern bar;");
    auto token = boost::get<ExternToken>(get_identifier_token(stream));
    ASSERT_EQ("extern", token.get_identifier());
}

TEST(get_number_token, integer) {
    std::istringstream stream("1234");
    auto token = boost::get<NumberToken>(get_number_token(stream));
    ASSERT_EQ(1234, token.get_val());
}

// TODO: more tests to get-Num_token, test_get_token with nums.

TEST(get_token, recognizes_identifier) {
    std::istringstream stream("ident");
    auto token = boost::get<IdentifierToken>(get_token(stream));
    ASSERT_EQ("ident", token.get_identifier());
}

TEST(get_token, skips_whitespace) {
    std::istringstream stream("  \t\t  ident");
    auto token = boost::get<IdentifierToken>(get_token(stream));
    ASSERT_EQ("ident", token.get_identifier());
}

