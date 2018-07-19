#ifndef DEF_MATRIX
#define DEF_MATRIX

#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>

/**
    Every method ends with "const" keyword which means that the object on which the method is called is never modified.
    Instead it will return a new Matrix.

    It is not very efficient, and it could be a lot faster,
    but for the sake of simplicity we will stick to this architecture.
**/

class Matrix
{
public:
    Matrix();
    Matrix(int height, int width);
    Matrix(std::vector<std::vector<double> > const &array);

    Matrix add(Matrix const &m) const; // addition
    Matrix subtract(Matrix const &m) const; // subtraction
    Matrix multiply(Matrix const &m) const; // hadamard product
    Matrix dot(Matrix const &m) const; // dot product
    Matrix transpose() const; // transposed matrix

    Matrix multiply(double const &value) const; // scalar multiplication
    Matrix applyFunction(double (*function)(double)) const; // to apply a function to every element of the matrix

    int getWidth() const;
    int getHeight() const;
    double get(int i, int j) const;
    double sum() const;
    void print(std::ostream &flux) const; // pretty print of the matrix

private:
    std::vector<std::vector<double> > array;
    int height;
    int width;
};

// overloading operator "<<" to print easily
static std::ostream& operator<<(std::ostream &flux, Matrix const &m)
{
    m.print(flux);
    return flux;
}

#endif
