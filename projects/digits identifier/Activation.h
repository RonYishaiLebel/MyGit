#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"
#include <cmath>


namespace activation
{
    /**
     * @brief Applies the ReLU (Rectified Linear Unit) activation function
     * element-wise to the given matrix.
     *
     * The ReLU activation function is defined as follows:
     * ReLU(x) = max(0, x) for each element x in the matrix.
     *
     * @param mat The input matrix on which the ReLU activation is applied.
     * @return Matrix A new matrix with the same shape as the input matrix,
     * containing the ReLU activations.
     */
    Matrix relu (const Matrix& mat);

    /**
     * @brief Applies the softmax activation function to the given matrix.
     *
     * The softmax function is used to convert raw scores (logits) into
     * probabilities.
     * It exponentiates each element in the matrix and then normalizes them
     * by dividing by the sum of all exponentiated values.
     *
     * @param mat The input matrix on which the softmax activation is applied.
     * @return Matrix A new matrix with the same shape as the input matrix,
     * containing the softmax activations.
     */
    Matrix softmax(const Matrix& mat);

}

#endif //ACTIVATION_H