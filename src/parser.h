#include <map>
#include <experimental/optional>

#include "boost/variant.hpp"

#include "ast.h"
#include "lexer.h"

using std::experimental::optional;

using namespace chars;
using div_ = chars::div;

template <class char_>
bool is_char(const Token& token);

optional<ExprAst> parse_primary(Lexer& l, std::ostream& error_stream);

optional<ExprAst> parse_bin_op_rhs(int lhs_prec, ExprAst lhs, 
        Lexer& l, std::ostream& error_stream);

optional<ExprAst> parse_expression(Lexer& l, std::ostream& error_stream);

std::map<char, int> get_prec_map();

static std::map<char, int> operator_prec_map = get_prec_map();

optional<int> get_bin_op_precedence(const Token& token, const std::map<char, int>& prec_map=operator_prec_map);

optional<PrototypeAst> parse_prototype(Lexer& l, std::ostream& error_stream);
optional<FunctionAst> parse_definition(Lexer& l, std::ostream& error_stream);
