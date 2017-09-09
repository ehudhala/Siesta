#include "parser.h"

class is_closing_paren {
public:
    bool operator()(const CharToken<close_paren>&) const {
        return true;
    }

    template <class T>
    bool operator()(const T&) const {
        return false;
    }
};

class expr_parser {
public:
    expr_parser(Lexer& lexer, std::ostream& error_stream) 
        : lexer(lexer), error_stream(error_stream) {}

    optional<ExprAst> operator()(const NumberToken& token) const {
        return NumberExprAst(token.val);
    }

    optional<ExprAst> operator()(const IdentifierToken&) const {
        // TODO: implement
        return NumberExprAst(5);
    }

    optional<ExprAst> operator()(const CharToken<open_paren>&) const {
        auto inner{parse_expression(lexer, error_stream)};
        if (!inner) {
            return inner;
        }

        auto next{lexer.next_token()};
        if (!(boost::apply_visitor(is_closing_paren(), next))) {
            error_stream << "Expected ')'";
            return optional<ExprAst>();
        }

        return inner;
    }

    template <class T>
    optional<ExprAst> operator()(const T&) const {
        error_stream << "Unexpected token when expecting an expression";
        return optional<ExprAst>();
    }

private:
    Lexer& lexer;
    std::ostream& error_stream;
};

optional<ExprAst> parse_primary(Lexer& l, std::ostream& error_stream) {
    Token next(l.next_token());
    return boost::apply_visitor(expr_parser(l, error_stream), next);
}

optional<ExprAst> parse_expression(Lexer& l, std::ostream& error_stream) {
    // TODO: implement. this is only a prototype.
    return parse_primary(l, error_stream);
}
