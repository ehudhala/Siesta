#include "parser.h"

NumberExprAst::NumberExprAst(double val) : val(val) {}

VariableExprAst::VariableExprAst(std::string name) : name(std::move(name)) {}

BinaryExprAst::BinaryExprAst(char op, ExprAst left, ExprAst right)
    : op(op), left(std::move(left)), right(std::move(right)) {}

const ExprAst& BinaryExprAst::get_left() const {
    return left;
}

const ExprAst& BinaryExprAst::get_right() const {
    return right;
}

CallExprAst::CallExprAst(std::string callee, std::vector<ExprAst> args) 
    : callee(std::move(callee)), args(std::move(args)) {}

const std::vector<ExprAst>& CallExprAst::get_args() const {
    return args;
}
