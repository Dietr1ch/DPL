
// Includes
// ========
// C
#include <cassert>
#include <cstdlib>  // exit and (standard) failure codes
// C++
#include <algorithm>
// DPL
#include <dpl/planners/AStar/AStarPlanner.hpp>
#include <dpl/utils/log.hpp>


#include <dpl/utils/Queue.hpp>

#define MALLOC(type) (  (type*) malloc(sizeof(type))  )


// Object management
// -----------------
AStarPlanner::AStarPlanner(DiscreteSpaceInformation *environment) {
  log_mem << "Creating planner at " << this << endl;
  assert(environment);

  space = new AStarSpace(environment);
  assert(space);

  log_mem << "Planner created at " << this << endl;
}


AStarPlanner::~AStarPlanner(){
  log_mem << "Planner at " << this << " will be destroyed..." << endl;
  if(space)
    delete(space);
  space = nullptr;
  log_mem << "Planner at " << this << " was destroyed" << endl;
}




// Search
// ------
maybeSolution
AStarPlanner::plan(Seconds givenTime) {

  assert(space);
  assert(space->problem);
  assert(space->startState);
  assert(space->goalState);

  assert(path.empty());
  assert(space->openEmpty());

  // TODO: allow backwardSearch

  // Initialization
  // ==============

  // Get start & goal nodes
  // ----------------------
  AStarNode  *goalNode = space->getGoal();
  AStarNode *startNode = space->getStart();
  // Reset statistics (just in case)
  // ----------------
  stats.iteration  = 0;
  stats.expansions = 0;


  // 'Reach' starting node with cost 0
  // ---------------------------------
  startNode->_g = Cost();
  space->upsertOpen(startNode);


  // Expansion
  // =========

  CKey bestKey;
  while(!space->openEmpty()){
    // Get the best node
    // -----------------
    bestKey = space->open->getminkeyheap();
    AStarNode *bestNode = space->popOpen();

    // Check the search should go on
    // -----------------------------
    if(bestKey.key[0] >= Cost::infinity)         // Best node is not reachable
      break;
    if(goalNode->g(space->iteration) <= bestKey.key[0])    // Best node can't improve the path
      break;  // <= bestNode.f

    // Expand the best node
    // --------------------
    updateSuccessors(*bestNode);
    if(++stats.expansions%(1<<8)==0)
      log_alg << "    expansions so far: " << stats.expansions << endl;
  }

  log_alg << "  expansions: " << stats.expansions << endl;
  SBPL_TRACE("  open list final size: %d\n", space->open->currentsize);
  space->open->makeemptyheap();


  cost = goalNode->g(space->iteration);

  // Open list information
  // =====================
  SBPL_DEBUG("  reached A*Node[%p] %8d, %8d, %8d\n", goalNode,
             goalNode->g(space->iteration),
             goalNode->h(),
             goalNode->f()
  );

  SBPL_DEBUG("    Goal g: %5d", goalNode->g(space->iteration));

  if(space->openEmpty())
    SBPL_DEBUG("    A*: No more nodes to reach");

  if(bestKey.key[0] >= Cost::infinity) {
    SBPL_DEBUG("    Next bestKey is infinite, no more nodes are reachable");
    return SBPL_ERR;
  }



  // Recover the path
  // ================
  assert(path.empty());
  StateID current = goalNode->id();
  StateID startState = space->startState->id;

  auto maxPathLength = stats.expansions;
  for(uint i=0; i<=maxPathLength; i++) {
    path.push_back(current);
    if(current==startState)
      break;

    auto currentNode = space->getNode(current);
    if(!currentNode->bestPredecesor)
      // FIXME: initial state should retain the predecessor
      break;

    current = currentNode->bestPredecesor->id;
  }
  reverse(path.begin(), path.end());


  // Print path
  // ----------
  SBPL_DEBUG("  Path:\n");
  for(auto id : path){
    auto node = space->getNode(id);
    assert(node);
    assert(node->MDPstate);
    assert(id==node->MDPstate->id);

    //       SBPL_DEBUG("    reached (StateID%5zu) A*Node[%p] {%s} %8d, %8d, %8d\n",
    //                   id,
    //                   node,
    //                   space->problem->cString(id),
    //                   node->g(space->iteration),
    //                   node->h(),
    //                   node->f()
    //                 );
  }

  //     SBPL_DEBUG("    Next bestKey is worse than goalKey"
  //                     ", no more nodes are worth expanding");

  return SBPL_OK;
}

inline
void
AStarPlanner::updateSuccessors(AStarNode &node) {
  assert(space);
  assert(space->problem);

  StateID id = node.id();

  // Get successors from environment
#if AStar_SAVE_NEIGHBOURS
  if(!node.successors)
    node.successors = space->problem->GetSuccs(id);
  assert(node.successors);
#else
  auto successors = space->problem->getSuccessors(id);
#endif

  // Reach each node updating path&cost on improvements
#if AStar_SAVE_NEIGHBOURS
  for(NodeStub nS : node.successors)
#else
  for(NodeStub nS : successors)
#endif
    reachSuccessor(node, nS);
}


inline
void
AStarPlanner::reachSuccessor(AStarNode &node, const NodeStub &nS) {
  assert(nS.valid());

  auto *neigh = space->getNode(nS.id);

  // Reached from node and and action that costs nS.cost
  auto newG = node.g(space->iteration) + nS.cost;
  assert(newG>=0);

  if (newG < neigh->g(space->iteration)) {          // A better path was found
    // Use new path
    neigh->_g = newG;  //Save new cost
    neigh->bestPredecesor = node.MDPstate;

    // Update this node on the open list
    space->upsertOpen(neigh);
  }
}

inline
void
AStarPlanner::updatePredecessors(AStarNode &node) {
  //TODO: Reuse updateSuccessors code. Find how to do it w/o performance hits
  assert(space);
  assert(space->problem);

  StateID id = node.id();

  // Get predecessors from environment
#if AStar_SAVE_NEIGHBOURS
  if(!node.predecessors)
    node.predecessors = space->problem->GetPreds(id);
  assert(node.predecessors);
#else
  auto predecessors = space->problem->getPredecessors(id);
#endif

  // Reach each node updating path&cost on improvements
#if AStar_SAVE_NEIGHBOURS
  for(NodeStub nS : *node.predecessors)
#else
  for(NodeStub nS : predecessors)
#endif
    reachPredecessor(node, nS);

  QUIT("%s", "Not yet implemented");
}

inline
void
AStarPlanner::reachPredecessor(AStarNode &node, const NodeStub &nS) {
  //TODO: reach predecessors
  QUIT("%s", "Not yet implemented");
}





// SBPL Planning API
// -----------------
int
AStarPlanner::replan(double givenSeconds, Path& path, int& cost) {

  assert(path.empty());


  assert(space->openEmpty());
  // Start timing
  time.start = clock();

  bool solutionFound;
  solutionFound = Search(path, cost, firstSolutionOnly, true, givenSeconds);

  // End timing
  time.end = clock();
  space->openClear();

  auto elapsed = (double)(time.end-time.start) / (double)CLOCKS_PER_SEC;

  if(solutionFound)
    SBPL_TRACE("A* found a solution (on %.5fs/%.2fs)...",
               elapsed, givenSeconds);
  else
    SBPL_TRACE("A* failed to find a solution (on %.5fs/%.2fs)...",
               elapsed, givenSeconds);

  return solutionFound;
}
int
AStarPlanner::replan(Path& path, ReplanParams params, int& cost) {
  // TODO: replan with parameters (givenTime, repairTime, epsilon)
  return SBPL_ERR;
}


// SBPL Configuration API
// ----------------------
inline
int
AStarPlanner::set_start(StateID startID) {
  space->setStart(startID);
  return SBPL_OK;
}
inline
int
AStarPlanner::set_goal(StateID goalID) {
  space->setGoal(goalID);
  return SBPL_OK;
}

inline
int
AStarPlanner::set_search_mode(bool firstSolutionOnly) {
  this->firstSolutionOnly = firstSolutionOnly;
  return SBPL_OK;
}
inline
int
AStarPlanner::force_planning_from_scratch() {
  // TODO: Drop search efforts
  return SBPL_OK;
}


// SBPL Update API
// ---------------
inline
void
AStarPlanner::costs_changed(StateChangeQuery const & stateChange) {
  // TODO: update costs
}
