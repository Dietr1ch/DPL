#pragma once

using namespace std;


// Includes
// --------
// C
#include <ctime>
// DPL
#include <dpl/planners/Planner.hpp>
#include <dpl/environments/MPD.hpp>
// #include <dpl/utils/mdp.h>
// #include <dpl/utils/mdpconfig.h>
// #include <dpl/discrete_space_information/environment.h>
// #include <dpl/utils/key.h>
// #include <dpl/utils/heap.h>



// Configuration
// -------------
// Nodes consult their neighborhood once (+StateChangeQuery should give a boost)
#define AStar_SAVE_NEIGHBOURS 0




/**
 * Adaptive A* Search Space
 */
class AStarSpace {
public:
  // Problem data
  CMDP MDP;
  DiscreteSpaceInformation *problem;

  // Instance data
  CMDPSTATE *startState;
  CMDPSTATE *currentState;
  CMDPSTATE *goalState;

  // Iterative Planner information
  // -----------------------------

  // Current search Iteration
  SearchID iteration;  // Search iteration identifier

  // Data
  CHeap *open;

  // Configuration
  bool backwardSearch;  // It should be on the planner, but eases heuristic calculation


  /** Statistics */
  struct{
    /** Per-search-episode stuff */
    struct{
      /** Expansions made on this space */
      vector<uint> expansions;
      vector<uint> pathLength;
    } perSearch;
  } stats;



  // Object management
  // =================

  AStarSpace(DiscreteSpaceInformation *problem);
  ~AStarSpace();

  /** Ensures Node values are updated up to this search iteration */
  inline void updateNode(AStarNode &node);
  /** Computes the heuristic value from a State to the goal State */
  inline int computeHeuristic(const CMDPSTATE &origin);


  // Configuration
  // =============
  void setStart(StateID startID);
  void setGoal(StateID goalID);



  // Open list management
  // ====================

  /** Inserts or Updates (Upserts) a node in the open list */
  void upsertOpen(AStarNode *node);


  /** UNSAFE: inserts a node in open, No questions asked */
  void insertOpen_(AStarNode *node, CKey key);
  /** UNSAFE: updates a node in open, No questions asked */
  void updateOpen_(AStarNode *node, CKey key);

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
  AStarNode* getNode(CMDPSTATE *mdpState);

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
