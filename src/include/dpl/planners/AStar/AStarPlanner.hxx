#pragma once

// Includes
// C
#include <ctime>
// DPL
#include <dpl/planners/Planner.hxx>
#include <dpl/planners/AStar/AStarSpace.hxx>


/**
 * A* Planner
 */
template<typename keyType=Cost, KeySize keySize=1>
class AStarPlanner : public Planner {

  typedef AStarSpace<keyType, keySize> AStarSpace;
  typedef AStarNode<keyType, keySize> AStarNode;
  typedef void (*reachingFunction) (const AStarNode &node, const NodeStub &nS);

protected:
  std::unique_ptr<AStarSpace> space;

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
  AStarPlanner(DiscreteEnvironment<> *environment);
  ~AStarPlanner();


protected:

  /** Expand Node forward
   * An expansion (forward) 'reaches' all the neighbors
   */
  inline void updateSuccessors(AStarNode &node);

  /** Reach Node forward
   * A (forward) 'reach' reviews if a Node can reach the Node specified by
   *   the neighborStub in a better way than before, recording the improvement
   *   (if any)
   */
  inline void  reachSuccessor (AStarNode &node, const NodeStub &nS);


  // DPL API
  // ========

public:
  Maybe<Solution> plan();
  Maybe<Solution> plan(Seconds givenTime);

  int set_start(StateID startID);
  int set_goal(StateID goalID);

  int force_planning_from_scratch();
  int set_search_mode(bool bSearchUntilFirstSolution);
  void costs_changed(StateChanges const & stateChanges);
};

