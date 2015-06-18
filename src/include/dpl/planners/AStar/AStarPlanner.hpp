#pragma once

// Includes
// C
#include <ctime>
// DPL
#include <dpl/planners/Planner.hpp>


/**
 * A* Planner
 */
class AStarPlanner : public Planner {

protected:
  AStarSpace *space;

  // Configuration
  bool firstSolutionOnly;

  /** Statistics */
  struct {
    uint expansions;
    uint iteration;
  } stats;

  /** Timing Data */
  struct{
    clock_t start;
    clock_t end;
    clock_t total;
  } time;


public:
  AStarPlanner(DiscreteSpaceInformation *environment);
  ~AStarPlanner();


protected:

  /** Expand Node forward
   * An expansion (forward) 'reaches' all the neighbors
   */
  inline void updateSuccessors(ASTARNode &node);

  /** Reach Node forward
   * A (forward) 'reach' reviews if a Node can reach the Node specified by
   *   the neighborStub in a better way than before, recording the improvement
   *   (if any)
   */
  inline void  reachSuccessor (ASTARNode &node, const NodeStub &nS);


  // DPL API
  // ========

public:
  maybeSolution plan();
  maybeSolution plan(Seconds givenTime);

  int set_start(StateID startID);
  int set_goal(StateID goalID);

  int force_planning_from_scratch();
  int set_search_mode(bool bSearchUntilFirstSolution);
  void costs_changed(StateChangeQuery const & stateChange);
};

typedef void (*reachingFunction) (const ASTARNode &node, const NodeStub &nS);
