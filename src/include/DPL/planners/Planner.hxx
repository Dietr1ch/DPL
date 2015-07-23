# pragma once

// Includes
// ========
// C++
#include <future>
#include <chrono>
// DPL
#include <DPL/utils/types.hxx>
#include <DPL/planners/Node.hxx>




namespace DPL {

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
  virtual optional<Solution> plan() = 0;

  /**
   * \brief Try to solve the given problem under the given time bound.
   *
   * REVIEW: Use future<> and wait_for and simply call plan().
   *           Currently the problem is that it requires calling a
   *           static (non-member) function.
   */
  virtual optional<Solution> plan(const seconds timeGiven) = 0;

};

}
