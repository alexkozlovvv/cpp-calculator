#pragma once
#include <string>
#include <variant>
#include <vector>
#include <map>

namespace Token{

    enum class TokenType {
        NUMBER, 
        VARIABLE, 
        CONSTANT,
        PLUS, 
        MINUS, 
        MULTIPLY, 
        DIVIDE, 
        POWER,
        UNARY_PLUS, 
        UNARY_MINUS,
        UNARY_FACTORIAL,
        LEFT_PAREN, 
        RIGHT_PAREN,
        LEFT_BRACKET, 
        RIGHT_BRACKET,
        LEFT_BRACE, 
        RIGHT_BRACE,
        FUNCTION
    };

    struct Token_Param{
        TokenType type;
        std::variant<double, std::string> value;
        size_t position;
    };

    //using Variables = std::map<std::string, double>;
    using Variables = std::map<std::string, std::variant<double, std::string>>;
    using Constants = std::map<std::string, double>;
    using Functions = std::map<std::string, double(*)(double)>;
    using FunctionArgs = std::vector<double>;

    Constants GetDefaultConstants();
    Functions GetDefaultFunctions();
    bool IsOperator(TokenType type);

} //End of namespace Token