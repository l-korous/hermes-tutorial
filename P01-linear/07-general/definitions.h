#include "hermes2d.h"

using namespace Hermes;
using namespace Hermes::Hermes2D;
using namespace Hermes::Hermes2D::WeakFormsH1;
using namespace Hermes::Hermes2D::Views;

/* Essential boundary conditions */

class CustomEssentialBCNonConst : public EssentialBoundaryCondition<double> 
{
public:
  CustomEssentialBCNonConst(std::string marker);

  inline EssentialBCValueType  get_value_type() const;

  virtual double value(double x, double y, double n_x, double n_y, 
                       double t_x, double t_y) const;
};

/* Weak forms */

class CustomWeakFormGeneral : public WeakForm<double> 
{
public:
  CustomWeakFormGeneral(std::string bdy_vertical);

private:
  class MatrixFormVolGeneral : public MatrixFormVol<double> 
  {
  public:
    MatrixFormVolGeneral(int i, int j);

    virtual double value(int n, double *wt, Func<double> *u_ext[], Func<double> *u, 
                         Func<double> *v, Geom<double> *e, ExtData<double> *ext) const;

    virtual Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *u, Func<Ord> *v, 
                    Geom<Ord> *e, ExtData<Ord> *ext) const;

    MatrixFormVol<double>* clone();
  };

  class VectorFormVolGeneral : public VectorFormVol<double> 
  {
  public:
    VectorFormVolGeneral(int i);

    virtual double value(int n, double *wt, Func<double> *u_ext[], Func<double> *v, 
                         Geom<double> *e, ExtData<double> *ext) const;

    virtual Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *v, 
                    Geom<Ord> *e, ExtData<Ord> *ext) const;

    VectorFormVol<double>* clone();
  private:
    double rhs(double x, double y) const;
  };

  class VectorFormSurfGeneral : public VectorFormSurf<double> 
  {
  public:
    VectorFormSurfGeneral(int i, std::string area = HERMES_ANY);

    virtual double value(int n, double *wt, Func<double> *u_ext[], Func<double> *v, 
                         Geom<double> *e, ExtData<double> *ext) const;

    virtual Ord ord(int n, double *wt, Func<Ord> *u_ext[], Func<Ord> *v, 
                    Geom<Ord> *e, ExtData<Ord> *ext) const;

    VectorFormSurf<double>* clone();
  private:
    double g_N(double x, double y) const;
  };
};
