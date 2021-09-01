#include <cassert>
#include "l1Relaxation.hpp"
#include "ingredients/strategy/GlobalizationStrategyFactory.hpp"
#include "ingredients/subproblem/SubproblemFactory.hpp"

l1Relaxation::l1Relaxation(Problem& problem, Subproblem& subproblem, const Options& options) :
      ConstraintRelaxationStrategy(subproblem),
      globalization_strategy(GlobalizationStrategyFactory::create(options.at("strategy"), options)),
      penalty_parameter(stod(options.at("l1_relaxation_initial_parameter"))),
      parameters({10., 0.1, 0.1}),
      elastic_variables(ConstraintRelaxationStrategy::count_elastic_variables(problem)) {
   assert(this->subproblem.number_variables == l1Relaxation::get_number_variables(problem) && "The number of variables is inconsistent");

   // generate elastic variables to relax the constraints
   ConstraintRelaxationStrategy::generate_elastic_variables(problem, this->elastic_variables);
   // add bounds for nonnegative elastic variables to the subproblem
   this->set_elastic_bounds_in_subproblem(problem, this->elastic_variables.size());

   // elastic variables are temporary and are discarded when passed to mechanism
   this->number_variables -= l1Relaxation::count_elastic_variables(problem);
}

Iterate l1Relaxation::initialize(Statistics& statistics, const Problem& problem, std::vector<double>& x, Multipliers& multipliers) {
   statistics.add_column("penalty param.", Statistics::double_width, 4);

   // initialize the subproblem
   Iterate first_iterate = this->subproblem.generate_initial_iterate(statistics, problem, x, multipliers);
   this->subproblem.compute_errors(problem, first_iterate, this->penalty_parameter);

   this->globalization_strategy->initialize(statistics, first_iterate);
   return first_iterate;
}

void l1Relaxation::generate_subproblem(const Problem& problem, Iterate& current_iterate, double trust_region_radius) {
   // preprocess the subproblem: scale the objective gradient and introduce the elastic variables
   // scale the objective gradient and (possibly) and Hessian
   this->subproblem.generate(problem, current_iterate, this->penalty_parameter, trust_region_radius);
   this->add_elastic_variables_to_subproblem(this->elastic_variables);
}

Direction l1Relaxation::compute_feasible_direction(Statistics& statistics, const Problem& problem, Iterate& current_iterate) {
   DEBUG << "penalty parameter: " << this->penalty_parameter << "\n";
   // use Byrd's steering rules to update the penalty parameter and compute descent directions
   Direction direction = this->compute_byrd_steering_rule(statistics, problem, current_iterate);

   // remove the temporary elastic variables
   this->remove_elastic_variables(problem, direction);
   return direction;
}

Direction l1Relaxation::solve_feasibility_problem(Statistics& statistics, const Problem& problem, Iterate& current_iterate,
      Direction& /*direction*/) {
   this->update_objective_multiplier(problem, current_iterate, 0.);
   return this->subproblem.solve(statistics, problem, current_iterate);
}

bool l1Relaxation::is_acceptable(Statistics& statistics, const Problem& problem, Iterate& current_iterate, Iterate& trial_iterate,
      const Direction& direction, double step_length) {
   // check if subproblem definition changed
   if (this->subproblem.subproblem_definition_changed) {
      this->globalization_strategy->reset();
      this->subproblem.subproblem_definition_changed = false;
      this->subproblem.compute_progress_measures(problem, current_iterate);
   }
   const double step_norm = step_length * direction.norm;

   bool accept = false;
   if (step_norm == 0.) {
      accept = true;
   }
   else {
      trial_iterate.compute_constraints(problem);
      // compute the predicted reduction (a mixture of the subproblem's and of the l1 relaxation's)
      const double predicted_reduction = this->compute_predicted_reduction(problem, current_iterate, direction, step_length);
      // invoke the globalization strategy for acceptance
      accept = this->globalization_strategy->check_acceptance(statistics, current_iterate.progress, trial_iterate.progress,
            this->penalty_parameter, predicted_reduction);
   }
   if (accept) {
      statistics.add_statistic("penalty param.", this->penalty_parameter);

      // compute the residuals
      trial_iterate.compute_objective(problem);
      this->subproblem.compute_errors(problem, trial_iterate, direction.objective_multiplier);
   }
   return accept;
}

void l1Relaxation::update_objective_multiplier(const Problem& problem, const Iterate& current_iterate, double objective_multiplier) {
   this->subproblem.update_objective_multiplier(problem, current_iterate, objective_multiplier);
   // add the positive elastic variables
   elastic_variables.positive.for_each_value([&](size_t i) {
      this->subproblem.objective_gradient.insert(i, 1.);
   });
   elastic_variables.negative.for_each_value([&](size_t i) {
      this->subproblem.objective_gradient.insert(i, 1.);
   });

   /*
   for (const auto& element: elastic_variables.positive) {
      const size_t i = element.second;
      this->subproblem.objective_gradient[i] = 1.;
   }
   // add the negative elastic variables
   for (const auto& element: elastic_variables.negative) {
      const size_t i = element.second;
      this->subproblem.objective_gradient[i] = 1.;
   }
    */
}

Direction l1Relaxation::solve_subproblem(Statistics& statistics, const Problem& problem, Iterate& current_iterate) {
   Direction direction = this->subproblem.solve(statistics, problem, current_iterate);
   assert(direction.constraint_partition.infeasible.empty() && "Infeasible constraints found, although direction is feasible");
   direction.objective_multiplier = this->penalty_parameter;
   DEBUG << "\n" << direction;
   return direction;
}

Direction l1Relaxation::solve_subproblem(Statistics& statistics, const Problem& problem, Iterate& current_iterate, double objective_multiplier) {
   this->update_objective_multiplier(problem, current_iterate, objective_multiplier);
   Direction direction = this->subproblem.solve(statistics, problem, current_iterate);
   assert(direction.constraint_partition.infeasible.empty() && "Infeasible constraints found, although direction is feasible");
   direction.objective_multiplier = objective_multiplier;
   DEBUG << "\n" << direction;
   return direction;
}

Direction l1Relaxation::compute_byrd_steering_rule(Statistics& statistics, const Problem& problem, Iterate& current_iterate) {
   /* stage a: compute the step within trust region */
   Direction direction = this->solve_subproblem(statistics, problem, current_iterate);

   /* penalty update: if penalty parameter is already 0, no need to decrease it */
   if (0. < this->penalty_parameter) {
      /* check infeasibility */
      double linearized_residual = this->compute_linearized_constraint_residual(direction.x);
      DEBUG << "Linearized residual mk(dk): " << linearized_residual << "\n\n";

      // if problem had to be relaxed
      if (linearized_residual != 0.) {
         const double current_penalty_parameter = this->penalty_parameter;

         /* stage c: compute the lowest possible constraint violation (penalty = 0) */
         DEBUG << "Compute ideal solution (param = 0):\n";
         Direction direction_lowest_violation = this->solve_subproblem(statistics, problem, current_iterate, 0.);
         const double residual_lowest_violation = this->compute_linearized_constraint_residual(direction_lowest_violation.x);
         DEBUG << "Ideal linearized residual mk(dk): " << residual_lowest_violation << "\n\n";

         if (!(0. < current_iterate.errors.constraints && residual_lowest_violation == current_iterate.errors.constraints)) {
            /* compute the ideal error (with a zero penalty parameter) */
            const double error_lowest_violation = l1Relaxation::compute_error(problem, current_iterate, direction_lowest_violation.multipliers, 0.);
            DEBUG << "Ideal error: " << error_lowest_violation << "\n";
            if (error_lowest_violation == 0.) {
               /* stage f: update the penalty parameter */
               this->penalty_parameter = 0.;
               direction = direction_lowest_violation;
            }
            else {
               /* stage f: update the penalty parameter */
               const double updated_penalty_parameter = this->penalty_parameter;
               const double term = error_lowest_violation / std::max(1., current_iterate.errors.constraints);
               this->penalty_parameter = std::min(this->penalty_parameter, term * term);
               if (this->penalty_parameter < updated_penalty_parameter) {
                  if (this->penalty_parameter == 0.) {
                     direction = direction_lowest_violation;
                  }
                  else {
                     direction = this->solve_subproblem(statistics, problem, current_iterate, this->penalty_parameter);
                  }
               }
   
   
   
               /* decrease penalty parameter to satisfy 2 conditions */
               bool condition1 = false, condition2 = false;
               while (!condition2) {
                  if (!condition1) {
                     /* stage d: reach a fraction of the ideal decrease */
                     if ((residual_lowest_violation == 0. && linearized_residual == 0) || (residual_lowest_violation != 0. &&
                     current_iterate.errors.constraints - linearized_residual >= this->parameters.epsilon1 *
                     (current_iterate.errors.constraints - residual_lowest_violation))) {
                        condition1 = true;
                        DEBUG << "Condition 1 is true\n";
                     }
                  }
                  /* stage e: further decrease penalty parameter if necessary */
                  if (condition1 && current_iterate.errors.constraints - direction.objective >=
                                    this->parameters.epsilon2 * (current_iterate.errors.constraints - direction_lowest_violation.objective)) {
                     condition2 = true;
                     DEBUG << "Condition 2 is true\n";
                  }
                  if (!condition2) {
                     this->penalty_parameter /= this->parameters.tau;
                     if (this->penalty_parameter < 1e-10) {
                        this->penalty_parameter = 0.;
                        condition2 = true;
                     }
                     else {
                        DEBUG << "\nAttempting to solve with penalty parameter " << this->penalty_parameter << "\n";
                        direction = this->solve_subproblem(statistics, problem, current_iterate, this->penalty_parameter);
   
                        linearized_residual = this->compute_linearized_constraint_residual(direction.x);
                        DEBUG << "Linearized residual mk(dk): " << linearized_residual << "\n\n";
                     }
                  }
               }
            } // end else
         }

         if (this->penalty_parameter < current_penalty_parameter) {
            DEBUG << "\n*** Penalty parameter updated to " << this->penalty_parameter << "\n";
            this->globalization_strategy->reset();
         }
      }
   }
   return direction;
}

size_t l1Relaxation::get_number_variables(const Problem& problem) {
   return problem.number_variables + l1Relaxation::count_elastic_variables(problem);
}

double l1Relaxation::compute_linearized_constraint_residual(std::vector<double>& direction) {
   double residual = 0.;
   // l1 residual of the linearized constraints: sum of elastic variables
   this->elastic_variables.positive.for_each_value([&](size_t i) {
      residual += direction[i];
   });
   this->elastic_variables.negative.for_each_value([&](size_t i) {
      residual += direction[i];
   });

   /*
   for (const auto& element: this->elastic_variables.positive) {
      size_t i = element.second;
      residual += direction[i];
   }
   for (const auto& element: this->elastic_variables.negative) {
      size_t i = element.second;
      residual += direction[i];
   }
    */
   return residual;
}

/* measure that combines KKT error and complementarity error */
double l1Relaxation::compute_error(const Problem& problem, Iterate& iterate, Multipliers& multipliers, double penalty_parameter) const {
   /* complementarity error */
   double error = this->subproblem.compute_complementarity_error(problem, iterate, multipliers);
   /* KKT error */
   std::vector<double> lagrangian_gradient = iterate.lagrangian_gradient(problem, penalty_parameter, multipliers);
   error += norm_1(lagrangian_gradient);
   return error;
}

void l1Relaxation::remove_elastic_variables(const Problem& problem, Direction& direction) {
   // compute set of satisfied/violated constraints

   /* remove p and n */
   // TODO change that!!!
   direction.x.resize(problem.number_variables);
   direction.multipliers.lower_bounds.resize(problem.number_variables);
   direction.multipliers.upper_bounds.resize(problem.number_variables);
   direction.norm = norm_inf(direction.x);

   //elastic_variables.positive.for_each([&](size_t j, size_t i) {
      //this->subproblem.objective_gradient.erase(i);
      //this->subproblem.constraints_jacobian[j].erase(i);
   //});
   //elastic_variables.negative.for_each([&](size_t j, size_t i) {
      //this->subproblem.objective_gradient.erase(i);
      //this->subproblem.constraints_jacobian[j].erase(i);
   //});
   /*
   // remove contribution of positive part variables
   for (auto&[j, i]: elastic_variables.positive) {
      this->subproblem.objective_gradient.erase(i);
      this->subproblem.constraints_jacobian[j].erase(i);
   }
   // remove contribution of negative part variables
   for (auto&[j, i]: elastic_variables.negative) {
      this->subproblem.objective_gradient.erase(i);
      this->subproblem.constraints_jacobian[j].erase(i);
   }
    */
}

double l1Relaxation::compute_predicted_reduction(const Problem& problem, Iterate& current_iterate, const Direction& direction, double step_length) {
   // compute the predicted reduction of the l1 relaxation as a postprocessing of the predicted reduction of the subproblem
   if (step_length == 1.) {
      return current_iterate.errors.constraints + this->subproblem.compute_predicted_reduction(direction, step_length);;
   }
   else {
      // determine the linearized constraint violation term: c(x_k) + alpha*\nabla c(x_k)^T d
      auto residual_function = [&](size_t j) {
         const double component_j = current_iterate.constraints[j] + step_length * dot(direction.x, current_iterate.constraints_jacobian[j]);
         return problem.compute_constraint_violation(component_j, j);
      };
      const double linearized_constraint_violation = norm_1(residual_function, problem.number_constraints);
      return current_iterate.errors.constraints - linearized_constraint_violation + this->subproblem.compute_predicted_reduction(direction,
            step_length);
   }
}

void l1Relaxation::recover_l1qp_active_set_(const Problem& problem, const Direction& direction) {
   // remove extra variables p and n
   for (size_t i = problem.number_variables; i < direction.x.size(); i++) {
      // TODO
      //direction.active_set.bounds.at_lower_bound.erase(i);
      //direction.active_set.bounds.at_upper_bound.erase(i);
   }
   // constraints: only when p-n = 0
   /*
   for (size_t j = 0; j < direction.multipliers.constraints.size(); j++) {
      // compute constraint violation
      double constraint_violation = 0.;
      try {
         size_t i = this->elastic_variables.positive.at(j);
         constraint_violation += direction.x[i];
      }
      catch (const std::out_of_range& e) {
      }
      try {
         size_t i = this->elastic_variables.negative.at(j);
         constraint_violation += direction.x[i];
      }
      catch (const std::out_of_range& e) {
      }
      // update active set
      if (0. < constraint_violation) {
         // TODO
         //direction.active_set.constraints.at_lower_bound.erase(j);
         //direction.active_set.constraints.at_upper_bound.erase(j);
      }
   }
    */
}