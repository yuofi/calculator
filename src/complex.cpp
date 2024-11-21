#include <iostream>
#include <string>
#include <cmath>
#include <regex>
#include "complex.hpp"
#include <iomanip>


Complex::Complex(double re, double im) {
    this->re = re;
    this->im = im;
}

Complex::Complex(const Complex &x) {
    this->re = x.re;
    this->im = x.im;
} 

Complex::Complex(const std::string &s) {
    try {
        std::regex complex_regex(R"(^([+-]?\d*\.?\d*)\s*([+-])\s*(\d*\.?\d*)i$)");
        std::smatch matches;

        if (std::regex_match(s, matches, complex_regex)) {
            this->re = std::stod(matches[1].str());
            this->im = std::stod(matches[3].str());
            
            // Обработка знака мнимой части
            if (matches[2].str() == "-") {
                this->im = -this->im;
            }
        } else {
            // Попытка обработать другие форматы
            std::regex alternative_regex(R"(^([+-]?)?(\d*\.?\d*)i?$)");
            if (std::regex_match(s, matches, alternative_regex)) {
                if (s.find('i') != std::string::npos) {
                    this->re = 0;
                    this->im = std::stod(matches[2].str());
                    
                    if (matches[1].matched && matches[1].str() == "-") {
                        this->im = -this->im;
                    }
                } else {
                    this->re = std::stod(matches[2].str());
                    this->im = 0;
                }
            } else {
                throw std::invalid_argument("Неверный формат комплексного числа");
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        this->re = 0;
        this->im = 0;
    }
}

Complex Complex::operator+(const Complex &x) const {
    return Complex(this->re + x.re, this->im + x.im);
}

 
Complex Complex::operator+(const double& x) const {
    return Complex(this->re + static_cast<double>(x), this->im);
}

Complex Complex::operator-(const Complex &x) const {
    return Complex(this->re - x.re, this->im - x.im);
}

 
Complex Complex::operator-(const double& x) const {
    return Complex(this->re - static_cast<double>(x), this->im);
}

Complex Complex::operator* (const Complex &x) const {
   double re = this->re * x.re - this->im * x.im;
   double im = this->re * x.im + this->im * x.re;
    return Complex(re, im);
}

 
Complex Complex::operator*(const double& x) const {
    return Complex(this->re * static_cast<double>(x), this->im * static_cast<double>(x));
}


Complex Complex::operator/ (const Complex &x) const {
    if (x.re == 0 && x.im == 0) {
        std::cout << "Деление на ноль" << std::endl;
        exit(1);
    }
   double re = (this->re * x.re + this->im * x.im) / (x.re * x.re + x.im * x.im);
   double im = (this->im * x.re - this->re * x.im) / (x.re * x.re + x.im * x.im);
    return Complex(re, im);
}

 
Complex Complex::operator/(const double& x) const {
    if (x == 0 ) {
        std::cout << "Деление на ноль" << std::endl;
        exit(1);
    }

   double re = (this->re * x ) / (x * x);
   double im = (this->im * x ) / (x * x);
    return Complex(re, im);
}

 
Complex& Complex::operator+=(const double& x)  {
    *this = *this + x;
    return *this;
}

 
Complex& Complex::operator-=(const double& x)  {
    *this = *this - x;
    return *this;
}

 
Complex& Complex::operator*=(const double& x)  {
    *this = *this * x;
    return *this;
}

std::pair<double, double> Complex::trig_form(const Complex &x) const {
   double r = std::sqrt(x.re * x.re + x.im * x.im);
   double phi = std::atan2(x.im, x.re);

    return std::make_pair(r, phi);
}

 
Complex Complex::operator^ (const double& x) const {
   double r = std::sqrt(this->re * this->re + this->im * this->im);
   double phi = std::atan2(this->im, this->re);

    return Complex(std::pow(r, x) * std::cos(x * phi), std::pow(r, x) * std::sin(x * phi));
}

Complex& Complex::operator+=(const Complex &x) {
    this->re += x.re;
    this->im += x.im;
    return *this;
}

Complex& Complex::operator-=(const Complex &x) {
    this->re -= x.re;
    this->im -= x.im;
    return *this;
}

Complex& Complex::operator*=(const Complex &x) {
   double newRe = this->re * x.re - this->im * x.im;
   double newIm = this->re * x.im + this->im * x.re;
    this->re = newRe;
    this->im = newIm;
    return *this;
}

bool Complex::operator== (const Complex &x) const {
    return (std::sqrt(this->re * this->re + this->im * this->im) ==std::sqrt(x.re * x.re + x.im * x.im));
}

bool Complex::operator!= (const Complex &x) const {
    return !(*this == x);
}

std::ostream& operator<<(std::ostream &out, const Complex &x) {
    if (x.im < 0) {
        out << x.re << " " << x.im << "i";
    } else {
        out << x.re << " + " << x.im << "i";
    }
    return out;
}