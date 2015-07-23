#pragma once

// Includes
// ========
// DPL
#include <DPL/planners/AStar/AStarSpace.hxx>




namespace DPL {

/**
 * \brief A* Planner
 *
 * \param KeyType:             Type to use for comparisons.
 * \param keySize:             Number of comparisons ('1+tie breaks').
 * \param stateArgumentCount:  Number of arguments that States have (2D grid -> 2).
 * \param OpenType:            Queue to rank the 'Open List'.
 */
template<
  typename  KeyType=Cost,
  int       keySize=1,
  size_t    stateArgumentCount=1,
  typename  OpenType=VectorQueue<
                                 AStarNode<Key<KeyType,keySize>>,
                                 &AStarNode<Key<KeyType, keySize>>::indexOpen,
                                 Key<KeyType,keySize>
                                >
>
class AStarPlanner : public Planner {


  typedef Key<KeyType, keySize> _Key;
  typedef AStarNode<_Key> _Node;
  typedef AStarSpace<_Key, stateArgumentCount, OpenType> _Space;
  typedef DiscreteEnvironment<stateArgumentCount> _Env;

  /**
   * \brief The Open 'list' ranks the Nodes on the fringe.
   *
   * Actually it's a priority queue, allowing to quickly expand the best (pop) and update nodes.
   */
  typedef OpenType _Open;

  static_assert(is_base_of<
                     IndexedQueue<_Node, &_Node::indexOpen, _Key>,
                     OpenType
                >::value,
                "OpenType must derive from IndexedQueue");

  typedef void (*reachingFunction) (const _Node& node, const NodeStub& nS);

protected:
  _Space space;

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
  AStarPlanner(_Env* environment);
  ~AStarPlanner();


protected:

  /**
   * \brief Expand a node forward.
   *
   * An expansion (forward) 'reaches' all the neighbors
   */
  inline void updateSuccessors(_Node& node);

  /**
   * \brief Reach Node forward recording the improvement.
   *
   * A (forward) 'reach' checks if a Node can reach the Node specified by
   *   the neighborStub in a better way than before, recording the improvement
   *   (if any)
   */
  inline void  reachSuccessor(_Node& node, const NodeStub& nS);


  // DPL API
  // ========

public:
  optional<Solution> plan() {
    optional<Solution> ret;

    return ret;
  }

  int set_start(StateID startID);
  int set_goal(StateID goalID);

  int force_planning_from_scratch();
  int set_search_mode(bool bSearchUntilFirstSolution);
  void costs_changed(StateChanges const & stateChanges);
};

}
