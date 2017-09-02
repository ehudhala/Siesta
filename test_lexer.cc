#include  <iostream>

#include "gtest/gtest.h"

#include "lexer.h"

TEST(get_identifier_token, splits_identifier_by_whitespace) {
    std::istringstream stream("identifier not_identifier_anymore");
    auto token = boost::get<IdentifierToken>(get_identifier_token(stream));
    ASSERT_EQ("identifier", token.get_identifier());
}

TEST(get_identifier_token, splits_identifier_when_not_alphanumeric) {
    std::istringstream stream("identifier#another;another_again(yet_again)");
    auto token = boost::get<IdentifierToken>(get_identifier_token(stream));
    ASSERT_EQ("identifier", token.get_identifier());
    stream.get();
    token = boost::get<IdentifierToken>(get_identifier_token(stream));
    ASSERT_EQ("another", token.get_identifier());
    stream.get();
    token = boost::get<IdentifierToken>(get_identifier_token(stream));
    ASSERT_EQ("another_again", token.get_identifier());
    stream.get();
    token = boost::get<IdentifierToken>(get_identifier_token(stream));
    ASSERT_EQ("yet_again", token.get_identifier());
}

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

TEST(get_number_token, integer_num) {
    std::istringstream stream("1234");
    auto token = boost::get<NumberToken>(get_number_token(stream));
    ASSERT_EQ(1234, token.get_val());
}

TEST(get_number_token, double_num) {
    std::istringstream stream("12.3456");
    auto token = boost::get<NumberToken>(get_number_token(stream));
    ASSERT_EQ(12.3456, token.get_val());
}

TEST(get_token, recognizes_identifier) {
    std::istringstream stream("ident");
    auto token = boost::get<IdentifierToken>(get_token(stream));
    ASSERT_EQ("ident", token.get_identifier());
}

TEST(get_token, recognizes_number) {
    std::istringstream stream("1234");
    auto token = boost::get<NumberToken>(get_token(stream));
    ASSERT_EQ(1234, token.get_val());
}

TEST(get_token, skips_whitespace) {
    std::istringstream stream("  \t\t  ident");
    auto token = boost::get<IdentifierToken>(get_token(stream));
    ASSERT_EQ("ident", token.get_identifier());
}

