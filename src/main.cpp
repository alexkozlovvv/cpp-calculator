#include "../include/CLI11.hpp"
#include <iostream>
#include <string>
#include <calculator.h>

using namespace std::string_literals;

std::map<std::string, std::variant<double, std::string>> ParseVariables(std::vector<std::string>& raw_vars){
    std::map<std::string, std::variant<double, std::string>> result_vars{};
    if(!raw_vars.empty()) {
        for(const auto& arg : raw_vars) {
            std::string pair = arg;
            size_t eq_pos = pair.find('=');
            if(eq_pos == std::string::npos || eq_pos == 0 || eq_pos == pair.size()-1) {
                throw std::runtime_error("Invalid variable format: "s + pair);
            } else {
                std::string name = pair.substr(0, eq_pos);
                if(isdigit(pair.at(eq_pos + 1))) {
                    double value{0.0};
                    try {
                        size_t var_symbols = pair.substr(eq_pos + 1).size();
                        size_t var_digit_count{0};
                        value = std::stod(pair.substr(eq_pos + 1), &var_digit_count);
                        if(var_digit_count != var_symbols) {
                            throw std::runtime_error("Invalid variable format: "s + pair);
                        }
                    } catch(const std::exception& e) {
                        throw std::runtime_error("Invalid number format in: "s + pair);
                    }
                    result_vars[name] = value;
                } else {
                    result_vars[name] = pair.substr(eq_pos + 1);
                }
            }
        }
    }
    return result_vars;
}

int main(int argc, char** argv) {
    CLI::App app{"Calculator"};
    // Используем библиотеку CLI11 для парсинга выражения и переменных
    std::string expression;
    app.add_option("expression", expression, "Mathematical expression to evaluate")->required()->expected(1);
    std::vector<std::string>raw_vars;
    app.add_option("--var, -v", raw_vars, "Variable values (e.g., --var x=1.0 y=2.0)");

    CLI11_PARSE(app, argc, argv);

    try{
        std::map<std::string, std::variant<double, std::string>> variables;
        variables = ParseVariables(raw_vars);
        Calculator calc;
        double result = calc.Calculate(expression, variables);
        std::cout << "Result: " << result << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

    