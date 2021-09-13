#include "ConstraintRelaxationStrategyFactory.hpp"
#include "FeasibilityRestoration.hpp"
#include "l1Relaxation.hpp"

size_t ConstraintRelaxationStrategyFactory::get_number_variables(std::string_view constraint_relaxation_type, const Problem& problem) {
   if (constraint_relaxation_type == "feasibility-restoration") {
      return FeasibilityRestoration::get_number_variables(problem);
   }
   else if (constraint_relaxation_type == "l1-relaxation") {
      return l1Relaxation::get_number_variables(problem);
   }
   else {
      throw std::invalid_argument("ConstraintRelaxationStrategy type does not exist");
   }
}

std::unique_ptr<ConstraintRelaxationStrategy> ConstraintRelaxationStrategyFactory::create(std::string_view constraint_relaxation_type,
      Problem& problem, Subproblem& subproblem, const Options& options) {
   if (constraint_relaxation_type == "feasibility-restoration") {
      return std::make_unique<FeasibilityRestoration>(problem, subproblem, options);
   }
   else if (constraint_relaxation_type == "l1-relaxation") {
      const double initial_parameter = stod(options.at("l1_relaxation_initial_parameter"));
      const double decrease_factor = stod(options.at("l1_relaxation_decrease_factor"));
      const double epsilon1 = stod(options.at("l1_relaxation_epsilon1"));
      const double epsilon2 = stod(options.at("l1_relaxation_epsilon2"));
      l1RelaxationParameters parameters({initial_parameter, decrease_factor, epsilon1, epsilon2});
      return std::make_unique<l1Relaxation>(problem, subproblem, parameters, options);
   }
   else {
      throw std::invalid_argument("ConstraintRelaxationStrategy type does not exist");
   }
}