#pragma once

using namespace std;


// Includes
// --------
// C
#include <ctime>
// DPL
#include <DPL/planners/AStar/AStarNode.hxx>
#include <DPL/planners/Space.hxx>
#include <DPL/environments/Environment.hxx>
#include <DPL/utils/queues/VectorQueue.hxx>


// Configuration
// -------------
// Nodes consult their neighborhood once (+StateChangeQuery should give a boost)
#define AStar_SAVE_NEIGHBOURS 0




/**
 * \brief A* Search Space
 *
 * \param KeyType:             Type to use for comparisons.
 * \param keySize:             Number of comparisons ('1+tie breaks').
 * \param stateArgumentCount:  Number of arguments that States have (2D grid -> 2).
 * \param OpenType:            Queue to rank the 'Open List'.
 */
template<
  typename    KeyType=Cost,
  KeySize     keySize=1,
  std::size_t stateArgumentCount=1,
  typename    OpenType=VectorQueue<AStarNode<KeyType,keySize>, &AStarNode<KeyType,keySize>::indexOpen, KeyType, keySize>
  >
class AStarSpace : public Space<AStarNode<KeyType, keySize>> {


public:

  typedef AStarNode<KeyType, keySize> _Node;
  typedef DiscreteEnvironment<stateArgumentCount> _Env;
  typedef Key<KeyType, keySize> _Key;

  /**
   * \brief The Open 'list' ranks the Nodes on the fringe.
   *
   * Actually it's a priority queue, allowing to quickly expand the best (pop) and update nodes.
   */
  typedef OpenType _Open;

  static_assert(std::is_base_of<
                     IndexedQueue<_Node, &_Node::indexOpen, KeyType, keySize>,
                     OpenType
                >::value,
                "OpenType must derive from IndexedQueue");



public:
  // Problem data
  std::unique_ptr<MDP> mdp;
  _Env& environment;

  // Instance data
  MDPState *startState   = nullptr;
  MDPState *currentState = nullptr;
  MDPState *goalState    = nullptr;

  // Iterative Planner information
  // -----------------------------

  // Current search Iteration
  SearchID iteration;  // Search iteration identifier

  // Data
  OpenType open;

  // Configuration
  bool backwardSearch;  // It should be on the planner, but eases heuristic calculation



  // Object management
  // =================

  AStarSpace(_Env& environment) : environment(environment) {
    dbg_mem << "Creating space with " << environment;

    backwardSearch = false;
  }
  ~AStarSpace() {
    log_mem << "AStarSpace at " << (void*)this << " will be destroyed...";
    log_mem << "AStarSpace at " << (void*)this << " was destroyed...";
  }

  /** Ensures Node values are updated up to this search iteration */
  inline void updateNode(_Node &node);
  /** Computes the heuristic value from a State to the goal State */
  inline int computeHeuristic(const MDPState &origin);


  // Configuration
  // =============
  void setStart(StateID startID);
  void setGoal(StateID goalID);



  // Open list management
  // ====================

  /** Inserts or Updates (Upserts) a node in the open list */
  void upsertOpen(_Node *node);


  /** UNSAFE: inserts a node in open, No questions asked */
  void insertOpen_(_Node *node, _Key key);
  /** UNSAFE: updates a node in open, No questions asked */
  void updateOpen_(_Node *node, _Key key);

  /** gets the best node on the open 'list' */
  _Node* popOpen();

  /** Checks wheter the open list is empty */
  bool openEmpty();

  /** Clears the open list */
  void openClear();



  // Nodes acquisition
  // =================

  /** Get an updated node */
  _Node& getNode(StateID id) {
  }
  /** Get a node without updating h (having h=0 if it's new) */
  _Node& getNode0(StateID id) {
  }

  /** Get an updated node */
  _Node* getNode(MDPState *mdpState) {
  }

  /** UNSAFE: Get an updated node */
  _Node* getNode_(StateID id) {
  }

  /** Gets the updated starting node */
  _Node& getStart() {
  }
  /** Gets the updated goal node */
  _Node* getGoal() {
  }



  // Statistics
  // ==========
  void resetStatistics();
};
