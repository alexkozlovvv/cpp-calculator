#pragma once
#include "token.h"
#include <vector>

class Lexer {
public:
    explicit Lexer(const std::string& expression);
    std::vector<Token::Token_Param> GetTokens();
private:
    void HandleUnaryOperators(std::vector<Token::Token_Param>& tokens);
private:
    Token::Constants constants_;
    Token::Functions functions_;
    const std::string& expression_;
};