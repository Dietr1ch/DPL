#pragma once

using namespace std;


// Includes
// --------
// C
#include <ctime>
// DPL
#include <dpl/planners/Planner.hxx>
#include <dpl/planners/AStar/AStarNode.hxx>
#include <dpl/planners/Space.hxx>
#include <dpl/environments/Environment.hxx>
#include <dpl/environments/MDP.hxx>


// Configuration
// -------------
// Nodes consult their neighborhood once (+StateChangeQuery should give a boost)
#define AStar_SAVE_NEIGHBOURS 0




/**
 * \brief A* Search Space
 */
template<typename keyType=Cost, KeySize keySize=1, std::size_t stateArgumentCount=1>
class AStarSpace : public Space {

  typedef AStarNode<keyType, keySize> AStarNode;
  typedef Key<keyType, keySize> AStarKey;
  typedef DiscreteEnvironment<stateArgumentCount> DiscreteEnvironment;
  typedef typename AStarNode::Open Open;

public:
  // Problem data
  std::unique_ptr<MDP> mdp;
  DiscreteEnvironment& environment;

  // Instance data
  MDPState *startState;
  MDPState *currentState;
  MDPState *goalState;

  // Iterative Planner information
  // -----------------------------

  // Current search Iteration
  SearchID iteration;  // Search iteration identifier

  // Data
  Open open;

  // Configuration
  bool backwardSearch;  // It should be on the planner, but eases heuristic calculation



  // Object management
  // =================

  AStarSpace(DiscreteEnvironment& environment) : environment(environment) {}
  ~AStarSpace() {
    log_mem << "AStarSpace at " << (void*)this << " will be destroyed...";
    log_mem << "AStarSpace at " << (void*)this << " was destroyed...";
  }

  /** Ensures Node values are updated up to this search iteration */
  inline void updateNode(AStarNode &node);
  /** Computes the heuristic value from a State to the goal State */
  inline int computeHeuristic(const MDPState &origin);


  // Configuration
  // =============
  void setStart(StateID startID);
  void setGoal(StateID goalID);



  // Open list management
  // ====================

  /** Inserts or Updates (Upserts) a node in the open list */
  void upsertOpen(AStarNode *node);


  /** UNSAFE: inserts a node in open, No questions asked */
  void insertOpen_(AStarNode *node, AStarKey key);
  /** UNSAFE: updates a node in open, No questions asked */
  void updateOpen_(AStarNode *node, AStarKey key);

  /** gets the best node on the open 'list' */
  AStarNode* popOpen();

  /** Checks wheter the open list is empty */
  bool openEmpty();

  /** Clears the open list */
  void openClear();



  // Nodes acquisition
  // =================

  /** Get an updated node */
  AStarNode* getNode(StateID id);
  /** Get a node without updating h (having h=0 if it's new) */
  AStarNode* getNode0(StateID id);

  /** Get an updated node */
  AStarNode* getNode(MDPState *mdpState);

  /** UNSAFE: Get an updated node */
  AStarNode* getNode_(StateID id);

  /** Gets the updated starting node */
  AStarNode* getStart();
  /** Gets the updated goal node */
  AStarNode* getGoal();



  // Statistics
  // ==========
  void resetStatistics();
};
