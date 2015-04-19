/*
 * Complex.cpp
 *
 *  Created on: Apr 19, 2015
 *      Author: ericwadkins
 */

#include "Complex.h"

template<typename T>
std::string Complex<T>::variable = "i";

template<typename T>
Complex<T>::Complex() {
	std::vector<T> numCoeffs;
	numCoeffs.push_back(0);
	_numCoeffs = numCoeffs;
	std::vector<T> denCoeffs;
	denCoeffs.push_back(1);
	_denCoeffs = denCoeffs;
}

template<typename T>
Complex<T>::Complex(std::vector<T> numCoeffs, std::vector<T> denCoeffs) {
	_numCoeffs = numCoeffs;
	_denCoeffs = denCoeffs;
	simplify();
}

template<typename T>
Complex<T>::Complex(std::vector<T> numCoeffs, T den) {
	_numCoeffs = numCoeffs;
	std::vector<T> denCoeffs;
	denCoeffs.push_back(den);
	_denCoeffs = denCoeffs;
	simplify();
}

template<typename T>
Complex<T>::Complex(std::vector<T> numCoeffs) {
	_numCoeffs = numCoeffs;
	std::vector<T> denCoeffs;
	denCoeffs.push_back(1);
	_denCoeffs = denCoeffs;
	simplify();
}

template<typename T>
Complex<T>::Complex(T num, std::vector<T> denCoeffs) {
	std::vector<T> numCoeffs;
	numCoeffs.push_back(num);
	_numCoeffs = numCoeffs;
	_denCoeffs = denCoeffs;
	simplify();
}

template<typename T>
Complex<T>::Complex(T num, T den) {
	std::vector<T> numCoeffs;
	numCoeffs.push_back(num / den);
	_numCoeffs = numCoeffs;
	std::vector<T> denCoeffs;
	denCoeffs.push_back(1);
	_denCoeffs = denCoeffs;
}

template<typename T>
Complex<T>::Complex(T value) {
	std::vector<T> numCoeffs;
	numCoeffs.push_back(value);
	_numCoeffs = numCoeffs;
	std::vector<T> denCoeffs;
	denCoeffs.push_back(1);
	_denCoeffs = denCoeffs;
}

template<typename T>
template<typename S>
Complex<T>::Complex(Complex<S> other) {
	_numCoeffs = other._numCoeffs;
	_denCoeffs = other._denCoeffs;
}

template<typename T>
T Complex<T>::value(T x) {
	T num = _numCoeffs[0];
	for (int i = 1; i < _numCoeffs.size(); i++)
		num += _numCoeffs[i] * (std::pow(x, i));
	T den = _denCoeffs[0];
	for (int i = 1; i < _denCoeffs.size(); i++)
		den += _denCoeffs[i] * (std::pow(x, i));

	return num / den;
}

template<typename T>
T Complex<T>::value() {
	return value(0);
}

template<typename T>
Complex<T> Complex<T>::add(Complex other) {
	if (_denCoeffs != other._denCoeffs) {
		Complex<T> cd =
				Complex<T>(
						(Complex<T>(_denCoeffs, 1)
								* Complex<T>(other._denCoeffs, 1))._numCoeffs,
						1);
		Complex<T> p1 =
				Complex<T>(
						(Complex<T>(_numCoeffs, 1)
								* Complex<T>(other._denCoeffs, 1))._numCoeffs,
						1);
		Complex<T> p2 =
				Complex<T>(
						(Complex<T>(other._numCoeffs, 1)
								* Complex<T>(_denCoeffs, 1))._numCoeffs, 1);

		std::vector<T> numCoeffs(
				std::max(p1._numCoeffs.size(), p2._numCoeffs.size()));
		for (int i = 0; i < p1._numCoeffs.size(); i++)
			numCoeffs[i] += p1._numCoeffs[i];
		for (int i = 0; i < p2._numCoeffs.size(); i++)
			numCoeffs[i] += p2._numCoeffs[i];

		return Complex<T>(numCoeffs, cd._numCoeffs);
	} else {
		std::vector<T> numCoeffs(
				std::max(_numCoeffs.size(), other._numCoeffs.size()));
		for (int i = 0; i < _numCoeffs.size(); i++)
			numCoeffs[i] += _numCoeffs[i];
		for (int i = 0; i < other._numCoeffs.size(); i++)
			numCoeffs[i] += other._numCoeffs[i];

		return Complex<T>(numCoeffs, _denCoeffs);
	}
}

template<typename T>
Complex<T> Complex<T>::mul(T n) {
	std::vector<T> numCoeffs(_numCoeffs.size());
	for (int i = 0; i < _numCoeffs.size(); i++)
		numCoeffs[i] = n * _numCoeffs[i];
	return Complex<T>(numCoeffs, _denCoeffs);
}

template<typename T>
Complex<T> Complex<T>::mul(Complex other) {
	std::vector<T> numCoeffs;
	for (int i = 0; i < _numCoeffs.size(); i++)
		for (int j = 0; j < other._numCoeffs.size(); j++) {
			while (i + j >= numCoeffs.size())
				numCoeffs.push_back(0);
			numCoeffs[i + j] += _numCoeffs[i] * other._numCoeffs[j];
		}
	std::vector<T> denCoeffs;
	for (int i = 0; i < _denCoeffs.size(); i++)
		for (int j = 0; j < other._denCoeffs.size(); j++) {
			while (i + j >= denCoeffs.size())
				denCoeffs.push_back(0);
			denCoeffs[i + j] += _denCoeffs[i] * other._denCoeffs[j];
		}
	return Complex<T>(numCoeffs, denCoeffs);
}

template<typename T>
Complex<T> Complex<T>::reciprocal() {
	return Complex(_denCoeffs, _numCoeffs);
}

template<typename T>
Complex<T> Complex<T>::clone() {
	return Complex<T>(_numCoeffs, _denCoeffs);
}

template<typename T>
bool Complex<T>::isConstant() {
	return _numCoeffs.size() == 1 && _denCoeffs.size() == 1 ? true : false;
}

template<typename T>
bool Complex<T>::equals(T rhs) {
	if (isConstant() && value() == rhs)
		return true;
	return false;
}

template<typename T>
bool Complex<T>::equals(Complex rhs) {
	if (isConstant() && rhs.isConstant())
		return value() == rhs.value();
	if (_numCoeffs.size() != rhs._numCoeffs.size()
			|| _numCoeffs.size() != rhs._numCoeffs.size())
		return false;
	bool result = true;
	for (int i = 0; i < _numCoeffs.size(); i++)
		if (_numCoeffs[i] != rhs._numCoeffs[i])
			return false;
	for (int i = 0; i < _denCoeffs.size(); i++)
		if (_denCoeffs[i] != rhs._denCoeffs[i])
			return false;
	return result;
}

template<typename T>
std::string Complex<T>::toString() {

	// Whether to display terms in order of decreasing order or increasing order
	const bool decreasingOrder = true;

	std::vector<T> num = _numCoeffs;
	std::vector<T> den = _denCoeffs;

	std::string str = "";
	std::string str1 = "";
	int numCount = 0;
	for (int i = 0; i < num.size(); i++) {
		int order;
		if (decreasingOrder)
			order = num.size() - (i + 1);
		else
			order = i;
		if (num[order] != 0 || num.size() == 1) {
			if (numCount != 0) {
				if (num[order] > 0)
					str1 += " + ";
				else
					str1 += " - ";
			}
			if (numCount == 0 && num[order] < 0)
				str1 += "-";
			if ((num[order] != 1 && num[order] != -1) || order == 0)
				str1 += trimNumber(std::to_string(std::abs(num[order])));
			if (order == 1)
				str1 += variable;
			else if (order > 1)
				str1 += variable + "^" + std::to_string(order);
			numCount++;
		}
	}
	str = str1;

	if (den.size() > 1 || den[0] != 1) {
		if (numCount > 1)
			str1 = "(" + str1 + ")";
		std::string str2 = "";
		int denCount = 0;
		for (int i = 0; i < den.size(); i++) {
			int order;
			if (decreasingOrder)
				order = den.size() - (i + 1);
			else
				order = i;
			if (den[order] != 0 || den.size() == 1) {
				if (denCount != 0) {
					if (den[order] > 0)
						str2 += " + ";
					else
						str2 += " - ";
				}
				if (denCount == 0 && den[order] < 0)
					str2 += "-";
				if ((den[order] != 1 && den[order] != -1) || order == 0)
					str2 += trimNumber(std::to_string(std::abs(den[order])));
				if (order == 1)
					str2 += variable;
				else if (order > 1)
					str2 += variable + "^" + std::to_string(order);
				denCount++;
			}
		}
		if (denCount > 1)
			str2 = "(" + str2 += ")";
		str = str1 + "/" + str2;
	}
	return str;
}

template<typename T>
void Complex<T>::print() {
	std::cout << toString() << std::endl;
}

template<typename T>
std::vector<T> Complex<T>::coeffs() {
	return _numCoeffs;
}

template<typename T>
void Complex<T>::set(Complex other) {
	/*std::vector<T> numCoeffs;
	 for (int i = 0; i < other._numCoeffs.size(); i++)
	 _numCoeffs[i] = other._numCoeffs[i];
	 std::vector<T> denCoeffs;
	 for (int i = 0; i < other._denCoeffs.size(); i++)
	 _denCoeffs[i] = other._denCoeffs[i];*/
	_numCoeffs = other._numCoeffs;
	_denCoeffs = other._denCoeffs;
}

template<typename T>
void Complex<T>::simplify() {

	// Check for 0 division
	if (_denCoeffs.size() == 1 && _denCoeffs[0] == 0)
		throw std::runtime_error("Complex division by zero");

	// If any of the ending coefficients are 0, remove that term
	while (_numCoeffs.size() > 1 && _numCoeffs[_numCoeffs.size() - 1] == 0)
		_numCoeffs.pop_back();
	while (_denCoeffs.size() > 1 && _denCoeffs[_denCoeffs.size() - 1] == 0)
		_denCoeffs.pop_back();

	// If a variable can be factored out of all terms, do this until there is a constant that prevents it
	while (_numCoeffs.size() > 1 && _denCoeffs.size() > 1 && _numCoeffs[0] == 0
			&& _denCoeffs[0] == 0) {
		_numCoeffs.erase(_numCoeffs.begin());
		_denCoeffs.erase(_denCoeffs.begin());
	}

	// If the numerator and denominator are both constants, set the Complex to the value/1
	if (isConstant() && _denCoeffs[0] != 1) {
		_numCoeffs[0] = value();
		_denCoeffs[0] = 1;
		return;
	}

	// If the denominator is just a constant, divide the top by the constant and make the bottom 1
	if (_denCoeffs.size() == 1) {
		for (int i = 0; i < _numCoeffs.size(); i++)
			_numCoeffs[i] *= (1.0 / _denCoeffs[0]);
		_denCoeffs[0] = 1;
	}

	while (_numCoeffs.size() > 2) {
		_numCoeffs[_numCoeffs.size() - 3] -= _numCoeffs[_numCoeffs.size() - 1];
		_numCoeffs.pop_back();
	}
	while (_denCoeffs.size() > 2) {
		_denCoeffs[_denCoeffs.size() - 3] -= _denCoeffs[_denCoeffs.size() - 1];
		_denCoeffs.pop_back();
	}

	// Complex long division
	/*if (_denCoeffs.size() > 1 && _numCoeffs.size() >= _denCoeffs.size()) {
	 Complex<T> dividend = Complex(_numCoeffs);
	 Complex<T> divisor = Complex(_denCoeffs);
	 Complex<T> result = Complex();
	 Complex<T> remainder = dividend;
	 while (remainder != 0
	 && remainder._numCoeffs.size() >= divisor._numCoeffs.size()) {
	 std::vector<T> remainderLead;
	 for (int i = 0; i < remainder._numCoeffs.size() - 1; i++)
	 remainderLead.push_back(0);
	 remainderLead.push_back(
	 remainder._numCoeffs[remainder._numCoeffs.size() - 1]);

	 std::vector<T> divisorLead;
	 for (int i = 0; i < divisor._numCoeffs.size() - 1; i++)
	 divisorLead.push_back(0);
	 divisorLead.push_back(
	 divisor._numCoeffs[divisor._numCoeffs.size() - 1]);

	 Complex<T> temp = Complex(remainderLead, divisorLead);
	 result += temp;
	 remainder -= (temp * divisor);
	 }
	 if (remainder == 0) {
	 _numCoeffs = result._numCoeffs;
	 _denCoeffs = result._denCoeffs;
	 }
	 }*/

	// If the numerator is 0, regardless of the denominator, set the Complex to 0/1
	if (_numCoeffs.size() == 1 && _numCoeffs[0] == 0) {
		std::vector<T> denCoeffs;
		denCoeffs.push_back(1);
		_denCoeffs = denCoeffs;
		return;
	}
}

template<typename T>
Complex<T> Complex<T>::operator+(Complex rhs) {
	return add(rhs);
}

template<typename T>
void Complex<T>::operator+=(Complex rhs) {
	set(add(rhs));
}

template<typename T>
Complex<T> Complex<T>::operator-(Complex rhs) {
	return add(rhs.mul(-1));
}

template<typename T>
void Complex<T>::operator-=(Complex rhs) {
	set(add(rhs.mul(-1)));
}

template<typename T>
Complex<T> Complex<T>::operator-() {
	return mul(-1);
}

template<typename T>
Complex<T> Complex<T>::operator*(T rhs) {
	return mul(rhs);
}

template<typename T>
Complex<T> Complex<T>::operator*(Complex rhs) {
	return mul(rhs);
}

template<typename T>
void Complex<T>::operator*=(T rhs) {
	set(mul(rhs));
}

template<typename T>
void Complex<T>::operator*=(Complex rhs) {
	set(mul(rhs));
}

template<typename T>
Complex<T> Complex<T>::operator/(T rhs) {
	return mul(1 / rhs);
}

template<typename T>
Complex<T> Complex<T>::operator/(Complex rhs) {
	return mul(rhs.reciprocal());
}

/*template<typename T>
 Complex<T> operator/(T lhs, Complex<T> rhs) {
 return rhs.reciprocal().mul(lhs);
 }*/

template<typename T>
void Complex<T>::operator/=(T rhs) {
	set(mul(1 / rhs));
}

template<typename T>
void Complex<T>::operator/=(Complex rhs) {
	set(mul(rhs.reciprocal()));
}

template<typename T>
Complex<T> Complex<T>::operator^(T rhs) {
	Complex<T> result = Complex(1);
	if (rhs == 0)
		return result;
	Complex<T> original = clone();
	int n = std::abs(rhs);
	for (int i = 0; i < n; i++) {
		result.set(result.mul(original));
	}
	if (rhs < 0)
		result.set(result.reciprocal());
	return result;
}

template<typename T>
bool Complex<T>::operator==(T rhs) {
	return equals(rhs);
}

template<typename T>
bool Complex<T>::operator==(Complex rhs) {
	return equals(rhs);
}

template<typename T>
bool Complex<T>::operator!=(T rhs) {
	return !equals(rhs);
}

template<typename T>
bool Complex<T>::operator!=(Complex rhs) {
	return !equals(rhs);
}

template<typename T>
void Complex<T>::operator=(T rhs) {
	std::vector<T> numCoeffs;
	numCoeffs.push_back(rhs);
	_numCoeffs = numCoeffs;
	std::vector<T> denCoeffs;
	denCoeffs.push_back(1);
	_denCoeffs = denCoeffs;
}

template<typename T>
void Complex<T>::operator=(Complex rhs) {
	set(rhs);
}

template<typename T>
std::string Complex<T>::trimNumber(std::string str) {
	if (str.find(std::string(".")) != std::string::npos) {
		for (int c = str.length() - 1; c >= 0; c--) {
			if (str.at(c) != '0' || str.at(c - 1) == '.')
				break;
			str = str.substr(0, c);
		}
	}
	return str;
}

// Explicit instantiation of template classes
template class Complex<int> ;
template class Complex<float> ;
template class Complex<double> ;
template class Complex<long double> ;