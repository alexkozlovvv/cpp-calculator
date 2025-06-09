#include "calculator.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include <string>


double Calculator::Calculate(const std::string& expression, 
    const std::map<std::string, std::variant<double, std::string>>& vars) {
    
    /* Разбивка входной строки выражения на токены */
    Lexer lexer(expression);
    auto tokens = lexer.GetTokens();
    /* Формирование абстрактного синтаксического дерева */
    Parser parser(tokens);
    auto ast = parser.Parse();
    /* Вычисление значения */
    return ast->Evaluate(vars);
}
