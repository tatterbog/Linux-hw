#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <cmath>

class Complex{
        private:
                double a, b;
        public:
                friend std::ostream& operator<<(std::ostream& os, const Complex& c);
                Complex(double a = 0, double b = 0);

                Complex operator+(const Complex& oth) const;
                Complex operator-(const Complex& oth) const;

                Complex operator*(double oth) const;

		Complex& operator=(const Complex& oth);

		double abs() const;

                bool operator>(const Complex& oth) const;
};

#endif 
