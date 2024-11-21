#include "big_int.hpp"

#include <iostream>
#include <vector>
#include <cstdint>
#include <cinttypes>
#include <cstdio>
#include <cmath>
#include <iomanip>
#include <bitset>
#include <stack>

BigInt::BigInt() {
    this->digits.push_back(0);
}

BigInt::BigInt(const std::string& s) {
    size_t len = s.length();
    bool isNegative = (s[0] == '-');


    std::string WithoutSign = isNegative ? s.substr(1) : s;

    uint64_t num = 0;
    uint32_t carry = 0;
    int shiftAmount = 0; 

    for (int i = WithoutSign.length(); i > 0; i -= 8) {
        size_t length = (i >= 8) ? 8 : i;
        std::string part = WithoutSign.substr(i - length, length);
        uint32_t partValue = static_cast<uint32_t>(std::stoul(part, nullptr, 16));

        num = ((uint64_t)partValue << shiftAmount) + carry;

        digits.push_back(static_cast<int32_t>(num & 0xFFFFFFFF));

        carry = static_cast<uint32_t>(num >> 32);

        shiftAmount = 0;
    }

    if (carry > 0) {
        digits.push_back(static_cast<int32_t>(carry));
    }

    if (isNegative) {
        this->negate();
    }
    if(this->check_last_bit()) {
        this->digits.push_back(0);
    }
}

BigInt::BigInt(int a) {
    if (a < 0) {
        digits.push_back(static_cast<int32_t>(a));
    } else {
        digits.push_back(a);
    }
}

BigInt::BigInt(const BigInt& x) {
    this->digits = x.digits;
}

void BigInt::negate() {
    for (size_t i = 0; i < digits.size(); ++i) {
        digits[i] = ~digits[i];
    }

    int64_t carry = 1; 
    for (size_t i = 0; i < digits.size(); ++i) {
        int64_t result = static_cast<uint32_t>(digits[i]) + carry;
        
        digits[i] = static_cast<int32_t>(result & 0xFFFFFFFF);
        
        carry = (result >> 32) & 0x1;

        if (carry == 0) {
            break;
        }
    }

    if (carry == 1) {
        digits.push_back(1);
    }
}

bool BigInt::check_last_bit() const {
    if (digits.empty()) return false;

    int32_t last_digit = digits.back();
    return (last_digit & 0x80000000) != 0;
}


BigInt& BigInt::operator+=(const BigInt& a) {
    size_t maxSize = std::max(this->digits.size(), a.digits.size());
    this->digits.resize(maxSize, 0);

    int64_t carry = 0;
    bool thisNegative = (this->digits.back() & 0x80000000);
    bool aNegative = (a.digits.back() & 0x80000000); 
    for (size_t i = 0; i < maxSize; ++i) {
        int64_t thisDigit = (i < this->digits.size()) ? static_cast<uint32_t>(this->digits[i]) : 0;
        int64_t aDigit = (i < a.digits.size()) ? static_cast<uint32_t>(a.digits[i]) : 0;

        int64_t sum = thisDigit + aDigit + carry;

        this->digits[i] = static_cast<int32_t>(sum & 0xFFFFFFFF);

        carry = (sum >> 32) & 0x1;
    }

    //std::cout << "carry: " << carry << std::endl;

    if (thisNegative != aNegative) {
        if (thisNegative && !aNegative) {
            if (this->digits.back() & 0x80000000) {
                if (carry != 0) {
                    this->digits.push_back(-1);
                }
            }
        } else if (!thisNegative && aNegative) {
            if (!(this->digits.back() & 0x80000000) && carry != 0) {
                this->digits.push_back(0);
            }
        }
    } else if (carry != 0) {
        if (thisNegative) {
            this->digits.push_back(-1);
        } else {
            this->digits.push_back(0);
        }
    }
    // << "check: " << this->check_last_bit() << " thisNegative: "<< thisNegative << " aNegative: " << aNegative << std::endl; 
    if (this->check_last_bit() && !thisNegative && !aNegative ) {
        this->digits.push_back(0);
    }


    return *this;
}

BigInt BigInt::operator*(const double& y) const {
    BigInt result;
    result.naive_mul(result, *this, y, 0, this->digits.size());
    return result;
}

BigInt& BigInt::operator*=(const double& y) {
    BigInt result;
    result.naive_mul(result, *this, y, 0, this->digits.size());
    *this = result;
    return *this;
}

BigInt& BigInt::operator+=(const double& a) {
    int new_a = static_cast<int>(std::round(a));
    BigInt copy(new_a);
    *this += copy;
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& a) {
    BigInt copy = a;
    copy.negate();
    *this += copy;
    return *this;
}



BigInt BigInt::operator-(const BigInt& b) const {
    BigInt result = *this;
    result -= b;  
    return result;
}


BigInt BigInt::operator-(const double& b) const {
    BigInt result = *this;
    result -= BigInt(b);  
    return result;
}


BigInt& BigInt::operator-=(const double& a) {
    int new_a = static_cast<int>(std::round(a));
    BigInt copy(new_a);
    *this -= copy;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const BigInt& a) {
    BigInt positive = a;
    
    if (a.digits.back() < 0) {
        out << "-";
        positive.negate();
    }

    bool firstDigit = true;
    for (int i = positive.digits.size() - 1; i >= 0; --i) {
        if (firstDigit) {
            out << std::hex << (uint32_t)positive.digits[i];
            firstDigit = false;
        } else {
            out << std::setfill('0') << std::setw(8) << std::hex << (uint32_t)positive.digits[i];
        }
    }

    return out;
}

BigInt& BigInt::operator*=(const BigInt& y) {
    BigInt result;
    result = *this * y;
    *this = result;
    return *this;
}

BigInt BigInt::operator+(const BigInt& b) const {
    BigInt result = *this;
    result += b;  
    return result;
}

BigInt BigInt::operator+(const double& b) const {
    BigInt result = *this;
    result += BigInt(b);
    return result;
}


bool BigInt::operator>(const BigInt& b) const {
    
    bool thisLast = this->check_last_bit();
    bool bLast = b.check_last_bit();

    if (thisLast && bLast) {
        return thisLast > bLast ? 0 : 1;
    }
    if (thisLast == 1 && bLast == 1) {
        for (int i = this->digits.size() - 1; i >= 0; --i) {
            if(this->digits[i] < b.digits[i]) {
                return true;
            }
            if(this->digits[i] > b.digits[i]) {
                return false;
            }
        }
    }
    else {
        for (int i = this->digits.size() - 1; i >= 0; --i) {
            if(this->digits[i] > b.digits[i]) {
                return true;
            }
            if(this->digits[i] < b.digits[i]) {
                return false;
            }
        }
    }
    return false;

}

bool BigInt::operator<(const BigInt& b) const {
    return b > *this;
}

bool BigInt::operator>=(const BigInt& b) const {
    return !(*this < b);
}

bool BigInt::operator!=(const BigInt& b) const {
    return !(*this == b);
}

bool BigInt::operator<=(const BigInt& b) const {
    return !(b < *this);
}

bool BigInt::operator==(const BigInt& b) const {
    return (*this > b)&&(!(b > *this));
}

void BigInt::print_in_bits() {
    for (auto a : this->digits) {
        std::bitset<32> binary(a);
        std::cout << binary << " ";
    }
    std::cout << std::endl;

}

void BigInt::print_in_int32() {
    for (int i = this->digits.size() - 1; i >= 0; --i) {
        std::cout << static_cast<int32_t>(digits[i]) << " ";
    }
    std::cout << std::endl;
}

BigInt BigInt::operator*(const BigInt& y) const {
    const size_t len_f_naive = 2;
    BigInt x = *this;
    size_t len = std::max(x.digits.size(), y.digits.size());
    //std::cout << "len: " << len << std::endl;
    
    size_t result_size = 2 * len;

    bool x_negative = x.check_last_bit();
    bool y_negative = y.check_last_bit();

    if (x_negative) x.negate();
    BigInt y_copy = y;
    if (y_negative) y_copy.negate();

    x.pad_to_power_of_two(len);
    y_copy.pad_to_power_of_two(len);

    BigInt result;
    result.digits.resize(result_size, 0);

    std::stack<std::tuple<BigInt, BigInt, size_t, size_t, bool>> tasks;
    tasks.push({x, y_copy, 0, len, false});
    
    while (!tasks.empty()) {
        auto [current_x, current_y, start, size, done] = tasks.top();
        tasks.pop();
        
        if (done) {
           // std::cout << "1" << std::endl;
            combine_results(result, current_x, current_y, start, size);
            continue;
        }
        
        if (size <= len_f_naive) {
           // std::cout << "2" << std::endl;
            naive_mul(result, current_x, current_y, start, size);
        } else {
           // std::cout << "3" << std::endl;
            size_t half_size = size / 2;

            BigInt Xl = current_x.split(half_size, size - half_size);
            BigInt Xr = current_x.split(0, half_size);
            BigInt Yl = current_y.split(half_size, size - half_size);
            BigInt Yr = current_y.split(0, half_size);

            BigInt sum_x = Xl + Xr;
            BigInt sum_y = Yl + Yr;
            
            tasks.push({current_x, current_y, start, size, true});
            tasks.push({sum_x, sum_y, start + half_size, half_size, false});
            tasks.push({Xr, Yr, start, half_size, false});
            tasks.push({Xl, Yl, start + size, half_size, false});
        }
    }
    
    for (size_t i = 0; i < result.digits.size() - 1; ++i) {
        uint64_t current = static_cast<uint64_t>(static_cast<uint32_t>(result.digits[i]));
        result.digits[i] = static_cast<int32_t>(current & 0xFFFFFFFF);
        result.digits[i + 1] += static_cast<int32_t>(current >> 32);
    }
    
    if (x_negative != y_negative) {
        result.negate();
    }
    
    if (result.check_last_bit()) {
        result.digits.push_back(0);
    }
    
    return result;
}

void BigInt::pad_to_power_of_two(size_t len) {
    size_t new_size = 1;
    while (new_size < len) new_size <<= 1;
    digits.resize(new_size, 0);
}

BigInt BigInt::split(size_t start, size_t len) const {
    BigInt result;
    result.digits = std::vector<int32_t>(digits.begin() + start, digits.begin() + start + len);
    return result;
}

void BigInt::combine_results(BigInt& result, const BigInt& P1, const BigInt& P2, size_t start, size_t len) const {
    int64_t carry = 0;
    for (size_t i = 0; i < len * 2; ++i) {
        if (i < len) {
            int64_t sum = static_cast<int64_t>(result.digits[start + i]) +
                         static_cast<int64_t>(P1.digits[i]) + carry;
            result.digits[start + i] = static_cast<int32_t>(sum & 0xFFFFFFFF);
            carry = sum >> 32;
        }
        if (i >= len) {
            int64_t sum = static_cast<int64_t>(result.digits[start + i]) +
                         static_cast<int64_t>(P2.digits[i - len]) + carry;
            result.digits[start + i] = static_cast<int32_t>(sum & 0xFFFFFFFF);
            carry = sum >> 32;
        }
    }
    if (carry > 0) {
        result.digits.push_back(static_cast<int32_t>(carry));
    }
}

void BigInt::naive_mul(BigInt& result, const BigInt& x, const BigInt& y, size_t start, size_t len) const {
    for (size_t i = 0; i < len; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t current = static_cast<uint64_t>(static_cast<uint32_t>(x.digits[i])) *
                              static_cast<uint64_t>(static_cast<uint32_t>(y.digits[j])) +
                              static_cast<uint64_t>(static_cast<uint32_t>(result.digits[start + i + j])) +
                              carry;
            
            result.digits[start + i + j] = static_cast<int32_t>(current & 0xFFFFFFFF);
            carry = current >> 32;
        }
        if (carry > 0 && start + i + len < result.digits.size()) {
            result.digits[start + i + len] += static_cast<int32_t>(carry);
        }
    }
}