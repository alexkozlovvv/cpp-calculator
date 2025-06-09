#include "token.h"
#include <cmath>
#include <variant>
#include <sstream>

namespace Token{

    Constants GetDefaultConstants() {
        return {
            {"PI", M_PI}
        };
    }

    Functions GetDefaultFunctions() {
        return {
            {"sin", sin}, {"cos", cos}
        };
    }

    bool IsOperator(TokenType type) {
        return type == TokenType::PLUS || type == TokenType::MINUS ||
               type == TokenType::MULTIPLY || type == TokenType::DIVIDE ||
               type == TokenType::POWER || type == TokenType::UNARY_PLUS ||
               type == TokenType::UNARY_MINUS;
    }

}