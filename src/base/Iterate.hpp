#ifndef ITERATE_H
#define ITERATE_H

#include <ostream>
#include <vector>
#include <map>
#include "Problem.hpp"
#include "Constraint.hpp"
#include "Matrix.hpp"

enum OptimalityStatus {NOT_OPTIMAL = 0,
						KKT_POINT, /* feasible stationary point */
						FJ_POINT, /* infeasible stationary point */
						FEASIBLE_SMALL_STEP,
						INFEASIBLE_SMALL_STEP};

std::ostream& operator<< (std::ostream &stream, OptimalityStatus& status);

/*! \class Iterate
* \brief Optimization iterate
*
*  Point and its evaluations during an optimization process
*/
class Iterate {
	public:
		/*!
         *  Constructor
         */
		Iterate(Problem& problem, std::vector<double>& x, std::vector<double>& multipliers);
		
		std::vector<double> x; /*!< \f$\mathbb{R}^n\f$ primal variables */
		std::vector<double> multipliers; /*!< \f$\mathbb{R}^{n+m}\f$ Lagrange multipliers/dual variables */
		
		double objective; /*!< Objective value */
		std::vector<double> constraints; /*!< Constraint values (size \f$m)\f$ */

		OptimalityStatus status;
		double residual; /*!< Constraint residual */
		double KKTerror;
		double complementarity_error;
		
		double infeasibility_measure;
		double feasibility_measure;

		std::map<int,double> objective_gradient; /*!< Sparse Jacobian of the objective */
		std::vector<std::map<int,double> > constraint_jacobian; /*!< Sparse Jacobian of the constraints */
		bool is_objective_gradient_computed; /*!< Flag that indicates if the objective gradient has already been computed */
		bool is_constraint_jacobian_computed; /*!< Flag that indicates if the constraint Jacobian has already been computed */
		
		Matrix hessian; /*!< Sparse Lagrangian Hessian */
		bool is_hessian_computed; /*!< Flag that indicates if the Hessian has already been computed */
		
		void set_objective_gradient(std::map<int,double>& objective_gradient);
		void compute_constraint_jacobian(Problem& problem);
		
		/*!
         *  Compute the Hessian in a lazy way: the Hessian is computed only when required and stored
         *  The Hessian is stored in CSC (Compressed Sparse Column)
         */
		void compute_hessian(Problem& problem, double obj_multiplier, std::vector<double>& constraint_multipliers);
		
		friend std::ostream& operator<< (std::ostream &stream, Iterate& iterate);
};

#endif // ITERATE_H
