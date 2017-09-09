#include "parser.h"

class expr_parser {
public:
    expr_parser(std::ostream& error_stream) : error_stream(error_stream) {}

    optional<ExprAst> operator()(const NumberToken& token) const {
        return NumberExprAst(token.val);
    }

    optional<ExprAst> operator()(const IdentifierToken&) const {
        // TODO: implement
        return NumberExprAst(5);
    }

    optional<ExprAst> parse_paren_expr(const CharToken&) const {
        // TODO: implement
        return NumberExprAst(5);
    }

    optional<ExprAst> operator()(const CharToken& token) const {
        switch (token.val) {
        default:
            return unexpected_token();
        case '(':
            return parse_paren_expr(token);
        }
    }

    template <class T>
    optional<ExprAst> operator()(const T&) const {
        return unexpected_token();
    }

    optional<ExprAst> unexpected_token() const {
        error_stream << "Unexpected token when expecting an expression";
        return optional<ExprAst>();
    }

private:
    std::ostream& error_stream;
};

optional<ExprAst> parse_primary(Lexer& l, std::ostream& error_stream) {
    Token next(l.next_token());
    return boost::apply_visitor(expr_parser(error_stream), next);
}
