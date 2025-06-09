#include "token.h"
#include "lexer.h"
#include <sstream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace Token;

Lexer::Lexer(const std::string& expression) 
    : constants_(GetDefaultConstants()),
      functions_(GetDefaultFunctions()),
      expression_(expression) {}

std::vector<Token_Param> Lexer::GetTokens() {
    std::vector<Token_Param> tokens;
    size_t pos = 0;
    size_t length = expression_.size();
    
    while (pos < length) {
        char c = expression_[pos];
        
        // Пропускаем пробелы
        if (isspace(c)) {
            pos++;
            continue;
        }
        
        // Обработка чисел
        if (isdigit(c) || c == '.') {
            size_t start = pos;
            bool has_dot = (c == '.');
            
            while (pos < length && (isdigit(expression_[pos]) || expression_[pos] == '.')) {
                if (expression_[pos] == '.') {
                    if (has_dot) {
                        // В случае если в составе числа обнаружена вторая точка
                        throw std::runtime_error("Invalid number format at position " + std::to_string(pos));
                    }
                    has_dot = true;
                }
                pos++;
            }
            
            try {
                double value = stod(expression_.substr(start, pos - start));
                tokens.push_back({TokenType::NUMBER, value, start});
            } catch (...) {
                throw std::runtime_error("Invalid number at position " + std::to_string(start));
            }
            continue;
        }
        
        // Обработка идентификаторов (переменные, функции, константы)
        if (isalpha(c) || c == '_') {
            size_t start = pos;
            while (pos < length && (isalnum(expression_[pos]) || expression_[pos] == '_')) {
                pos++;
            }
            
            std::string ident = expression_.substr(start, pos - start);
            
            if (constants_.count(ident)) {
                tokens.push_back({TokenType::CONSTANT, constants_.at(ident), start});
            } else if (functions_.count(ident)) {
                tokens.push_back({TokenType::FUNCTION, ident, start});
            } else {
                tokens.push_back({TokenType::VARIABLE, ident, start});
            }
            continue;
        }
        
        // Обработка операторов
        switch (c) {
            case '+':
                tokens.push_back({TokenType::PLUS, std::string(1, c), pos});
                pos++;
                break;
            case '-':
                tokens.push_back({TokenType::MINUS, std::string(1, c), pos});
                pos++;
                break;
            case '*':
                tokens.push_back({TokenType::MULTIPLY, std::string(1, c), pos});
                pos++;
                break;
            case '/':
                tokens.push_back({TokenType::DIVIDE, std::string(1, c), pos});
                pos++;
                break;
            case '^':
                tokens.push_back({TokenType::POWER, std::string(1, c), pos});
                pos++;
                break;
            case '!':
                tokens.push_back({TokenType::UNARY_FACTORIAL, std::string(1, c), pos});
                pos++;
                break;
                
            // Обработка скобок
            case '(':
                tokens.push_back({TokenType::LEFT_PAREN, std::string(1, c), pos});
                pos++;
                break;
            case ')':
                tokens.push_back({TokenType::RIGHT_PAREN, std::string(1, c), pos});
                pos++;
                break;
            case '[':
                tokens.push_back({TokenType::LEFT_BRACKET, std::string(1, c), pos});
                pos++;
                break;
            case ']':
                tokens.push_back({TokenType::RIGHT_BRACKET, std::string(1, c), pos});
                pos++;
                break;
            case '{':
                tokens.push_back({TokenType::LEFT_BRACE, std::string(1, c), pos});
                pos++;
                break;
            case '}':
                tokens.push_back({TokenType::RIGHT_BRACE, std::string(1, c), pos});
                pos++;
                break;
                
            default:
                throw std::runtime_error("Unknown character '" + std::string(1, c) + 
                                 "' at position " + std::to_string(pos));
        }
    }
    // Дополнительный проход по токенам с целью поиска префиксных унарных операторов
    HandleUnaryOperators(tokens);
    return tokens;
}

void Lexer::HandleUnaryOperators(std::vector<Token_Param>& tokens) {
    if (tokens.empty()) return;
    
    // Первый токен может быть унарным + или -
    if (tokens[0].type == TokenType::PLUS) {
        tokens[0].type = TokenType::UNARY_PLUS;
    } else if (tokens[0].type == TokenType::MINUS) {
        tokens[0].type = TokenType::UNARY_MINUS;
    }
    
    // Обработка остальных токенов
    for (size_t i = 1; i < tokens.size(); i++) {
        TokenType prev_type = tokens[i-1].type;
        
        // Если текущий токен + или -, а предыдущий - оператор или открывающая скобка
        if ((tokens[i].type == TokenType::PLUS || tokens[i].type == TokenType::MINUS) &&
            (IsOperator(prev_type) || 
             prev_type == TokenType::LEFT_PAREN ||
             prev_type == TokenType::LEFT_BRACKET ||
             prev_type == TokenType::LEFT_BRACE)) {
            
            tokens[i].type = (tokens[i].type == TokenType::PLUS) ? 
                TokenType::UNARY_PLUS : TokenType::UNARY_MINUS;
        }
    }
}