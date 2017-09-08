#include <iostream>

#include "gtest/gtest.h"

#include "parser.h"

TEST(parse_primary, parsing_char_fails) {
    Lexer l(")");
    ASSERT_FALSE(parse_primary(l));
}
