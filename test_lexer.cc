#include  <iostream>

#include "gtest/gtest.h"

#include "lexer.h"

TEST(get_next_char, simple_char) {
    std::istringstream stream("c");
    char c = get_next_char(stream);
    ASSERT_EQ('c', c);
}

TEST(get_next_char, string) {
    std::istringstream stream("many characters in string");
    ASSERT_EQ('m', get_next_char(stream));
    ASSERT_EQ('a', get_next_char(stream));
}

TEST(get_next_char, space_before_char) {
    std::istringstream stream(" c");
    char c = get_next_char(stream);
    ASSERT_EQ('c', c);
}

TEST(get_next_char, tab_before_char) {
    std::istringstream stream("\tc");
    char c = get_next_char(stream);
    ASSERT_EQ('c', c);
}

TEST(get_next_char, tab_and_spacesbefore_char) {
    std::istringstream stream("\t  \t  c");
    char c = get_next_char(stream);
    ASSERT_EQ('c', c);
}

