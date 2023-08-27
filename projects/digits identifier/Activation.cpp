//
// Created by ron on 7/24/23.
//
#include "Activation.h"

Matrix activation::relu (const Matrix &mat)
{
  int rows = mat.get_rows ();
  int cols = mat.get_cols ();
  Matrix res (rows, cols);

  int mat_size = rows * cols;
  for (int i = 0; i < mat_size; ++i)
  {
    if (mat[i] >= 0)
    {
      res[i] = mat[i];
    }
    else
    {
      res[i] = 0;
    }
  }
  return res;
}

Matrix activation::softmax (const Matrix &mat)
{
  float mat_sum = 0;
  Matrix res(mat);
  for (int i = 0; i < mat.get_cols()*mat.get_rows(); ++i)
  {
    res[i] = std::exp(res[i]);
    mat_sum += res[i];
  }
  return (1 / mat_sum) * res;
}