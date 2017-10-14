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

        // TODO: refactor!
        while (!is_char<close_paren>(lexer.curr_token)) {
            if (auto arg = parse_expression(lexer, error_stream))
                args.push_back(*arg);
            else
                return arg;

            if (is_char<close_paren>(lexer.curr_token))
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

std::map<char, int> get_prec_map() {
    return std::map<char, int>{
        {'<', 1},
        {'>', 1},
        {'+', 2},
        {'-', 2},
        {'*', 3},
        {'/', 3},
    };
}

optional<int> get_bin_op_precedence(const Token& token, const std::map<char, int>& prec_map) {
    auto token_char = get_char(token);
    if (!token_char) {
        return optional<int>{};
    }
    auto prec = prec_map.find(*token_char);
    if (prec == prec_map.end()) {
        return optional<int>{};
    }
    return prec->second;
}

/**
 * lhs_prec - precedence of left and prev. 
 * curr_prec - precedence of left and right. 
 * rhs_prec - precedence of right and next. 
 * We have three cases:
 *      curr < lhs
 *           We dropped precedence, as in x * y _+_ z.
 *           We return the current lhs.
 *           This will happen at the end of the recursion.
 *      lhs < curr && curr < rhs
 *           We have more precedence than the next, as in x _*_ y + z.
 *           We return a binary operation.
 *      lhs < curr && curr > rhs
 *           We have less precedence than the next, as in x _*_ y / z.
 *           We recurse to parse the right hand side as a binary operation too.
 */
optional<ExprAst> parse_bin_op_rhs(int lhs_prec, ExprAst lhs,
        Lexer& l, std::ostream& error_stream) {
    // TODO: refactor and add tests!!!
    auto curr_prec = get_bin_op_precedence(l.curr_token);
    if (!curr_prec || *curr_prec < lhs_prec)
        return lhs;
    auto op = *get_char(l.curr_token); // op is checked in curr_prec already.
    l.next_token();

    auto rhs = parse_primary(l, error_stream);
    if (!rhs)
        return rhs;

    auto rhs_prec = get_bin_op_precedence(l.curr_token);
    if (rhs_prec && curr_prec < *rhs_prec) {
        rhs = parse_bin_op_rhs(*curr_prec + 1, *rhs, l, error_stream);
        if (!rhs)
            return rhs;
    }
    lhs = BinaryExprAst(op, lhs, *rhs);
    return parse_bin_op_rhs(lhs_prec, lhs, l, error_stream);
}

optional<ExprAst> parse_expression(Lexer& l, std::ostream& error_stream) {
    auto lhs = parse_primary(l, error_stream);
    if (!lhs) {
        return lhs;
    }

    return parse_bin_op_rhs(0, *lhs, l, error_stream);
}
