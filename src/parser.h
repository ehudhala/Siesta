#include <experimental/optional>

#include "boost/variant.hpp"

#include "ast.h"
#include "lexer.h"

using std::experimental::optional;

optional<ExprAst> parse_primary(Lexer&, std::ostream& error_stream);
