#include "parser.h"

class expr_parser {
public:
    optional<ExprAst> operator()(const IdentifierToken&) const {
        return NumberExprAst(5);
    }

    template <class T>
    optional<ExprAst> operator()(const T&) const {
        return optional<ExprAst>();
    }
};

optional<ExprAst> parse_primary(Lexer& l) {
    Token next(l.next_token());
    return boost::apply_visitor(expr_parser(), next);
}
