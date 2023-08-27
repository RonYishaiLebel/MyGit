#include "MlpNetwork.h"
#define FIRST 0
#define SECOND 1
#define THIRD 2
#define FORTH 3
#define DIG_NUM 10

MlpNetwork::MlpNetwork (const Matrix *weights_arr, const Matrix *bias_arr) :
    first
    (weights_arr[FIRST], bias_arr[FIRST], activation::relu),
    second
    (weights_arr[SECOND], bias_arr[SECOND], activation::relu),
    third
    (weights_arr[THIRD], bias_arr[THIRD], activation::relu),
    forth
    (weights_arr[FORTH], bias_arr[FORTH], activation::softmax)
{}

digit MlpNetwork::operator() (Matrix &mat) const
{
  Matrix proc_mat = forth( third( second( first( mat.vectorize()))));
  unsigned int max = proc_mat.argmax();
  digit  res{max, proc_mat[max]};
  return res;
}