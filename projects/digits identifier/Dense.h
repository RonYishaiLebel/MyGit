#ifndef DENSE_H
#define DENSE_H

#include "Activation.h"

typedef Matrix (*ActivationFunction) (const Matrix &);

class Dense
{
  public:

    /**
     * @brief Constructs a Dense layer with given weights, bias,
     * and activation function.
     *
     * @param weights The weight matrix of the dense layer.
     * @param bias The bias matrix of the dense layer.
     * @param func The activation function to be applied on the output of the
     * dense layer.
     */
  Dense(const Matrix& weights, const Matrix& bias, ActivationFunction func);

  /**
   * @brief Returns the weights matrix of the dense layer.
   *
   * @return const Matrix& The weights matrix of the dense layer.
   */
  const Matrix & get_weights() const;


  /**
   * @brief Returns the bias matrix of the dense layer.
   *
   * @return const Matrix& The bias matrix of the dense layer.
   */
  const Matrix& get_bias() const;

  /**
   * @brief Returns the activation function of the dense layer.
   *
   * @return const ActivationFunction& The activation function of the
   * dense layer.
   */
  const ActivationFunction& get_activation() const;

  /**
   * @brief Applies the Dense layer to the input matrix using the stored
   * weights, bias, and activation function.
   *
   * @param mat The input matrix to be processed by the dense layer.
   * @return Matrix The output matrix after applying the dense layer
   * and activation function.
   */
  Matrix operator() (const Matrix& mat) const;

 private:
  Matrix weights; // The weight matrix of the dense layer
  Matrix bias; // The bias matrix of the dense layer
  ActivationFunction act_func; // The activation function to be applied to the
  // dense layer's output
};



#endif //DENSE_H
