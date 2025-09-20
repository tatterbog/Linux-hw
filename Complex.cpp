#include "Complex.h"

Complex::Complex(double a, double b) : a(a), b(b) {}

Complex Complex::operator+(const Complex& oth) const {
	return Complex(a + oth.a, b + oth.b);
}


Complex Complex::operator-(const Complex& oth) const {
	return Complex(a - oth.a, b - oth.b);
}


Complex Complex::operator*(double oth) const {
	return Complex(a * oth, b * oth);
} 

Complex& Complex::operator=(const Complex& oth){
	if(this != &oth){
		a = oth.a;
		b = oth.b;
	}
	return *this;
}


double Complex::abs() const {
	return sqrt(a * a + b * b);
}

		
bool Complex::operator>(const Complex& oth) const {
	return this->abs() > oth.abs(); 
}



std::ostream& operator<<(std::ostream& os, const Complex& c){
	os << c.a;
	if(c.b >= 0){
		os << '+';
	}

	os << c.b << 'i';
	return os;
}

