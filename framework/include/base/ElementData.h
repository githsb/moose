#ifndef ELEMENTDATA_H
#define ELEMENTDATA_H

//MOOSE includes
#include "Moose.h"
#include "MooseArray.h"

//libMesh includes
#include "transient_system.h"

//Forward Declarations
class MooseSystem;
class QGauss;
class DofMap;
class FEBase;
template<class T> class NumericVector;
template<class T> class DenseVector;
template<class T> class DenseSubVector;
template<class T> class DenseMatrix;

/**
 * One stop shop for all the data a Kernel class needs.
 *
 * _One_ of these will get built for each MooseSystem.
 */
class ElementData
{
public:
  ElementData(MooseSystem & moose_system);

  ~ElementData();

  void sizeEverything();

  void init();

  void initKernels();

  /**
   * Allows specification of per variable scaling factors.
   * The size of the vector MUST be the same as the number of Nonlinear Variables.
   * Should be called after Kernel::init() (because that sets the default scaling).
   * Can be called multiple times to change the scaling.
   * The initial scaling is just 1 for each variable.
   */
  void setVarScaling(std::vector<Real> scaling);
  
  void reinitKernels(THREAD_ID tid, const NumericVector<Number>& soln, const Elem * elem, DenseVector<Number> * Re, DenseMatrix<Number> * Ke);
  
  /**
   * The MooseSystem this Kernel is associated with.
   */
  MooseSystem & _moose_system;

  DofMap * _dof_map;

  std::vector<std::vector<unsigned int> > _dof_indices;

  /**
   * Interior finite element.
   */
  std::vector<std::map<FEType, FEBase*> > _fe;
  
  /**
   * Interior quadrature rule.
   */
  std::vector<QGauss *> _qrule;

  /**
   * Interior Jacobian pre-multiplied by the weight.
   */
  std::vector<std::map<FEType, const std::vector<Real> *> > _JxW;

  /**
   * Interior shape function.
   */
  std::vector<std::map<FEType, const std::vector<std::vector<Real> > *> > _phi;

  /**
   * Interior test function.
   *
   * Note that there is a different test function for each variable... allowing for modified
   * basis for things like SUPG and GLS.
   */
  std::vector<std::map<unsigned int, std::vector<std::vector<Real> > > > _test;

  /**
   * Gradient of interior shape function.
   */
  std::vector<std::map<FEType, const std::vector<std::vector<RealGradient> > *> > _dphi;

  /**
   * Second derivative of interior shape function.
   */
  std::vector<std::map<FEType, const std::vector<std::vector<RealTensor> > *> > _d2phi;

  /**
   * XYZ coordinates of quadrature points
   */
  std::vector<std::map<FEType, const std::vector<Point> *> > _q_point;

    /**
   * Value of the variables at the quadrature points.
   */
  MooseArray<MooseArray<MooseArray<Real> > > _var_vals;

  /**
   * Gradient of the variables at the quadrature points.
   */
  MooseArray<MooseArray<MooseArray<RealGradient> > > _var_grads;

  /**
   * Second derivatives of the variables at the quadrature points.
   */
  MooseArray<MooseArray<MooseArray<RealTensor> > > _var_seconds;

  /**
   * Value of the variables at the quadrature points.
   */
  MooseArray<MooseArray<MooseArray<Real> > > _var_vals_old;

  /**
   * Value of the variables at the quadrature points at t-2.
   */
  MooseArray<MooseArray<MooseArray<Real> > > _var_vals_older;

  /**
   * Gradient of the variables at the quadrature points.
   */
  MooseArray<MooseArray<MooseArray<RealGradient> > > _var_grads_old;

  /**
   * Gradient of the variables at the quadrature points.
   */
  MooseArray<MooseArray<MooseArray<RealGradient> > > _var_grads_older;

  /**
   * Value of the variables at the quadrature points.
   */
  MooseArray<MooseArray<MooseArray<Real> > > _aux_var_vals;

  /**
   * Gradient of the variables at the quadrature points.
   */
  MooseArray<MooseArray<MooseArray<RealGradient> > > _aux_var_grads;

  /**
   * Value of the variables at the quadrature points.
   */
  MooseArray<MooseArray<MooseArray<Real> > > _aux_var_vals_old;

  /**
   * Value of the variables at the quadrature points at t-2.
   */
  MooseArray<MooseArray<MooseArray<Real> > > _aux_var_vals_older;

  /**
   * Gradient of the variables at the quadrature points.
   */
  MooseArray<MooseArray<MooseArray<RealGradient> > > _aux_var_grads_old;

  /**
   * Gradient of the variables at the quadrature points.
   */
  MooseArray<MooseArray<MooseArray<RealGradient> > > _aux_var_grads_older;

  /**
   * Current element
   */
  std::vector<const Elem *> _current_elem;

  /**
   * Variable numbers of the variables.
   */
  std::vector<unsigned int> _var_nums;

  /**
   * Variable numbers of the auxiliary variables.
   */
  std::vector<unsigned int> _aux_var_nums;

    /**
   * Residual vectors for all variables.
   */
  std::vector<std::vector<DenseSubVector<Number> * > > _var_Res;
  
  /**
   * Jacobian matrices for all variables.
   */
  std::vector<std::vector<DenseMatrix<Number> * > > _var_Kes;

  /**
   * Dof Maps for all the variables.
   */
  std::vector<std::vector<std::vector<unsigned int> > > _var_dof_indices;

  /**
   * Dof Maps for all the auxiliary variables.
   */
  std::vector<std::vector<std::vector<unsigned int> > > _aux_var_dof_indices;

  /**
   * Pointer to the material that is valid for the current block.
   */
  std::vector<Material *> _material;

  /**
   * Scaling factors for each variable.
   */
  std::vector<Real> _scaling_factor;

  
};

#endif //ELEMENTDATA_H
