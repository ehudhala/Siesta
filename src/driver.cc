#include <iostream>

#include "lexer.h"
#include "parser.h"

class is_eof {
public:
    bool operator()(const EofToken&) const {
        return true;
    }

    template <class T>
    bool operator()(const T&) const {
        return false;
    }
};

class parse_top_level {
public:
    parse_top_level(Lexer& lexer, std::ostream& error_stream, std::ostream& out_stream) 
        : lexer(lexer), error_stream(error_stream), out_stream(out_stream) {}

    void operator()(const CharToken<chars::semicolon>&) const {
        lexer.next_token();
    }

    void operator()(const DefToken&) const {
        handle_parse(bool(parse_definition(lexer, error_stream)),
        "Parsed a function definition");
    }

    void operator()(const ExternToken&) const {
        handle_parse(bool(parse_extern(lexer, error_stream)),
        "Parsed an extern");
    }

    template <class T>
    void operator()(const T&) const {
        handle_parse(bool(parse_top_level_expr(lexer, error_stream)),
        "Parsed a top-level expression");
    }

private:
    void handle_parse(bool parse_result, std::string success_message) const {
        if (parse_result) {
            out_stream << success_message << std::endl;
        }
        else {
            lexer.next_token();
        }
    }

    Lexer& lexer;
    std::ostream& error_stream, &out_stream;
};

//TODO: check what if i input a single char
static void driver_loop(Lexer& l) {
    while (!boost::apply_visitor(is_eof(), l.curr_token)) {
        std::cout << "> ";
        boost::apply_visitor(parse_top_level(l, std::cout, std::cout), l.curr_token);
    }
}

int main() {
    std::cout << "> ";
    Lexer l{std::cin};
    driver_loop(l);
}
