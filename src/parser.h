#include <experimental/optional>

#include "boost/variant.hpp"

#include "ast.h"
#include "lexer.h"

using std::experimental::optional;

template <class char_>
bool is_char(const Token& token);

optional<ExprAst> parse_primary(Lexer&, std::ostream& error_stream);

optional<ExprAst> parse_expression(Lexer&, std::ostream& error_stream);
