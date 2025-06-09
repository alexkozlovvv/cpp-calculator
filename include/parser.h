#pragma once
#include "token.h"
#include "ast.h"
#include <vector>
#include <memory>

class Parser {
public:
    Parser(const std::vector<Token::Token_Param>& tokens);
    std::unique_ptr<ASTNode> Parse();
    
private:
    const std::vector<Token::Token_Param>& tokens_;
    size_t current_pos_ = 0;
    
    const Token::Token_Param& Peek() const;
    const Token::Token_Param& Advance();
    bool Match(Token::TokenType type);
    void Match(Token::TokenType type, const std::string& message);
    bool Check(Token::TokenType type) const;
    bool IsAtEnd() const;
    
    std::unique_ptr<ASTNode> ParseExpression();
    std::unique_ptr<ASTNode> ParsePrimaryExpr();
    std::unique_ptr<ASTNode> ParseUnaryOp();
    std::unique_ptr<ASTNode> ParsePowerOp();
    std::unique_ptr<ASTNode> ParseMultiplicativeOp();
    std::unique_ptr<ASTNode> ParseAdditiveOp();
    std::unique_ptr<ASTNode> ParseFunctionCall(const std::string& name);
    
    bool IsMatchingBracket(Token::TokenType open, Token::TokenType close) const;
};