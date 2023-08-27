//
// Created by ron on 6/11/23.
//
#include "Matrix.h"
#include <iostream>
#include <cmath>
#include <fstream>

#define POINT_ONE 0.1
#define LEN_ERR "Matrix dimension doesn't fit.\n"
#define RANGE_ERR "Trying to access out of range item in the matrix.\n"
#define FILL_MAT_ERR "Failed to extract element from the input stream.\n"
#define BAD_IMAGE_ERR "istream is bad.\n"
#define SHORT_IMAGE "Image is to little for the matrix.\n"
#define TWO 2
#define HALF 0.5
#define NEG_ROW_COL "Rows and cols can't be negative\n"


matrix_dims initiate_mat_dims(int rows, int cols)
{
  matrix_dims mat_dims;
  mat_dims.rows = rows;
  mat_dims.cols = cols;
  return mat_dims;
}

Matrix::Matrix (int rows, int cols)
{
  if (rows <= 0 || cols <= 0)
  {
    throw std::length_error (NEG_ROW_COL);
  }
  _dims = initiate_mat_dims (rows, cols);
  _mat = new float [cols * rows];
  for (int i = 0; i < rows*cols; ++i)
  {
    _mat[i] = 0;
  }
}

Matrix::Matrix (): Matrix(1, 1)
{
  _mat = new float [1];
  _mat[0] = 0;
}


Matrix::Matrix (const Matrix &mat)
{
  _dims = mat._dims;
  _mat = new float[_dims.rows * _dims.cols];
  for(int i = 0; i < _dims.rows * _dims.cols; i++)
  {
    _mat[i] = mat._mat[i];
  }
}


Matrix::~Matrix ()
{
  delete[] _mat;
}

int Matrix::get_rows () const
{
  return _dims.rows;
}


int Matrix::get_cols () const
{
  return _dims.cols;
}


Matrix& Matrix::transpose ()
{
  Matrix new_mat(_dims.cols, _dims.rows);
  for (int i = 0; i < _dims.rows; i++)
  {
    for (int j = 0; j < _dims.cols; j++)
    {
      new_mat._mat[i + _dims.rows * j] = _mat[j + i * _dims.cols];
    }
  }
  *this = new_mat;
  return *this;
}


Matrix& Matrix::vectorize ()
{
  _dims.rows *= _dims.cols;
  _dims.cols = 1;
  return *this;
}


void Matrix::plain_print ()
{
  for (int i = 0; i < _dims.rows; i++)
  {
    for (int j = 0 ; j < _dims.cols; j++)
    {
      std::cout<< _mat[i * _dims.cols + j] << " ";
    }
    std::cout << std::endl;
  }
}


Matrix Matrix::dot (const Matrix &other_mat) const
{
  if (!(_dims.rows == other_mat._dims.rows &&
  _dims.cols == other_mat._dims.cols))
  {
    throw std::length_error (LEN_ERR);
  }
  Matrix product = Matrix(_dims.rows, _dims.cols);
  for (int i = 0; i < _dims.rows * _dims.cols; ++i)
  {
      product._mat[i] = _mat[i] * other_mat._mat[i];
  }
  return product;
}


float Matrix::norm () const
{
  float res = 0;
  for (int i = 0; i < _dims.rows * _dims.cols; ++i)
  {
      res += std::pow(_mat[i], TWO);
  }
  return std::sqrt (res);
}


int Matrix::argmax () const
{
  int cur_max_ind = 0;
  float cur_max = _mat[0];
  for (int i = 0; i < _dims.rows * _dims.cols; ++i)
  {
      if(_mat[i] > cur_max)
      {
        cur_max = _mat[i];
        cur_max_ind = i;
    }
  }
  return cur_max_ind;
}


float Matrix::sum () const
{
  float coor_sum = 0;
  for (int i = 0; i < _dims.rows * _dims.cols; ++i)
  {
    coor_sum += _mat[i];
  }
  return coor_sum;
}


Matrix Matrix::operator+ (const Matrix &other) const
{
  if (!(_dims.rows == other._dims.rows && _dims.cols == other._dims.cols))
  {
    throw std::length_error (LEN_ERR);
  }
  Matrix new_mat = Matrix(_dims.rows, _dims.cols);
  for (int i = 0; i < _dims.rows; ++i)
  {
    for (int j = 0; j < _dims.cols; ++j)
    {
      int ind = i * _dims.cols + j;
      new_mat._mat[ind] = _mat[ind] + other._mat[ind];
    }
  }
  return new_mat;
}


Matrix& Matrix::operator= (const Matrix &other)
{
  if(this != &other)
  {
    if(_dims.rows != other._dims.rows || _dims.cols != other._dims.cols)
    {
      delete[] _mat;
      _mat = new float[other._dims.rows * other._dims.cols];
      _dims = other._dims;
    }
    for (int i = 0; i < _dims.rows * _dims.cols; ++i)
    {
      _mat[i] = other._mat[i];
    }
  }
  return *this;
}

Matrix Matrix::operator* (const Matrix &rhs) const
{
  if (_dims.cols != rhs._dims.rows)
  {
    throw std::length_error (LEN_ERR);
  }
  Matrix product = Matrix(_dims.rows, rhs._dims.cols);
  for (int i = 0; i < product._dims.rows; ++i)
  {
    for (int j = 0; j < product._dims.cols; ++j)
    {
      product._mat[i * product._dims.cols + j] = product_i_j (rhs, i, j);
    }
  }
  return product;
}


Matrix Matrix::operator* (float rhs) const
{
  Matrix product = Matrix(_dims.rows, _dims.cols);
  for (int i = 0; i < _dims.rows; ++i)
  {
    for (int j = 0; j < _dims.cols; ++j)
    {
      product._mat[i * _dims.cols + j] = _mat[i * _dims.cols + j] * rhs;
    }
  }
  return product;
}


Matrix operator*(float lhs, const Matrix& rhs)
{
  return rhs.operator* (lhs);
}


const float& Matrix::operator() (int i, int j) const
{
  if (i < 0 || i >= _dims.rows || j < 0 || j>= _dims.cols)
  {
    throw std::out_of_range (RANGE_ERR);
  }
  return _mat[i * _dims.cols + j];
}


float& Matrix::operator() (int i, int j)
{
  if (i < 0 || i >= _dims.rows || j < 0 || j>= _dims.cols)
  {
    throw std::out_of_range (RANGE_ERR);
  }
  return _mat[i * _dims.cols + j];
}


const float& Matrix::operator[] (int i) const
{
  if (0 > i || i >= (_dims.cols * _dims.rows))
  {
    throw std::out_of_range (RANGE_ERR);
  }
  return _mat[i];
}


float& Matrix::operator[] (int i)
{
  if (0 > i || i >= (_dims.cols * _dims.rows))
  {
    throw std::out_of_range (RANGE_ERR);
  }
  return _mat[i];
}



std::ostream& operator<<(std::ostream& os, const Matrix& mat)
{
  for (int i = 0; i < mat._dims.rows; ++i)
  {
    for (int j = 0; j < mat._dims.cols; ++j)
    {
      int ind = i * mat._dims.cols + j;
      if (mat._mat[ind] > POINT_ONE)
      {
        std::cout << "**";
      }
      else
      {
        std::cout << "  ";
      }
    }
    std::cout << std::endl;
  }
  return os;
}


std::istream & operator>>(std::ifstream& is, const Matrix& mat)
{
  for (int i = 0; i < mat._dims.rows * mat._dims.cols; ++i)
  {
    is.read ((char *) &mat[i], sizeof (float ));
    if(!is)
    {
      throw std::runtime_error(FILL_MAT_ERR);
    }
  }
  return is;
}


Matrix& Matrix::operator+= (const Matrix &rhs)
{
  if(_dims.rows != rhs._dims.rows || _dims.cols != rhs._dims.cols)
  {
    throw std::length_error (LEN_ERR);
  }
  for (int i = 0; i < _dims.rows; ++i)
  {
    for (int j = 0; j < _dims.cols; ++j)
    {
      int ind = i * _dims.cols + j;
      _mat[ind] += rhs._mat[ind];
    }
  }
  return *this;
}


Matrix Matrix::rref() const {
  Matrix rrefMatrix(*this); // Create a copy of the original matrix

  int lead = 0;
  int rowCount = rrefMatrix.get_rows();
  int colCount = rrefMatrix.get_cols();

  for (int r = 0; r < rowCount; ++r) {
    if (colCount <= lead) {
      break;
    }

    int i = r;
    while (rrefMatrix(i, lead) == 0) {
      ++i;
      if (i == rowCount) {
        i = r;
        ++lead;
        if (colCount == lead) {
          break;
        }
      }
    }

    // Swap rows i and r
    if (i != r) {
      for (int j = 0; j < colCount; ++j) {
        std::swap(rrefMatrix(i, j), rrefMatrix(r, j));
      }
    }

    float lv = rrefMatrix(r, lead);
    for (int j = 0; j < colCount; ++j) {
      rrefMatrix(r, j) /= lv;
    }

    for (int i = 0; i < rowCount; ++i) {
      if (i != r) {
        float lv = rrefMatrix(i, lead);
        for (int j = 0; j < colCount; ++j) {
          rrefMatrix(i, j) -= lv * rrefMatrix(r, j);
        }
      }
    }

    ++lead;
  }

  return rrefMatrix;
}

int main()
{
  Matrix mat(3,3);
  mat[1] = 1;
  mat[4] = 1;
  mat[7] = 6;
  mat.plain_print();
  std::cout<<std::endl;
  (mat.rref()).plain_print();
}

