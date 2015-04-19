/*
 * GenericMatrix.h
 *
 *  Created on: Apr 19, 2015
 *      Author: ericwadkins
 */

#ifndef GENERICMATRIX_H_
#define GENERICMATRIX_H_

#include <stdexcept>
#include <vector>
#include <string>

template<typename T>
class Matrix;
template<typename T>
class PolynomialMatrix;
template<typename T>
class ComplexMatrix;

template<typename T>
class GenericMatrix: public Matrix<T> {
public:
	GenericMatrix(int m, int n);
	GenericMatrix(int size);
	template<typename S> GenericMatrix(GenericMatrix<S> other);
	virtual ~GenericMatrix() {
	}
	virtual std::vector<T> eigenvalues();
	virtual GenericMatrix<T>* toGenericMatrix();
	virtual PolynomialMatrix<T>* toPolynomialMatrix();
	virtual ComplexMatrix<T>* toComplexMatrix();
	virtual std::vector<std::string> toStringVector();
};

#endif /* GENERICMATRIX_H_ */