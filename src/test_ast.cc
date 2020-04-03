#include "gtest/gtest.h"

#include "ast.h"

TEST(ast_equals, different_types) {
    ASSERT_FALSE(NumberExprAst(1) == VariableExprAst("a"));
}

TEST(ast_equals, number) {
    ASSERT_TRUE(NumberExprAst(1) == NumberExprAst(1));
    ASSERT_FALSE(NumberExprAst(1) == NumberExprAst(2));
    ASSERT_FALSE(NumberExprAst(2) == NumberExprAst(1));
}

TEST(ast_equals, variable) {
    ASSERT_TRUE(VariableExprAst("a") == VariableExprAst("a"));
    ASSERT_FALSE(VariableExprAst("a") == VariableExprAst("b"));
    ASSERT_FALSE(VariableExprAst("b") == VariableExprAst("a"));
}

TEST(BinaryExprAstEquals, equals) {
    ASSERT_TRUE(BinaryExprAst('+', NumberExprAst(1), NumberExprAst(2)) ==
                BinaryExprAst('+', NumberExprAst(1), NumberExprAst(2)));
}

TEST(BinaryExprAstEquals, unequal_op) {
    ASSERT_FALSE(BinaryExprAst('+', NumberExprAst(1), NumberExprAst(2)) ==
                BinaryExprAst('*', NumberExprAst(1), NumberExprAst(2)));
}

TEST(BinaryExprAstEquals, unequal_child) {
    ASSERT_FALSE(BinaryExprAst('+', NumberExprAst(2), NumberExprAst(2)) ==
                BinaryExprAst('+', NumberExprAst(1), NumberExprAst(2)));

    ASSERT_FALSE(BinaryExprAst('+', NumberExprAst(1), NumberExprAst(1)) ==
                BinaryExprAst('+', NumberExprAst(1), NumberExprAst(2)));
}
