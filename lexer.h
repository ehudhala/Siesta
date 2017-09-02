#include <string>
#include <iostream>

#include "boost/variant.hpp"

class EofToken {
};

class DefToken {
};


class ExternToken {
};

class IdentifierToken {
public:
    IdentifierToken(const std::string& identifier);

    const std::string& get_identifier() const;
private:
    std::string m_identifier;
};

class NumberToken {
public:
    NumberToken(int val);

    int get_val() const;
private:
    int m_val;
};

using Token = boost::variant<
    EofToken, 
    DefToken, 
    ExternToken, 
    IdentifierToken, 
    NumberToken>;

Token get_token(std::istream& inupt);
