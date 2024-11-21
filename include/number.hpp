#ifndef NUMBER_HPP
#define NUMBER_HPP


template <typename T>
class INumbers {
protected:
    virtual ~INumbers() = default;

public:
    virtual T operator+(const T& other) const = 0;
    virtual T operator-(const T& other) const = 0;
    virtual T operator*(const T& other) const = 0;
    virtual T operator/(const T& other) const = 0;

    virtual T& operator+=(const T& other) = 0;
    virtual T& operator-=(const T& other) = 0;
    virtual T& operator*=(const T& other) = 0;

    virtual bool operator<(const T& other) const = 0;
    virtual bool operator>(const T& other) const = 0;
    virtual bool operator==(const T& other) const = 0;
    virtual bool operator!=(const T& other) const = 0;
    virtual bool operator<=(const T& other) const = 0;
    virtual bool operator>=(const T& other) const = 0;

    virtual T operator+(const double& other) const = 0;
    virtual T operator-(const double& other) const = 0;
    virtual T operator*(const double& other) const = 0;
    virtual T operator/(const double& other) const = 0;

    virtual T& operator+=(const double& other) = 0;
    virtual T& operator-=(const double& other) = 0;
    virtual T& operator*=(const double& other) = 0;
};

#endif