#include  <iostream>

#include "gtest/gtest.h"

#include "lexer.h"

using namespace chars;

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
    ASSERT_EQ(1234, token.val);
}

TEST(get_number_token, double_num) {
    std::istringstream stream("12.3456");
    auto token = boost::get<NumberToken>(get_number_token(stream));
    ASSERT_EQ(12.3456, token.val);
}

TEST(handle_comment, skips_after_newline) {
    std::istringstream stream("#comment\nidentifier");
    auto token = boost::get<IdentifierToken>(handle_comment(stream));
    ASSERT_EQ("identifier", token.get_identifier());
}

TEST(handle_comment, skips_handles_eof) {
    std::istringstream stream("#comment\n");
    boost::get<EofToken>(handle_comment(stream));
}

TEST(get_token, recognizes_identifier) {
    std::istringstream stream("ident");
    auto token = boost::get<IdentifierToken>(get_token(stream));
    ASSERT_EQ("ident", token.get_identifier());
}

TEST(get_token, recognizes_number) {
    std::istringstream stream("1234");
    auto token = boost::get<NumberToken>(get_token(stream));
    ASSERT_EQ(1234, token.val);
}

TEST(get_token, recognizes_comment) {
    std::istringstream stream("#comment");
    boost::get<EofToken>(handle_comment(stream));
}

TEST(get_token, recognizes_eof) {
    std::istringstream stream("");
    boost::get<EofToken>(get_token(stream));
}

TEST(get_token, recognizes_eof_after_identifier) {
    std::istringstream stream("ident");
    auto token = boost::get<IdentifierToken>(get_token(stream));
    ASSERT_EQ("ident", token.get_identifier());
    boost::get<EofToken>(get_token(stream));
}

TEST(get_token, returns_char_token_when_unknown) {
    std::istringstream stream("(func)");
    auto token = boost::get<CharToken<open_paren>>(get_token(stream));
    ASSERT_EQ('(', token.val);
}

TEST(get_token, skips_whitespace) {
    std::istringstream stream("  \t\t  ident");
    auto token = boost::get<IdentifierToken>(get_token(stream));
    ASSERT_EQ("ident", token.get_identifier());
}

TEST(lexer, uses_stream) {
    std::istringstream stream("ident#comment\n12.34");
    Lexer lexer(stream);
    auto token = boost::get<IdentifierToken>(lexer.curr_token);
    ASSERT_EQ("ident", token.get_identifier());
    auto num_token = boost::get<NumberToken>(lexer.next_token());
    ASSERT_EQ(12.34, num_token.val);
    Lexer same_stream_lexer(stream);
    boost::get<EofToken>(same_stream_lexer.next_token());
}

TEST(get_char, char_token) {
    CharToken<chars::open_paren> open_paren('(');
    CharToken<chars::close_paren> close_paren(')');
    CharToken<chars::comma> comma(',');
    CharToken<chars::plus> plus('+');
    ASSERT_EQ('(', *get_char(open_paren));
    ASSERT_EQ(')', *get_char(close_paren));
    ASSERT_EQ(',', *get_char(comma));
    ASSERT_EQ('+', *get_char(plus));
}

TEST(get_char, unknown_char_token) {
    CharToken<chars::unknown_char> question('?');
    ASSERT_EQ('?', *get_char(question));
}

TEST(get_char, non_char_token) {
    ASSERT_FALSE(get_char(EofToken()));
    ASSERT_FALSE(get_char(DefToken("def")));
    ASSERT_FALSE(get_char(NumberToken(1234)));
}
