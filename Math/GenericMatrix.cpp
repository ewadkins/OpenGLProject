/*
 * GenericMatrix.cpp
 *
 *  Created on: Apr 19, 2015
 *      Author: ericwadkins
 */

#include "GenericMatrix.h"
#include "Matrix.h"

// Basic constructor
template<typename T>
GenericMatrix<T>::GenericMatrix(int rows, int cols) :
		Matrix<T>(rows, cols) {
}

// Square matrix constructor
template<typename T>
GenericMatrix<T>::GenericMatrix(int size) :
		Matrix<T>(size) {
}

// Constructor that allows matrix casting
template<typename T>
template<typename S>
GenericMatrix<T>::GenericMatrix(GenericMatrix<S> other) :
		Matrix<T>(other) {
}

// Returns a vector of the eigenvalues of this matrix
template<typename T>
std::vector<T> GenericMatrix<T>::eigenvalues() {
	if (this->_rows != this->_cols)
		throw std::runtime_error("Matrix must be a square matrix");
	PolynomialMatrix<T> m = toPolynomialMatrix();
	PolynomialMatrix<T> lambdaI = PolynomialMatrix<T>(this->_rows);
	T arr[] = { 0, -1 };
	std::vector<T> coeffs(arr, arr + sizeof(arr) / sizeof(arr[0]));
	for (int n = 0; n < this->_rows; n ++)
		lambdaI.set(n, n, Polynomial<T>(coeffs, 1));

	m = m + lambdaI;
	m.print();
	m.determinant().print();
	std::cout << m.determinant().value() << std::endl;

	//throw std::runtime_error("Matrix eigenvalues operation not available");

	std::vector<T> eigenvalues;

	return eigenvalues;
}

// Returns a generic version of this matrix
template<typename T>
GenericMatrix<T>* GenericMatrix<T>::toGenericMatrix() {
	return this->clone();
}

// Returns a polynomial version of this matrix
template<typename T>
PolynomialMatrix<T>* GenericMatrix<T>::toPolynomialMatrix() {
	PolynomialMatrix<T>* m = new PolynomialMatrix<T>(this->_rows);
	std::vector<T> values = this->getVector();
	std::vector<Polynomial<T> > polyValues;
	for (int i = 0; i < values.size(); i++)
		polyValues.push_back(Polynomial<T>(values[i]));
	m->setVector(polyValues);
	return m;
}

// Returns a complex version of this matrix
template<typename T>
ComplexMatrix<T>* GenericMatrix<T>::toComplexMatrix() {
	ComplexMatrix<T>* m = new ComplexMatrix<T>(this->_rows);
	std::vector<T> values = this->getVector();
	std::vector<Complex<T> > polyValues;
	for (int i = 0; i < values.size(); i++)
		polyValues.push_back(Complex<T>(values[i]));
	m->setVector(polyValues);
	return m;
}

// Returns a vector of strings representing this matrix
template<typename T>
std::vector<std::string> GenericMatrix<T>::toStringVector() {
	std::vector<std::string> strings;
	std::string** arr = new std::string*[this->_rows];
	for (int i = 0; i < this->_rows; i++) {
		arr[i] = new std::string[this->_cols];
		for (int j = 0; j < this->_cols; j++) {
			std::string str = std::to_string(this->_matrix[i][j]);
			if (str.find(std::string(".")) != std::string::npos) {
				for (int c = str.length() - 1; c >= 0; c--) {
					if (str.at(c) != '0' || str.at(c - 1) == '.')
						break;
					str = str.substr(0, c);
				}
			}
			arr[i][j] = str;
		}
	}
	for (int j = 0; j < this->_cols; j++) {
		int maxLength = 0;
		for (int i = 0; i < this->_rows; i++)
			if (arr[i][j].length() > maxLength)
				maxLength = arr[i][j].length();
		for (int i = 0; i < this->_rows; i++)
			while (arr[i][j].length() < maxLength)
				arr[i][j] = " " + arr[i][j];
	}
	for (int i = 0; i < this->_rows; i++) {
		std::string str = "";
		for (int j = 0; j < this->_cols; j++) {
			if (j == 0)
				str += "[";
			str += " " + arr[i][j] + " ";
			if (j == this->_cols - 1)
				str += "]";
		}
		strings.push_back(str);
	}
	return strings;
}

// Explicit instantiation of template classes
template class GenericMatrix<float> ;
template class GenericMatrix<double> ;
template class GenericMatrix<long double> ;