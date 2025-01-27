// Copyright (c) 2022 Charlie Vanaret
// Licensed under the MIT license. See LICENSE file in the project directory for details.

#ifndef UNO_GLOBALIZATIONMECHANISM_H
#define UNO_GLOBALIZATIONMECHANISM_H

#include "ingredients/constraint_relaxation_strategy/ConstraintRelaxationStrategy.hpp"
#include "optimization/Model.hpp"
#include "preprocessing/Scaling.hpp"
#include "tools/Statistics.hpp"

class GlobalizationMechanism {
public:
   explicit GlobalizationMechanism(ConstraintRelaxationStrategy& constraint_relaxation_strategy);
   virtual ~GlobalizationMechanism() = default;

   virtual void initialize(Statistics& statistics, Iterate& first_iterate) = 0;
   virtual std::tuple<Iterate, double> compute_acceptable_iterate(Statistics& statistics, Iterate& current_iterate) = 0;

   [[nodiscard]] size_t get_hessian_evaluation_count() const;
   [[nodiscard]] size_t get_number_subproblems_solved() const;

protected:
   // reference to allow polymorphism
   ConstraintRelaxationStrategy& constraint_relaxation_strategy; /*!< Constraint relaxation strategy */
   size_t number_iterations{0}; /*!< Current number of iterations */

   static Iterate assemble_trial_iterate(Iterate& current_iterate, Direction& direction, double step_length = 1.);
   static void print_warning(const char* message);
};

#endif // UNO_GLOBALIZATIONMECHANISM_H
