#include "parser.h"

template <class char_>
class is_char_visitor {
public:
    bool operator()(const CharToken<char_>&) const {
        return true;
    }

    template <class T>
    bool operator()(const T&) const {
        return false;
    }
};

template <class char_>
bool is_char(const Token& token) {
    return boost::apply_visitor(is_char_visitor<char_>(), token);
}

class expr_parser {
public:
    expr_parser(Lexer& lexer, std::ostream& error_stream) 
        : lexer(lexer), error_stream(error_stream) {}

    optional<ExprAst> operator()(const NumberToken& token) const {
        NumberExprAst expr(token.val);
        lexer.next_token(); // Eat number;
        return expr;
    }

    optional<ExprAst> operator()(const CharToken<open_paren>&) const {
        lexer.next_token(); // Eat '('
        auto inner{parse_expression(lexer, error_stream)};
        if (!inner) {
            return inner;
        }

        if (!is_char<close_paren>(lexer.curr_token)) {
            error_stream << "Expected ')'" << std::endl;
            return optional<ExprAst>();
        }

        lexer.next_token(); // Eat ')'
        return inner;
    }

    optional<ExprAst> operator()(const IdentifierToken& token) const {
        std::string name = token.get_identifier();

        auto next{lexer.next_token()};
        if (!is_char<open_paren>(next)) {
            return VariableExprAst(name);
        }

        std::vector<ExprAst> args;

        lexer.next_token(); // Eat '('. now either ')' or arg.

        if (is_char<close_paren>(lexer.curr_token))
            return CallExprAst(name, std::move(args));

        // TODO: refactor!
        while (!is_char<close_paren>(lexer.curr_token)) {
            if (auto arg = parse_expression(lexer, error_stream))
                args.push_back(*arg);
            else
                return arg;

            if ((is_char<close_paren>(lexer.curr_token)))
                break;

            if (!is_char<comma>(lexer.curr_token)) {
                error_stream << "Expected ',' or ')' in argument list" << std::endl;
                return optional<ExprAst>();
            }
            lexer.next_token(); // Eat ','.
        }

        lexer.next_token(); // Eat ')'.

        return CallExprAst(name, std::move(args));
    }

    template <class T>
    optional<ExprAst> operator()(const T&) const {
        error_stream << "Unexpected token when expecting an expression" << std::endl;
        return optional<ExprAst>();
    }

private:
    Lexer& lexer;
    std::ostream& error_stream;
};

optional<ExprAst> parse_primary(Lexer& l, std::ostream& error_stream) {
    return boost::apply_visitor(expr_parser(l, error_stream), l.curr_token);
}

optional<ExprAst> parse_expression(Lexer& l, std::ostream& error_stream) {
    // TODO: implement. this is only a prototype.
    return parse_primary(l, error_stream);
}
