# Neural Network Digit Recognition Project

## Overview
This project implements a neural network in C++ for handwritten digit recognition. The network is a multi-layer perceptron (MLP) using a pre-trained set of weights and biases. It applies various matrix operations and activation functions such as ReLU and Softmax.

## Files
1. **`Matrix.h / Matrix.cpp`**: Implements the `Matrix` class, responsible for matrix operations such as addition, multiplication, transpose, and more. It includes constructors, destructors, and several operator overloads to facilitate matrix manipulations.
2. **`Activation.h / Activation.cpp`**: Implements the ReLU and Softmax activation functions, which are applied to the matrices to simulate neuron activations.
3. **`Dense.h / Dense.cpp`**: Implements the `Dense` class, representing a fully connected layer in the neural network. It applies the activation functions to the input matrices.
4. **`MlpNetwork.h / MlpNetwork.cpp`**: Implements the MLP network using four dense layers. The network processes an input matrix representing an image of a handwritten digit, passing it through the layers to classify the digit.
5. **`main.cpp`**: The entry point for the program, where the network is initialized with weights and biases and is used to classify a digit image.

## Running the Program

To compile and run the program, execute the following commands in the terminal:

```bash
g++ -std=c++11 main.cpp Matrix.cpp Activation.cpp Dense.cpp MlpNetwork.cpp -o digit_recognition
./digit_recognition
```
Ensure that all required files (weights and biases) are correctly placed in the working directory.

## Neural Network Architecture

1. **Input Layer**: The input is a matrix representing a flattened image of a handwritten digit.
2. **Dense Layer 1**: Fully connected layer with ReLU activation.
3. **Dense Layer 2**: Fully connected layer with ReLU activation.
4. **Dense Layer 3**: Fully connected layer with ReLU activation.
5. **Dense Layer 4**: Fully connected layer with Softmax activation, which outputs the probability distribution over the 10 possible digit classes (0-9).

## Activation Functions

- **ReLU**: Rectified Linear Unit activation function. It outputs the input if it is positive; otherwise, it outputs zero.
- **Softmax**: Normalizes the input vector into a probability distribution, useful for the classification task in the final layer.


## Matrix Operations

The `Matrix` class supports various matrix operations critical for the functioning of the neural network. These include:

1. **Matrix Addition**: Adds two matrices element-wise.
2. **Matrix Multiplication**: Performs matrix multiplication.
3. **Transpose**: Computes the transpose of a matrix.
4. **Vector-Matrix Multiplication**: Used in the forward propagation step of the network.
5. **Element-wise Operations**: Allows for operations such as ReLU to be applied element-wise to matrices.
