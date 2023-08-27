#include "Dense.h"

Dense::Dense (const Matrix &weights, const Matrix &bias,
              ActivationFunction func) :
    weights (weights), bias (bias), act_func (func)
{}

const Matrix &Dense::get_weights () const
{ return weights; }

const Matrix &Dense::get_bias () const
{ return bias; }

const ActivationFunction &Dense::get_activation () const
{ return act_func; }

Matrix Dense::operator() (const Matrix &mat) const
{
  return act_func ((weights * mat) + bias);
}
