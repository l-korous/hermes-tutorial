#include "definitions.h"

CustomNonlinearity::CustomNonlinearity(double alpha): Hermes1DFunction<double>()
{
  this->is_const = false;
  this->alpha = alpha;
}

double CustomNonlinearity::value(double u) const
{
  return 1 + Hermes::pow(u, alpha);
}

Ord CustomNonlinearity::value(Ord u) const
{
  return Ord(10);
}

CustomWeakFormPicard::CustomWeakFormPicard(Solution<double>* prev_iter_sln, 
                                           Hermes1DFunction<double>* lambda, 
                                           Hermes2DFunction<double>* f) 
  : WeakForm<double>(1)
{
  // Jacobian.
  CustomJacobian* matrix_form = new CustomJacobian(0, 0, lambda);
  matrix_form->ext.push_back(prev_iter_sln);
  add_matrix_form(matrix_form);

  // Residual.
  CustomResidual* vector_form = new CustomResidual(0, lambda, f);
  vector_form->ext.push_back(prev_iter_sln);
  add_vector_form(vector_form);
}

double CustomWeakFormPicard::CustomJacobian::value(int n, double *wt, Func<double> *u_ext[], 
                                                   Func<double> *u, Func<double> *v, 
                                                   Geom<double> *e, ExtData<double> *ext) const
{
  double result = 0;
  for (int i = 0; i < n; i++) 
  {
    result += wt[i] * lambda->value(ext->fn[0]->val[i]) 
                    * (u->dx[i] * v->dx[i] + u->dy[i] * v->dy[i]);
  }
  return result;
}

Ord CustomWeakFormPicard::CustomJacobian::ord(int n, double *wt, Func<Ord> *u_ext[], 
                                              Func<Ord> *u, Func<Ord> *v,
                                              Geom<Ord> *e, ExtData<Ord> *ext) const 
{
  Ord result = Ord(0);
  for (int i = 0; i < n; i++) 
  {
    result += wt[i] * lambda->value(ext->fn[0]->val[i]) 
                    * (u->dx[i] * v->dx[i] + u->dy[i] * v->dy[i]);
  }
  return result;
}

MatrixFormVol<double>* CustomWeakFormPicard::CustomJacobian::clone()
{
  CustomJacobian* form = new CustomJacobian(this->i, this->j, this->lambda);
  form->ext = this->ext;
  return form;
}

double CustomWeakFormPicard::CustomResidual::value(int n, double *wt, Func<double> *u_ext[],
                                                   Func<double> *v, Geom<double> *e, ExtData<double> *ext) const 
{
  double result = 0;
  for (int i = 0; i < n; i++) 
  {
    result += wt[i] * lambda->value(ext->fn[0]->val[i]) 
                    * (u_ext[0]->dx[i] * v->dx[i] + u_ext[0]->dy[i] * v->dy[i]);
    result += wt[i] * f->value(e->x[i], e->y[i]) * v->val[i];
  }
  return result;
}

Ord CustomWeakFormPicard::CustomResidual::ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *v, 
                                              Geom<Ord> *e, ExtData<Ord> *ext) const 
{
  Ord result = Ord(0);
  for (int i = 0; i < n; i++) 
  {
    result += wt[i] * lambda->value(ext->fn[0]->val[i]) * (u_ext[0]->dx[i] 
                    * v->dx[i] + u_ext[0]->dy[i] * v->dy[i]);
    result += wt[i] * f->value(e->x[i], e->y[i]) * v->val[i];
  }
  return result;
}

VectorFormVol<double>* CustomWeakFormPicard::CustomResidual::clone()
{
  return new CustomResidual(this->i, this->lambda, this->f);
}

EssentialBoundaryCondition<double>::EssentialBCValueType CustomEssentialBCNonConst::get_value_type() const
{
  return EssentialBoundaryCondition<double>::BC_FUNCTION; 
}

double CustomEssentialBCNonConst::value(double x, double y, double n_x, double n_y, 
                                        double t_x, double t_y) const
{
  return (x+10) * (y+10) / 100.;
}
