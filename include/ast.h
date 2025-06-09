#pragma once
#include "token.h"
#include <memory>

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual double Evaluate(const Token::Variables& vars) const = 0;
};

class NumberNode : public ASTNode {
    double value_;
public:
    NumberNode(double val) : value_(val) {}
    double Evaluate(const Token::Variables& vars) const override { return value_; }
};

class VariableNode : public ASTNode {
    std::string name_;
public:
    VariableNode(const std::string& name) : name_(name) {}
    double Evaluate(const Token::Variables& vars) const override;
};

class BinaryOpNode : public ASTNode {
    Token::TokenType operator_type_;
    std::unique_ptr<ASTNode> left_node_, right_node_;
public:
    BinaryOpNode(Token::TokenType operator_type, std::unique_ptr<ASTNode> left_node, std::unique_ptr<ASTNode> right_node)
        : operator_type_(operator_type), left_node_(std::move(left_node)), right_node_(std::move(right_node)) {}
    double Evaluate(const Token::Variables& vars) const override;
};

class UnaryOpNode : public ASTNode {
    Token::TokenType un_operator_type_;
    std::unique_ptr<ASTNode> operand_;
public:
    UnaryOpNode(Token::TokenType un_operator_type, std::unique_ptr<ASTNode> opnd)
        : un_operator_type_(un_operator_type), operand_(std::move(opnd)) {}
    double Evaluate(const Token::Variables& vars) const override;
};

class FunctionNode : public ASTNode {
    std::string name_;
    std::unique_ptr<ASTNode> args_;
public:
    FunctionNode(const std::string& name, std::unique_ptr<ASTNode> arg_expr)
        : name_(name), args_(std::move(arg_expr)) {}
    double Evaluate(const Token::Variables& vars) const override;
};