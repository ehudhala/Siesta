#include "ast.h"

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
    
class ast_equals_visitor {
public:
    template <class T, class U, 
             typename std::enable_if<!std::is_same<T, U>{}, int>::type = 0>
    bool operator()(const T&, const U&) const {
        return false;
    }

    bool operator()(const NumberExprAst& lhs, const NumberExprAst& rhs) const {
        return lhs.val == rhs.val;
    }

    bool operator()(const VariableExprAst& lhs, const VariableExprAst& rhs) const {
        return lhs.name == rhs.name;
    }

    bool operator()(const BinaryExprAst& lhs, const BinaryExprAst& rhs) const {
        return (lhs.op == rhs.op && 
                lhs.left == rhs.left && 
                lhs.right == rhs.right);
    }

    bool operator()(const CallExprAst& lhs, const CallExprAst& rhs) const {
        return (lhs.callee == rhs.callee && lhs.args == rhs.args);
    }

    bool operator()(const PrototypeAst& lhs, const PrototypeAst& rhs) const {
        return (lhs.name == rhs.name && lhs.args == rhs.args);
    }

    bool operator()(const FunctionAst& lhs, const FunctionAst& rhs) const {
        return (lhs.proto == rhs.proto && lhs.body == rhs.body);
    }
};

bool operator==(const Ast& lhs, const Ast& rhs) {
    return boost::apply_visitor(ast_equals_visitor(), lhs, rhs);
}
