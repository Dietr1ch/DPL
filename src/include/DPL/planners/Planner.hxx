# pragma once

// Includes
// ========
// DPL
#include <DPL/utils/types.hxx>
#include <DPL/planners/Node.hxx>




/**
 * \brief DPL Planner interface
 */
class Planner {

  virtual ~Planner() {}


  // Setup
  // =====
  /**
   * \brief sets the goal of search (planner will automatically decide whether it needs to replan from scratch)
   */
  virtual bool setGoal(StateID goalID) = 0;

  /**
   * \brief sets the start of search (planner will automatically decide whether it needs to replan from scratch)
   */
  virtual bool setStart(StateID startID) = 0;



  // Solve
  // =====
  /**
   * \brief Solves given problem eventually or proves unreachability.
   */
  Maybe<Solution> plan();
  /**
   * \brief Try to solve the given problem under the given time bound.
   */
  Maybe<Solution> plan(Seconds timeGiven);

};
