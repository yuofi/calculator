#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <cinttypes>
#include <cstdio>
#include <cmath>
#include <iomanip>
#include <bitset>
#include <stack>
#include "number.hpp"



class BigInt : public virtual INumbers<BigInt> {
    private:
        std::vector<int32_t> digits;

    public:
        BigInt(int a);
        BigInt(const BigInt& x);
        BigInt(const std::string& s);

        void print_in_int32();
        void negate();
        void print_in_bits();

        friend std::ostream& operator<<(std::ostream& out, const BigInt& a);
        
        bool check_last_bit() const;
        BigInt operator+(const BigInt& b) const override;
        BigInt operator-(const BigInt& b) const override;
        BigInt operator*(const BigInt& y) const override;
        BigInt& operator-=(const BigInt& b) override;
        BigInt& operator+=(const BigInt& b) override;
        BigInt& operator*=(const BigInt& y) override;
        
        BigInt operator+(const double& other) const override;
        BigInt operator-(const double& other) const override;
        BigInt operator*(const double& other) const override;
        BigInt& operator*=(const double& other) override;
        BigInt& operator-=(const double& other) override;
        BigInt& operator+=(const double& other) override;

        BigInt operator/(const BigInt& other) const override {
            throw std::logic_error("Operation '/' is not defined in BigInt");
        }

        BigInt operator/(const double& other) const override {
            throw std::logic_error("Operation '/' is not defined in BigInt");
        }

        bool operator<(const BigInt& b) const override;
        bool operator>(const BigInt& b) const override;
        bool operator==(const BigInt& b)const override;
        bool operator!=(const BigInt& b)const override;
        bool operator<=(const BigInt& b)const override;
        bool operator>=(const BigInt& b)const override;

        BigInt split(size_t start, size_t len) const;
        void combine_results(BigInt& result, const BigInt& P1, const BigInt& P2, size_t start, size_t len) const;
        void naive_mul(BigInt& result, const BigInt& x, const BigInt& y, size_t start, size_t len) const;
        void pad_to_power_of_two(size_t len);
        
        BigInt();
};

#endif