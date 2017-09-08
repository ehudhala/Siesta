#pragma once

#include <string>
#include <vector>

#include "boost/variant.hpp"

class NumberExprAst;
class VariableExprAst;
class BinaryExprAst;
class CallExprAst;
class PrototypeAst;

using ExprAst = boost::variant<
    NumberExprAst, 
    VariableExprAst,
    boost::recursive_wrapper<BinaryExprAst>,
    boost::recursive_wrapper<CallExprAst>>;

class NumberExprAst {
public:
    NumberExprAst(double val);

    double val;
};

class VariableExprAst {
public:
    VariableExprAst(std::string name);

    std::string name;
};

class BinaryExprAst {
public:
    BinaryExprAst(char op, ExprAst left, ExprAst right);

    const ExprAst& get_left() const;
    const ExprAst& get_right() const;

    char op;
private:
    ExprAst left, right;
};

class CallExprAst {
public:
    CallExprAst(std::string callee, std::vector<ExprAst> args);

    const std::vector<ExprAst>& get_args() const;

    std::string callee;
private:
    std::vector<ExprAst> args;
};

class PrototypeAst {
public:

};
