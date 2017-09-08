#include "parser.h"

NumberExprAst::NumberExprAst(double val) : val(val) {}

VariableExprAst::VariableExprAst(std::string name) : name(std::move(name)) {}

BinaryExprAst::BinaryExprAst(char op, ExprAst left, ExprAst right)
    : op(op), left(std::move(left)), right(std::move(right)) {}

CallExprAst::CallExprAst(std::string callee, std::vector<ExprAst> args) 
    : callee(std::move(callee)), args(std::move(args)) {}

PrototypeAst::PrototypeAst(std::string name, std::vector<std::string> args)
    : name(std::move(name)), args(std::move(args)) {}

FunctionAst::FunctionAst(PrototypeAst proto, ExprAst body)
    : proto(std::move(proto)), body(std::move(body)) {}
