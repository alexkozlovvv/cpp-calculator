# Calculator

Реализация калькулятора на с++

## Структура проекта

```mermaid
    classDiagram
    class Calculator {
        + Calculator()
        + Calculate(const std::string& expression, const Token::Variables& vars) double
    }
    
    class Lexer {
        + Lexer(const std::string& expression)
        + GetTokens() std::vector<Token::Token_Param>
        - constants_ Token::Constants
        - functions_ Token::Functions
        - expression_ const std::string&;
    }
    
    class Parser {
        +Parser(const std::vector<Token::Token_Param>& tokens)
        +Parse() std::unique_ptr<ASTNode>
        -Peek() const Token::Token_Param&
        -Check(Token::TokenType type) bool
        -ParseExpression() std::unique_ptr<ASTNode>
        ...()
        -tokens_ const std::vector<Token::Token_Param>&
        -current_pos_ size_t;
    }
    
    namespace Token {
        class Token_Param{
            + type TokenType
            + value std::variant<double, std::string>
            + position size_t;
        }
    }

    class ASTNode {
        <<interface>>
        + Evaluate(const Token::Variables& vars)* double
    }
    class NumberNode {
        + Evaluate(const Token::Variables& vars) double
        - value_ double
    }
    class VariableNode {
        + Evaluate(const Token::Variables& vars) double
        - name_ std::string
    }
    class BinaryOpNode {
        + Evaluate(const Token::Variables& vars) double
        - operator_type_ Token::TokenType
        - left_node_ std::unique_ptr<ASTNode>
        - right_node_ std::unique_ptr<ASTNode>
    }
    class UnaryOpNode {
        + Evaluate(const Token::Variables& vars) double
        - un_operator_type_ Token::TokenType
        - operand_ std::unique_ptr<ASTNode>
    }
    class FunctionNode {
        + Evaluate(const Token::Variables& vars) double
        - name_ std::string
        - args_ std::unique_ptr<ASTNode>
    }
    
    Calculator --> Parser : dependency
    Calculator --> Lexer : dependency
    Calculator --> ASTNode : dependency
    ASTNode --> Token_Param : dependency
    Parser --> Token_Param : dependency
    Parser --> ASTNode : dependency
    Lexer --> Token_Param : dependency
    ASTNode <|.. NumberNode
    ASTNode <|.. VariableNode
    ASTNode <|.. BinaryOpNode
    ASTNode <|.. UnaryOpNode
    ASTNode <|.. FunctionNode
```
<br>

### Основные структурные элементы:
- Calculator предназначен для управления этапами вычисления; 
- Lexer производит разбивку и определение токенов входного выражения;
- Parser отвечает за создание синтакчисеского дерева в соответствии с приоритетами выполняемых операций;
- ASTNode и его реализации представляют собой узлы синтаксического дерева, которые предоставляют метод Evaluate для вычисления значений соответствующих узлов.

### Используемые инструменты
Linux:
- OC: Ubuntu 22.04
- cmake: 3.22.1
- компилятор: gcc 12.3.0 x86-64-linux-gnu

Windows:
- OC: Windows 11
- cmake: 3.22.1
- компилятор: gcc 13.2.0 x86-w64-mingw32

## Download

Скачать репозиторий можно с помощью команды:

```
git clone git@github.com:alexkozlovvv/cpp-search-server.git
```

## Usage

На вход программы должно подаваться выражение, которое необходимо вычислить. Дополнительно, если выражение предполагает использование переменных, передаются параметры, в которых указывается их значения.

Например:
```
./calculator '2 + sin(x) / {3 + cos(x)} * PI' --var x=PI
Result: 2
```

## Добавление новых функций

Для добавления новых токенов следует:
- дополнить перечень используемых токенов в файле token.h или же обновить перечень используемых констант и функций в файле token.cpp;
- при необходимости добавить проверку на наличие нового токена в процессе работы Lexer;
- добавить проверку на наличие нового токена в процессе создания синтаксического дерева

## Дальнейшее развитие

- Необходимо сделать процесс составления синтаксического дерева более гибким и простым с использованием явных приоритетов;
- Применить шаблоны проектирования (Visitor) для удобного отслеживания и управления исключениями. 

