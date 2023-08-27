//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

#include "Dense.h"

#define MLP_SIZE 4

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit {
	unsigned int value;
	float probability;
} digit;

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
									{64,  128},
									{20,  64},
									{10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
								 {64,  1},
								 {20,  1},
								 {10,  1}};

class MlpNetwork
{
 public:

  /**
   * @brief Constructs an MlpNetwork with given weights and biases
   * for the dense layers.
   *
   * @param weights_arr An array of weight matrices for the dense layers.
   * @param bias_arr An array of bias matrices for the dense layers.
   */
  MlpNetwork(const Matrix weights_arr[], const Matrix bias_arr[]);

  /**
   * @brief Applies the MlpNetwork to the input matrix and identifies
   * the digit with the highest probability.
   *
   * The MlpNetwork processes the input matrix through four dense
   * layers with specified weights and biases.
   * The output of the last layer contains the probabilities of the input
   * image being each digit (0 to 9).
   * The function identifies the digit with the highest probability and
   * returns it along with its probability.
   *
   * @param mat The input matrix representing an image to be classified.
   * @return digit The identified digit along with its probability.
   */
  digit operator() (Matrix & mat) const;

 private:

  Dense first; // The first dense layer of the MlpNetwork
  Dense second; // The second dense layer of the MlpNetwork
  Dense third; // The third dense layer of the MlpNetwork
  Dense forth; // The forth dense layer of the MlpNetwork
};

#endif // MLPNETWORK_H