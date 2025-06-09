#include "parser.h"
#include <stdexcept>
#include <memory>

using namespace Token;

Parser::Parser(const std::vector<Token_Param>& tokens) : tokens_(tokens) {}

std::unique_ptr<ASTNode> Parser::Parse() {
    try {
        return ParseExpression();
    } catch (const std::exception& e) {
        throw std::runtime_error("Parse error: " + std::string(e.what()));
    }
}

const Token_Param& Parser::Peek() const {
    return tokens_[current_pos_];
}

const Token_Param& Parser::Advance() {
    if (!IsAtEnd()) current_pos_++;
    return tokens_[current_pos_ - 1];
}

bool Parser::Match(TokenType type) {
    if (Check(type)) {
        Advance();
        return true;
    }
    return false;
}

void Parser::Match(TokenType type, const std::string& message) {
    if (Check(type)) {
        Advance();
        return;
    }
    throw std::runtime_error(message);
}

bool Parser::Check(TokenType type) const {
    if (IsAtEnd()) return false;
    return Peek().type == type;
}

bool Parser::IsAtEnd() const {
    return current_pos_ >= tokens_.size();
}
/*
    Парсинг строится на вызове функций:
    ParseAdditiveOp --> ParseMultiplicativeOp --> ParsePowerOp --> ParseUnaryOp
    --> ParsePrimaryExpr --> ParseFunctionCall --> ParseAdditiveOp...
    
    каждая функция сначала (если нет нужды обрабатывать префиксный оператор) 
    проваливается на уровень ниже и определяет
    более приоритетные узлы синтаксического дерева
    
    если более приоритетные узлы были найдены они добавляются в узел менее приоритетный

    Приоритеты (чем ниже тем выше приоритет):
    +, -
    *, /
    ^
    унарные операторы(-, +, !)
    числа, переменные, константы, функции, скобки
*/
std::unique_ptr<ASTNode> Parser::ParseExpression() {
    return ParseAdditiveOp();
}

std::unique_ptr<ASTNode> Parser::ParseAdditiveOp() {
    auto expr = ParseMultiplicativeOp();
    
    while (Check(TokenType::PLUS) || Check(TokenType::MINUS)) {
        Token_Param op = tokens_[current_pos_];
        Advance();
        auto right = ParseMultiplicativeOp();
        expr = std::make_unique<BinaryOpNode>(op.type, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::ParseMultiplicativeOp() {
    auto left = ParsePowerOp();
    
    while (Check(TokenType::MULTIPLY) || Check(TokenType::DIVIDE)) {
        Token_Param op = tokens_[current_pos_];
        Advance();
        auto right = ParsePowerOp();
        left = std::make_unique<BinaryOpNode>(op.type, std::move(left), std::move(right));
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::ParsePowerOp() {
    auto left = ParseUnaryOp();

    while (Check(TokenType::POWER)) {
        Token_Param op = tokens_[current_pos_];
        Advance();
        auto right = ParseUnaryOp();
        left = std::make_unique<BinaryOpNode>(op.type, std::move(left), move(right));
    }
    
    return left;
}

std::unique_ptr<ASTNode> Parser::ParseUnaryOp() {
    // Обработка унарных префиксных операторов
    if (Check(TokenType::UNARY_PLUS) || Check(TokenType::UNARY_MINUS)) {
        Token_Param op = tokens_[current_pos_];
        Advance();
        auto operand = ParsePrimaryExpr();
        return std::make_unique<UnaryOpNode>(op.type, std::move(operand));
    }

    auto primary = ParsePrimaryExpr();

    // Обработка постфиксного оператора факториала
    while (Check(TokenType::UNARY_FACTORIAL)) {
        Token_Param op = tokens_[current_pos_];
        Advance();
        primary = std::make_unique<UnaryOpNode>(op.type, std::move(primary));
    }
    
    return primary;
}

std::unique_ptr<ASTNode> Parser::ParseFunctionCall(const std::string& name) {
    Match(TokenType::LEFT_PAREN, "Expected '(' after function name");
    
    std::unique_ptr<ASTNode> args;
    if (!Check(TokenType::RIGHT_PAREN)) {
        // в случае если будет несколько аргументов
        args = std::move(ParseExpression());
    }
    
    Match(TokenType::RIGHT_PAREN, "Expected ')' after function arguments");
    
    return std::make_unique<FunctionNode>(name, move(args));
}

std::unique_ptr<ASTNode> Parser::ParsePrimaryExpr() {
    if (Check(TokenType::NUMBER)) {
        double value = std::get<double>(tokens_[current_pos_].value);
        Advance();
        return std::make_unique<NumberNode>(value);
    }
    
    if (Check(TokenType::VARIABLE)) {
        std::string name = std::get<std::string>(tokens_[current_pos_].value);
        Advance();
        return std::make_unique<VariableNode>(name);
    }
    
    if (Check(TokenType::CONSTANT)) {
        double value = std::get<double>(tokens_[current_pos_].value);
        Advance();
        return std::make_unique<NumberNode>(value);
    }
    
    if (Check(TokenType::FUNCTION)) {
        std::string name = std::get<std::string>(tokens_[current_pos_].value);
        Advance();
        return ParseFunctionCall(name);
    }
    
    if (Check(TokenType::LEFT_PAREN) || Check(TokenType::LEFT_BRACKET) || Check(TokenType::LEFT_BRACE)) {
        TokenType bracket_type = Peek().type;
        Advance(); // Пропускаем открывающую скобку
        
        auto expr = ParseExpression();
        
        if (!IsMatchingBracket(bracket_type, Peek().type)) {
            throw std::runtime_error("Mismatched brackets");
        }
        Advance(); // Пропускаем закрывающую скобку
        
        return expr;
    }
    
    throw std::runtime_error("Unexpected token at position " + std::to_string(Peek().position));
}

bool Parser::IsMatchingBracket(TokenType open, TokenType close) const {
    return (open == TokenType::LEFT_PAREN && close == TokenType::RIGHT_PAREN) ||
           (open == TokenType::LEFT_BRACKET && close == TokenType::RIGHT_BRACKET) ||
           (open == TokenType::LEFT_BRACE && close == TokenType::RIGHT_BRACE);
}

