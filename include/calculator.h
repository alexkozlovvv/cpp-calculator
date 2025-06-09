#pragma once
#include "token.h"
#include <string>

class Calculator {
public:
    Calculator() {};
    double Calculate(const std::string& expression, const Token::Variables& vars = {});
};