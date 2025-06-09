#include "ast.h"
#include "token.h"
#include <cmath>
#include <stdexcept>


double VariableNode::Evaluate(const Token::Variables& vars) const {
    auto it = vars.find(name_);
    if (it == vars.end()) {
        throw std::runtime_error("Unknown variable: " + name_);
    }
    if(std::holds_alternative<double>(it->second)){
        return std::get<double>(it->second);
    }
    std::string var_val = std::get<std::string>(it->second);
    if(Token::GetDefaultConstants().count(var_val)) {
        return Token::GetDefaultConstants()[var_val];
    }
    throw std::runtime_error("Unknown variable value: " + var_val);
}

double BinaryOpNode::Evaluate(const Token::Variables& vars) const {
    double leftVal = left_node_->Evaluate(vars);
    double rightVal = right_node_->Evaluate(vars);
    double result{0.0};
    switch(operator_type_) {
        case Token::TokenType::PLUS:
            result = std::isfinite(leftVal + rightVal) ? leftVal + rightVal :
                    throw std::runtime_error("Infinite result or Nan");
                    break; 
        case Token::TokenType::MINUS: 
            result = std::isfinite(leftVal - rightVal) ? leftVal - rightVal :
                    throw std::runtime_error("Infinite result or Nan");
                    break; 
        case Token::TokenType::MULTIPLY: 
            result = std::isfinite(leftVal * rightVal) ? leftVal * rightVal :
                    throw std::runtime_error("Infinite result or Nan");
                    break; 
        case Token::TokenType::DIVIDE: 
            result = std::isfinite(leftVal / rightVal) ? leftVal / rightVal :
                    throw std::runtime_error("Infinite result or Nan");
                    break; 
        case Token::TokenType::POWER: 
            result = std::isfinite(pow(leftVal, rightVal)) ? pow(leftVal, rightVal) :
                    throw std::runtime_error("Infinite result or Nan");
                    break;
        default: throw std::runtime_error("Unknown binary operator");
    }
    return result;
}

constexpr unsigned int MaxFactorialForDouble() {
    int n = 0;
    double fact = 1.0;
    double max_double = std::numeric_limits<double>::max();
    
    while (true) {
        if (fact > max_double / (n + 1)) {
            return n;
        }
        n++;
        fact *= n;
    }
}

double UnaryOpNode::Evaluate(const Token::Variables& vars) const {
    double val = operand_->Evaluate(vars);
    switch(un_operator_type_) {
        case Token::TokenType::UNARY_PLUS: return +val;
        case Token::TokenType::UNARY_MINUS: return -val;
        case Token::TokenType::UNARY_FACTORIAL: {
            // Проверяем, что значение целое и неотрицательное
            if (val < 0 || val != floor(val)) {
                throw std::runtime_error("Factorial is only defined for non-negative integers");
            }
            const unsigned int max_val_for_factorial = MaxFactorialForDouble();
            unsigned int n = static_cast<unsigned int>(val);
            /* Проверяем можно ли вычислить факториал числа без переполнения */
            if (n > max_val_for_factorial) {
                throw std::runtime_error("Factorial value too large");
            }
            unsigned long long result = 1;
            
            for (unsigned int i = 2; i <= n; ++i) {
                result *= i;
            }
            return static_cast<double>(result);
        }
        default: throw std::runtime_error("Unknown unary operator");
    }
}

double FunctionNode::Evaluate(const Token::Variables& vars) const {
    const Token::Functions funcs = Token::GetDefaultFunctions();
    
    auto it = funcs.find(name_);
    if (it == funcs.end()) {
        throw std::runtime_error("Unknown function: " + name_);
    }

    if (args_ == nullptr) {
        throw std::runtime_error("Function " + name_ + " expects exactly 1 argument");
    }

    double arg = args_->Evaluate(vars);
    double result = it->second(arg);
    return it->second(arg);
}