// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
    int rows, cols;
} matrix_dims;

// Insert Matrix class here...
class Matrix
{
 public:

  /**
   * Constructs Matrix of size 𝑟𝑜𝑤𝑠 × 𝑐𝑜𝑙𝑠.
   * Inits all elements to 0.
   * @param rows
   * @param cols
   */
  Matrix (int rows, int cols);

  /**
   * Constructs Matrix of size 1 × 1. Inits the single element to 0.
   */
  Matrix ();

  /**
   * Constructs a matrix from another Matrix mat
   * @param mat another matrix
   */
  Matrix (const Matrix &mat);

/**
 * destructor
 */
  ~Matrix ();

  /**
   * return number of rows in the matrix
   * @return int number of rows in the matrix
   */
  int get_rows () const;

  /**
   * returns the amount of columns as int.
   * @return returns the amount of columns as int.
   */
  int get_cols () const;

  /**
   * @brief Transposes the matrix in-place.
   *
   * @return Matrix& Reference to the transposed matrix.
   */
  Matrix &transpose ();

  /**
   * Transforms a matrix into a column vector
   * @return n*1 matrix
   */
  Matrix &vectorize ();

  /**
   *Prints matrix elements, no return value.
   *Prints space after each element (including last element in row).
   *Prints newline after each row (including last row).
   */
  void plain_print ();

  /**
   * Returns a matrix which is the element-wise multiplication (Hadamard
   *product) of this matrix with another matrix other_mat.
   * @param other_mat another matrix
   * @return element-wise product matrix
   */
  Matrix dot (const Matrix &other_mat) const;

  /**
   * Returns the Frobenius norm of the given matrix
   * @return Returns the Frobenius norm of the given matrix
   */
  float norm () const;

  /**
   * Returns index of the largest number in the matrix.
   * @return Returns index of the largest number in the matrix.
   */
  int argmax () const;

  /**
   * Return the sum of all elements in the matrix.
   * @return Return the sum of all elements in the matrix.
   */
  float sum () const;

  /**
   * Addition operator.
   * @param other the other matrix that should be added to this.
   * @return the sum of the two matrices.
   */
  Matrix operator+ (const Matrix &other) const;

  /**
   * Assignment operator for Matrix
   * @param other the Matrix that should be assigned into this
   * @return reference to this
   */
  Matrix &operator= (const Matrix &other);

  /**
   * Matrix multiplication.
   * @param other the other Matrix which we should multiply with.
   * @return the product.
   */
  Matrix operator* (const Matrix &rhs) const;

  /**
   * Scalar multiplication on the right.
   * @param rhs right hand side scalar.
   * @return the product.
   */
  Matrix operator* (float rhs) const;

  /**
   * Scalar multiplication on the left
   * @param lhs a scalar.
   * @param rhs a matrix.
   * @return the product.
   */
  friend Matrix operator*(float lhs, const Matrix& rhs);

  /**
   * Const parenthesis indexing, returns the item in the i'th row and j'th
   * column.
   * @param i row index.
   * @param j column index.
   * @return the item in the i'th row and j'th column.
   */
  const float& operator()(int i, int j) const;


  /**
   * Non-const parenthesis indexing, returns the item in the i'th row and j'th
   * column.
   * @param i row index.
   * @param j column index.
   * @return the item in the i'th row and j'th column.
   */
  float& operator()(int i, int j);



  /**
   * Const brackets indexing, return the i'th item in the vectorized form of
   * this.
   * @param i index.
   * @return return the i'th item in the vectorized form of this.
   */
  const float& operator[](int i) const;


  /**
 * Const brackets indexing, return the i'th item in the vectorized form of
 * this.
 * @param i index.
 * @return return the i'th item in the vectorized form of this.
 */
  float& operator[](int i);


  /**
   * Pretty print of matrix
   * @param os the out stream.
   * @param mat the mat we want to print it's content.
   * @return out stream including the content of the matrix.
   */
  friend std::ostream& operator<<(std::ostream& os, const Matrix& mat);

  /**
   * Fills matrix elements. The stream must be big enough to fill the entire
   * matrix (don't trust the user to validate it).
   * @param is
   * @param mat
   * @return
   */
  friend std::istream & operator>>(std::ifstream& is, const Matrix& mat);

  /**
   * Matrix addition.
   * @param rhs matrix to add to this.
   * @return matrices' sum.
   */
  Matrix& operator+=(const Matrix& rhs);

  /**
   * Returns a new Matrix that is the reduced row echelon form of the original.
   * The original Matrix should be unchanged.
   * @return A new Matrix in reduced row echelon form.
   */
  Matrix rref() const;



 private:
  matrix_dims _dims{}; // Matrix dimensions
  float *_mat; // Pointer to the matrix data

  float product_i_j (const Matrix &rhs, int i, int j) const
  {
    float res = 0;
    for (int k = 0; k < _dims.cols; ++k)
    {
      res += _mat[i * _dims.cols + k] * rhs._mat[k * rhs._dims.cols
                                                         + j];
    }
    return res;
  }

};

#endif //MATRIX_H