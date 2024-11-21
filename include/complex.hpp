#ifndef COMPLEX_HPP
#define COMPLEX_HPP
 
#include <string>  
#include <stdexcept>
#include <regex>
#include "big_int.hpp"

using std::string;

// template <typename T>
// class INumbers {
// protected:
//     virtual ~INumbers() = default;

// public:
//     virtual T operator+(const T& other) const = 0;
//     virtual T operator-(const T& other) const = 0;
//     virtual T operator*(const T& other) const = 0;
//     virtual T operator/(const T& other) const = 0;

//     virtual T& operator+=(const T& other) = 0;
//     virtual T& operator-=(const T& other) = 0;
//     virtual T& operator*=(const T& other) = 0;

//     virtual bool operator<(const T& other) const = 0;
//     virtual bool operator>(const T& other) const = 0;
//     virtual bool operator==(const T& other) const = 0;
//     virtual bool operator!=(const T& other) const = 0;
//     virtual bool operator<=(const T& other) const = 0;
//     virtual bool operator>=(const T& other) const = 0;

//     virtual T operator+(const double& other) const = 0;
//     virtual T operator-(const double& other) const = 0;
//     virtual T operator*(const double& other) const = 0;
//     virtual T operator/(const double& other) const = 0;

//     virtual T& operator+=(const double& other) = 0;
//     virtual T& operator-=(const double& other) = 0;
//     virtual T& operator*=(const double& other) = 0;
// };
  
class Complex : public virtual INumbers<Complex> {  
    private:  
       double re; 
       double im; 
    public:  
        Complex(double re = 0, double im = 0);
        Complex(const Complex &x);  
        Complex(const std::string &s);  
        
        Complex operator+(const Complex &x) const override;
        Complex operator-(const Complex &x) const override;
        Complex operator*(const Complex &x) const override;
        Complex operator/(const Complex &x) const override;
        
        Complex& operator+=(const Complex &x) override;
        Complex& operator-=(const Complex &x) override;
        Complex& operator*=(const Complex &x) override;

         
        Complex operator+(const double& x) const;       
        Complex operator-(const double& x) const;
        Complex operator*(const double& x) const;        
        Complex operator/(const double& x) const;    
        Complex& operator+=(const double& x);
        Complex& operator-=(const double& x);
        Complex& operator*=(const double& x);     
        Complex operator^(const double& x) const; 
        
        std::pair<double, double> trig_form(const Complex &x) const;

        bool operator>(const Complex &x) const override {
            throw std::logic_error("Operation '>' is not connectable with complex nubers");
        }
        bool operator<(const Complex &x) const override {
            throw std::logic_error("Operation '>' is not connectable with complex nubers");
        } 
        bool operator>=(const Complex &x) const override {
            throw std::logic_error("Operation '>' is not connectable with complex nubers");
        } 
        bool operator<=(const Complex &x) const override {
            throw std::logic_error("Operation '<=' is not connectable with complex nubers");
        }  
        bool operator==(const Complex &x) const override;  
        bool operator!=(const Complex &x) const override;   
        
        friend std::ostream &operator<<(std::ostream &out, const Complex &x);  
}; 

#endif