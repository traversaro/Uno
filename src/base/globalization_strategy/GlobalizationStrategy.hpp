#ifndef GLOBALIZATIONSTRATEGY_H
#define GLOBALIZATIONSTRATEGY_H

#include "Problem.hpp"
#include "Subproblem.hpp"
#include "Iterate.hpp"
#include "SubproblemSolution.hpp"
#include "Constraint.hpp"

/*! \class GlobalizationStrategy
 * \brief Step acceptance strategy
 *
 *  Strategy that accepts or declines a trial step (virtual class)
 */
class GlobalizationStrategy {
    public:
        /*!
         *  Constructor that takes an optimization problem and a tolerance
         * 
         * \param problem: optimization problem
         * \param constants: set of constants
         */
        GlobalizationStrategy(Subproblem& subproblem, double tolerance);
        virtual ~GlobalizationStrategy();

        Subproblem& subproblem;
        double tolerance; /*!< Tolerance of the termination criteria */

        virtual SubproblemSolution compute_step(Problem& problem, Iterate& current_iterate, std::vector<Range>& variables_bounds) = 0;

        /*!
         *  Check the validity of a step
         *  Purely virtual method (only implemented in subclasses)
         */
        virtual bool check_step(Problem& problem, Iterate& current_iterate, SubproblemSolution& solution, double step_length = 1.) = 0;
        virtual Iterate initialize(Problem& problem, std::vector<double>& x, Multipliers& multipliers, bool use_trust_region) = 0;
        
    protected:
        virtual void compute_measures(Problem& problem, Iterate& iterate) = 0;
        std::vector<double> compute_lagrangian_gradient(Problem& problem, Iterate& current_iterate, double objective_multiplier, Multipliers& multipliers);
        double compute_complementarity_error(const Problem& problem, Iterate& iterate);
        // specific to the strategy (objective multiplier depends on the strategy)
        double compute_KKT_error(Problem& problem, Iterate& iterate, double objective_mutiplier);
};

#endif // GLOBALIZATIONSTRATEGY_H
