#pragma once

#include <string>
#include <vector>

#include "boost/variant.hpp"

class NumberExprAst;
class VariableExprAst;
class BinaryExprAst;
class CallExprAst;
class PrototypeAst;
class FunctionAst;

using ExprAst = boost::variant<
    NumberExprAst, 
    VariableExprAst,
    boost::recursive_wrapper<BinaryExprAst>,
    boost::recursive_wrapper<CallExprAst>>;

using Ast = boost::variant<
    NumberExprAst, 
    VariableExprAst,
    boost::recursive_wrapper<BinaryExprAst>,
    boost::recursive_wrapper<CallExprAst>,
    boost::recursive_wrapper<PrototypeAst>,
    boost::recursive_wrapper<FunctionAst>>;

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

    char op;
    ExprAst left, right;
};

class CallExprAst {
public:
    CallExprAst(std::string callee, std::vector<ExprAst> args);

    std::string callee;
    std::vector<ExprAst> args;
};

class PrototypeAst {
public:
    PrototypeAst(std::string name, std::vector<std::string> args);

    std::string name;
    std::vector<std::string> args;
};

class FunctionAst {
public:
    FunctionAst(PrototypeAst proto, ExprAst body);

    PrototypeAst proto;
    ExprAst body;
};
