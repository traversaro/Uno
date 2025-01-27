// Copyright (c) 2022 Charlie Vanaret
// Licensed under the MIT license. See LICENSE file in the project directory for details.

#ifndef UNO_L1RELAXEDPROBLEM_H
#define UNO_L1RELAXEDPROBLEM_H

#include <vector>
#include <cmath>
#include "NonlinearProblem.hpp"
#include "linear_algebra/SparseVector.hpp"
#include "tools/Range.hpp"
#include "tools/Infinity.hpp"

struct ElasticVariables {
   SparseVector<size_t> positive;
   SparseVector<size_t> negative;
   explicit ElasticVariables(size_t capacity): positive(capacity), negative(capacity) {}
   [[nodiscard]] size_t size() const { return this->positive.size() + this->negative.size(); }
};

class l1RelaxedProblem: public NonlinearProblem {
public:
   l1RelaxedProblem(const Model& model, double objective_multiplier, double constraint_violation_coefficient, bool use_proximal_term);

   [[nodiscard]] double get_objective_multiplier() const override;
   [[nodiscard]] double evaluate_objective(Iterate& iterate) const override;
   void evaluate_objective_gradient(Iterate& iterate, SparseVector<double>& objective_gradient) const override;
   void evaluate_constraints(Iterate& iterate, std::vector<double>& constraints) const override;
   void evaluate_constraint_jacobian(Iterate& iterate, std::vector<SparseVector<double>>& constraint_jacobian) const override;
   void evaluate_lagrangian_hessian(const std::vector<double>& x, const std::vector<double>& multipliers, SymmetricMatrix& hessian) const override;

   [[nodiscard]] double get_variable_lower_bound(size_t i) const override;
   [[nodiscard]] double get_variable_upper_bound(size_t i) const override;
   [[nodiscard]] double get_constraint_lower_bound(size_t j) const override;
   [[nodiscard]] double get_constraint_upper_bound(size_t j) const override;

   [[nodiscard]] size_t get_maximum_number_objective_gradient_nonzeros() const override;
   [[nodiscard]] size_t get_maximum_number_jacobian_nonzeros() const override;
   [[nodiscard]] size_t get_maximum_number_hessian_nonzeros() const override;

   [[nodiscard]] const std::vector<size_t>& get_violated_linearized_constraints(const std::vector<double>& x);
   [[nodiscard]] double compute_linearized_constraint_violation(const std::vector<double>& x) const;
   [[nodiscard]] double compute_linearized_constraint_violation(const std::vector<double>& x, const std::vector<double>& dx) const;

   // parameterization
   void set_objective_multiplier(double new_objective_multiplier);
   void set_proximal_coefficient(double new_proximal_coefficient);
   void set_proximal_reference_point(const std::vector<double>& new_proximal_reference_point);

   // void set_elastic_variables(Iterate& iterate) const;
   void set_elastic_variables(Iterate& iterate, const std::function<void (Iterate&, size_t, size_t, double, double)>& elastic_setting_function) const;

protected:
   double objective_multiplier;
   // elastic variables
   ElasticVariables elastic_variables;
   double constraint_violation_coefficient;
   // proximal term
   const bool use_proximal_term;
   double proximal_coefficient{0.};
   std::vector<double> proximal_reference_point;
   std::vector<size_t> violated_constraints{};

   [[nodiscard]] static size_t count_elastic_variables(const Model& model);
   void generate_elastic_variables();
   [[nodiscard]] double get_proximal_weight(size_t i) const;
};

inline l1RelaxedProblem::l1RelaxedProblem(const Model& model, double objective_multiplier, double constraint_violation_coefficient,
         bool use_proximal_term):
      NonlinearProblem(model, model.number_variables + l1RelaxedProblem::count_elastic_variables(model), model.number_constraints),
      objective_multiplier(objective_multiplier),
      // elastic variables
      elastic_variables(this->number_constraints),
      constraint_violation_coefficient(constraint_violation_coefficient),
      use_proximal_term(use_proximal_term),
      proximal_reference_point(model.number_variables) {
   // register equality and inequality constraints
   this->model.equality_constraints.for_each([&](size_t j, size_t i) {
      this->equality_constraints.insert(j, i);
   });
   this->model.inequality_constraints.for_each([&](size_t j, size_t i) {
      this->inequality_constraints.insert(j, i);
   });

   // generate elastic variables
   this->generate_elastic_variables();

   // figure out bounded variables
   for (size_t i: this->model.lower_bounded_variables) {
      this->lower_bounded_variables.push_back(i);
   }
   for (size_t i: this->model.upper_bounded_variables) {
      this->upper_bounded_variables.push_back(i);
   }
   this->elastic_variables.positive.for_each_value([&](size_t elastic_index) {
      this->lower_bounded_variables.push_back(elastic_index);
   });
   this->elastic_variables.negative.for_each_value([&](size_t elastic_index) {
      this->lower_bounded_variables.push_back(elastic_index);
   });
   this->violated_constraints.reserve(this->number_constraints);
}

inline double l1RelaxedProblem::get_objective_multiplier() const {
   return this->objective_multiplier;
}

// return rho*f(x) + coeff*||c(x)||_1 + proximal term
inline double l1RelaxedProblem::evaluate_objective(Iterate& iterate) const {
   double objective = 0.;

   // scaled objective: rho*f(x)
   if (this->objective_multiplier != 0.) {
      iterate.evaluate_objective(this->model);
      objective += this->objective_multiplier*iterate.original_evaluations.objective;
   }

   // scaled constraint violation: coeff*||c(x)||_1
   iterate.evaluate_constraints(this->model);
   iterate.constraint_violation = this->model.compute_constraint_violation(iterate.original_evaluations.constraints, L1_NORM);
   objective += this->constraint_violation_coefficient * iterate.constraint_violation;

   // proximal term
   if (this->use_proximal_term && 0. < this->proximal_coefficient) {
      double proximal_term = 0.;
      for (size_t i = 0; i < this->model.number_variables; i++) {
         const double weight = this->get_proximal_weight(i);
         // weighted distance between trial iterate and current iterate
         proximal_term += std::pow(weight * (iterate.primals[i] - this->proximal_reference_point[i]), 2);
      }
      objective += this->proximal_coefficient/2.*proximal_term;
   }
   return objective;
}

inline void l1RelaxedProblem::evaluate_objective_gradient(Iterate& iterate, SparseVector<double>& objective_gradient) const {
   // scale nabla f(x) by rho
   if (this->objective_multiplier != 0.) {
      iterate.evaluate_objective_gradient(this->model);
      objective_gradient = iterate.original_evaluations.objective_gradient;
      scale(objective_gradient, this->objective_multiplier);
   }
   else {
      objective_gradient.clear();
   }

   // elastic contribution
   const auto insert_elastic_derivative = [&](size_t elastic_index) {
      objective_gradient.insert(elastic_index, this->constraint_violation_coefficient);
   };
   this->elastic_variables.positive.for_each_value(insert_elastic_derivative);
   this->elastic_variables.negative.for_each_value(insert_elastic_derivative);

   // proximal term
   if (this->use_proximal_term && 0. < this->proximal_coefficient) {
      for (size_t i = 0; i < this->model.number_variables; i++) {
         const double weight = this->get_proximal_weight(i);
         // measure weighted distance between trial iterate and current iterate
         const double derivative = this->proximal_coefficient * std::pow(weight, 2) * (iterate.primals[i] - this->proximal_reference_point[i]);
         objective_gradient.insert(i, derivative);
      }
   }
}

inline void l1RelaxedProblem::evaluate_constraints(Iterate& iterate, std::vector<double>& constraints) const {
   iterate.evaluate_constraints(this->model);
   copy_from(constraints, iterate.original_evaluations.constraints);
   // add the contribution of the elastics
   this->elastic_variables.positive.for_each([&](size_t j, size_t elastic_index) {
      constraints[j] -= iterate.primals[elastic_index];
   });
   this->elastic_variables.negative.for_each([&](size_t j, size_t elastic_index) {
      constraints[j] += iterate.primals[elastic_index];
   });
}

inline void l1RelaxedProblem::evaluate_constraint_jacobian(Iterate& iterate, std::vector<SparseVector<double>>& constraint_jacobian) const {
   iterate.evaluate_constraint_jacobian(this->model);
   constraint_jacobian = iterate.original_evaluations.constraint_jacobian;
   // add the contribution of the elastics
   this->elastic_variables.positive.for_each([&](size_t j, size_t elastic_index) {
      constraint_jacobian[j].insert(elastic_index, -1.);
   });
   this->elastic_variables.negative.for_each([&](size_t j, size_t elastic_index) {
      constraint_jacobian[j].insert(elastic_index, 1.);
   });
}

inline void l1RelaxedProblem::evaluate_lagrangian_hessian(const std::vector<double>& x, const std::vector<double>& multipliers,
      SymmetricMatrix& hessian) const {
   this->model.evaluate_lagrangian_hessian(x, this->objective_multiplier, multipliers, hessian);

   // add proximal term for the original variables
   if (this->use_proximal_term && 0. < this->proximal_coefficient) {
      for (size_t i = 0; i < this->model.number_variables; i++) {
         const double weight = this->get_proximal_weight(i);
         const double diagonal_term = this->proximal_coefficient * std::pow(weight, 2);
         hessian.insert(diagonal_term, i, i);
      }
   }
   // extend the dimension of the Hessian by finalizing the remaining columns (note: the elastics do not enter the Hessian)
   for (size_t j = this->model.number_variables; j < this->number_variables; j++) {
      hessian.finalize_column(j);
   }
}

inline double l1RelaxedProblem::compute_linearized_constraint_violation(const std::vector<double>& x) const {
   double constraint_violation = 0.;
   // l1 residual of the linearized constraints: sum of elastic variables
   auto add_elastic_contribution = [&](size_t i) {
      constraint_violation += x[i];
   };
   this->elastic_variables.positive.for_each_value(add_elastic_contribution);
   this->elastic_variables.negative.for_each_value(add_elastic_contribution);
   assert(0 <= constraint_violation && "The linearized constraint violation should not be negative");
   return constraint_violation;
}

inline double l1RelaxedProblem::compute_linearized_constraint_violation(const std::vector<double>& x, const std::vector<double>& dx) const {
   double constraint_violation = 0.;
   // l1 residual of the linearized constraints: sum of elastic variables
   auto add_elastic_contribution = [&](size_t i) {
      constraint_violation += (x[i] + dx[i]);
   };
   this->elastic_variables.positive.for_each_value(add_elastic_contribution);
   this->elastic_variables.negative.for_each_value(add_elastic_contribution);
   assert(0 <= constraint_violation && "The linearized constraint violation should not be negative");
   return constraint_violation;
}

inline double l1RelaxedProblem::get_variable_lower_bound(size_t i) const {
   if (i < this->model.number_variables) { // original variable
      return this->model.get_variable_lower_bound(i);
   }
   else { // elastic variable in [0, +inf[
      return 0.;
   }
}

inline double l1RelaxedProblem::get_variable_upper_bound(size_t i) const {
   if (i < this->model.number_variables) { // original variable
      return this->model.get_variable_upper_bound(i);
   }
   else { // elastic variable in [0, +inf[
      return INF;
   }
}

inline double l1RelaxedProblem::get_constraint_lower_bound(size_t j) const {
   return this->model.get_constraint_lower_bound(j);
}

inline double l1RelaxedProblem::get_constraint_upper_bound(size_t j) const {
   return this->model.get_constraint_upper_bound(j);
}

inline size_t l1RelaxedProblem::get_maximum_number_objective_gradient_nonzeros() const {
   size_t number_nonzeros = 0;

   // objective contribution
   if (this->objective_multiplier != 0.) {
      number_nonzeros += this->model.get_maximum_number_objective_gradient_nonzeros();
   }

   // elastic contribution
   number_nonzeros += this->elastic_variables.size();

   // proximal contribution
   if (this->use_proximal_term) {
      number_nonzeros += this->model.number_variables;
   }
   return number_nonzeros;
}

inline size_t l1RelaxedProblem::get_maximum_number_jacobian_nonzeros() const {
   return this->model.get_maximum_number_jacobian_nonzeros() + this->elastic_variables.size();
}

inline size_t l1RelaxedProblem::get_maximum_number_hessian_nonzeros() const {
   // add the proximal term
   return this->model.get_maximum_number_hessian_nonzeros() + (this->use_proximal_term ? this->model.number_variables : 0);
}

inline void l1RelaxedProblem::set_objective_multiplier(double new_objective_multiplier) {
   // update the objective multiplier
   this->objective_multiplier = new_objective_multiplier;
}

inline void l1RelaxedProblem::set_proximal_coefficient(double new_proximal_coefficient) {
   assert(0. <= new_proximal_coefficient && "The proximal coefficient is negative");
   // update the proximal coefficient
   this->proximal_coefficient = new_proximal_coefficient;
}

inline void l1RelaxedProblem::set_proximal_reference_point(const std::vector<double>& new_proximal_reference_point) {
   assert(this->model.number_variables <= new_proximal_reference_point.size() && "The proximal reference point is not long enough");
   // update the proximal reference point
   copy_from(this->proximal_reference_point, new_proximal_reference_point, this->model.number_variables);
}

inline size_t l1RelaxedProblem::count_elastic_variables(const Model& model) {
   size_t number_elastic_variables = 0;
   // if the subproblem uses slack variables, the bounds of the constraints are [0, 0]
   for (size_t j = 0; j < model.number_constraints; j++) {
      if (is_finite(model.get_constraint_lower_bound(j))) {
         number_elastic_variables++;
      }
      if (is_finite(model.get_constraint_upper_bound(j))) {
         number_elastic_variables++;
      }
   }
   return number_elastic_variables;
}

inline void l1RelaxedProblem::generate_elastic_variables() {
   // generate elastic variables p and n on the fly to relax the constraints
   // if the subproblem uses slack variables, the bounds of the constraints are [0, 0]
   size_t elastic_index = this->model.number_variables;
   for (size_t j = 0; j < this->model.number_constraints; j++) {
      if (is_finite(this->model.get_constraint_upper_bound(j))) {
         // nonnegative variable p that captures the positive part of the constraint violation
         this->elastic_variables.positive.insert(j, elastic_index);
         elastic_index++;
      }
      if (is_finite(this->model.get_constraint_lower_bound(j))) {
         // nonpositive variable n that captures the negative part of the constraint violation
         this->elastic_variables.negative.insert(j, elastic_index);
         elastic_index++;
      }
   }
}

// construct the list of linearized constraints that are violated
inline const std::vector<size_t>& l1RelaxedProblem::get_violated_linearized_constraints(const std::vector<double>& x) {
   // TODO: actually evaluate the linearized constraints
   this->violated_constraints.clear();
   const auto find_violated_constraints = [&](size_t j, size_t elastic_index) {
      if (0. < x[elastic_index]) {
         this->violated_constraints.push_back(j);
      }
   };
   this->elastic_variables.positive.for_each(find_violated_constraints);
   this->elastic_variables.negative.for_each(find_violated_constraints);
   return this->violated_constraints;
}

inline double l1RelaxedProblem::get_proximal_weight(size_t i) const {
   // weight of each diagonal term of the proximal term
   return std::min(1., 1. / std::abs(this->proximal_reference_point[i]));
}

inline void l1RelaxedProblem::set_elastic_variables(Iterate& iterate, const std::function<void (Iterate&, size_t, size_t, double, double)>&
      elastic_setting_function) const {
   iterate.set_number_variables(this->number_variables);
   this->elastic_variables.positive.for_each([&](size_t j, size_t elastic_index) {
      elastic_setting_function(iterate, j, elastic_index, -1., this->constraint_violation_coefficient);
   });
   this->elastic_variables.negative.for_each([&](size_t j, size_t elastic_index) {
      elastic_setting_function(iterate, j, elastic_index, 1., this->constraint_violation_coefficient);
   });
}

#endif // UNO_L1RELAXEDPROBLEM_H