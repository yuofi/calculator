#include <iostream>
#include <stack>
#include <queue>
#include <string>
#include <sstream>
#include <cmath>
#include <regex>
#include <map>

#include "big_int.hpp"
#include "complex.hpp"

enum Method {
            DOUBLE = 0,
            BIG_INT = 1,
            COMPLEX = 2
        };

class Calculator {
    public:

    Method determine_type(const std::string& str) {
    if (str.find('i') != std::string::npos) {
        return COMPLEX; 
    }

    std::regex hex_pattern("[xXabcdefABCDEF]");
    if (std::regex_search(str, hex_pattern)) {
        return BIG_INT;
    }

    return DOUBLE;
}

    int priority(const std::string& op) {
        if (op == "+" || op == "-") {
            return 1;
        } else if (op == "*" || op == "/") {
            return 2;
        } else if (op == "**") {
            return 3;
        }
        return 0;
    }

    std::string sorting_station(const std::string& maths) {
        std::stack<std::string> stack;
        std::queue<std::string> output;
        std::string current;
        bool lastOperator = true;
        bool isHex = false;    

        for (size_t i = 0; i < maths.length(); ++i) {
            char c = maths[i];

            if (std::isxdigit(c)) {
                current += c;
                lastOperator = false;
            } 
            else if (std::isdigit(c) || c == '.' || (c == '-' && lastOperator)) {
                if (!isHex) {
                    current += c;
                    lastOperator = false;
                }
            }
            else if (c == 'i') {
                if (current.empty()) {
                    current = "1";
                }
                current += "i";
                output.push(current + "|");
                current.clear();
                lastOperator = false;
            } 
            else if (c == '(') {
                stack.push("(");
                lastOperator = true;
            } else if (c == ')') {
                if (!current.empty()) {
                    output.push(current + "|");
                    current.clear();
                }
                while (!stack.empty() && stack.top() != "(") {
                    output.push(stack.top() + "|");
                    stack.pop();
                }
                if (stack.empty()) {
                    throw std::runtime_error("Несоответствие скобок в выражении.");
                }
                stack.pop();
                lastOperator = false;
            } else if (c == '+' || c == '-' || c == '*' || c == '/') {
                if (!current.empty()) {
                    output.push(current + "|");
                    current.clear();
                }
                std::string op(1, c);
                while (!stack.empty() && priority(stack.top()) >= priority(op)) {
                    output.push(stack.top() + "|");
                    stack.pop();
                }
                stack.push(op);
                isHex = false;
                lastOperator = true;
            } else if (!std::isspace(c)) {
                throw std::runtime_error("Недопустимый символ в выражении: " + std::string(1, c));
            }
        }

        if (!current.empty()) {
            output.push(current + "|");
        }

        while (!stack.empty()) {
            if (stack.top() == "(") {
                throw std::runtime_error("Несоответствие скобок в выражении.");
            }
            output.push(stack.top() + "|");
            stack.pop();
        }

        std::string postfix;
        while (!output.empty()) {
            postfix += output.front();
            output.pop();
        }

        return postfix;
    }


    std::string make_input(const std::string str) {
        std::string input = "";
        std::stringstream ss(str);
        std::string token;
        while (ss >> token) {
            input += token;
        }
        return input;
    }

template <typename T>
T calculate_postfix(const std::string& maths) {
    std::stack<T> stack;
    std::stringstream ss(maths);
    std::string token;

    while (std::getline(ss, token, '|')) {
        if (std::isdigit(token[0]) || 
            (token[0] == '-' && token.length() > 1) || 
            token.find('i') != std::string::npos || 
            std::isxdigit(token[0])) {
            
            try {
                if constexpr (std::is_same_v<T, double> || std::is_same_v<T, long double>) {
                    stack.push(std::stod(token));
                } else if constexpr (std::is_same_v<T, BigInt>) {
                    stack.push(BigInt(token));
                } else if constexpr (std::is_same_v<T, Complex>) {
                    stack.push(Complex(token));
                }
            } catch (const std::exception& e) {
                std::cerr << "Ошибка преобразования числа: " << token << " (" << e.what() << ")" << std::endl;
                return T(-1);
            }

        } else { // Это оператор
            if (stack.size() < 2) {
                std::string error = "Ошибка: недостаточно операндов для операции: " + token;
                throw std::runtime_error(error);
            }

            T b = stack.top();
            stack.pop();
            T a = stack.top();
            stack.pop();

            try {
                if (token == "+") {
                    stack.push(a + b);
                } else if (token == "-") {
                    stack.push(a - b);
                } else if (token == "*") {
                    stack.push(a * b);
                } else if (token == "/") {
                    if (b != 0) {
                        stack.push(a / b);
                    } else {
                        std::cerr << "Ошибка: деление на ноль!" << std::endl;
                        return T(-1);
                    }
                } else if (token == "**") {
                    if constexpr (std::is_same_v<T, double> || std::is_same_v<T, long double>) {
                        stack.push(static_cast<T>(pow(a, b)));
                    } else {
                        throw (std::runtime_error("Ошибка: возведение в степень для этого типа не поддерживается"));
                    }
                } else {
                    std::string error = "Ошибка: неизвестный оператор: " + token;
                    throw std::runtime_error(error);
                }
            } catch (const std::exception& e) {
                std::cerr << "Ошибка при выполнении операции: " << token << " (" << e.what() << ")" << std::endl;
                return T(-1);
            }
        }
    }

    if (stack.size() != 1) {
        throw (std::runtime_error("Ошибка: в стеке осталось несколько элементов!"));
    }

    return stack.top();
}

std::string remove_hex(const std::string& input) {
    std::regex hex_pattern("0[xX]");
    return std::regex_replace(input, hex_pattern, "");
}

};


int main() {
    Calculator calc;
    std::map<int, std::string> methods = {
        {0, "DOUBLE"},
        {1, "BIG_INT, output in hex string"},
        {2, "COMPLEX"}
    }; 
   std::string maths;
    while (1) {
        std::cout << ">>> ";
        getline(std::cin, maths);
        Method m = calc.determine_type(maths);
        
        if (m == BIG_INT) {
            maths = calc.remove_hex(maths);
        }
        
        std::string result = calc.sorting_station(maths);
        std::cout << "postfix: " << result << std::endl;

        if (m == BIG_INT) {
            std::cout << calc.calculate_postfix<BigInt>(result) << std::endl;
        }
        else if (m == COMPLEX) {
            std::cout << calc.calculate_postfix<Complex>(result) << std::endl;
        }
        else if (m == DOUBLE) {
            std::cout << calc.calculate_postfix<double>(result) << std::endl;
        }
        std::cout << "method: " << methods[m] << std::endl;
    } 
    return 0;
}




