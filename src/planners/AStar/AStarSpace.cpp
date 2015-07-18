#include <cassert>
#include <ctime>
#include <cstdlib>  // exit and (standard) failure codes
#include <algorithm>

// SBPL Includes
#include <DPL/planners/AStar/AStarSpace.hxx>




// A* Space
// ========
AStarSpace::~AStarSpace() {
  MEM("Space at %p will be destroyed...", (void*)this);

  // Clear search instance (States and Nodes are cleared with the rest)
  // ---------------------
  startState = nullptr;
  goalState  = nullptr;


  // Delete heap (Clearing heap indexes)
  // -----------
  DELETE(open);


  // Delete states and nodes
  // -----------------------
  for(CMDPSTATE *state : MDP.StateArray){
    // Delete Node
    // REVIEW: heap index clear becomes useless (just here?)
    delete (AStarNode*) state->PlannerSpecificData;
    state->PlannerSpecificData = nullptr;

    // Delete State
    delete state;
  }
  MDP.StateArray.clear();


  // Delete problem data
  // -------------------
  //DELETE(problem);

  MEM("Space at %p was destroyed", (void*)this);
}

inline
void
AStarSpace::updateNode(AStarNode &node) {
  if(node.lastUpdated!=iteration) {
    node._h = computeHeuristic(*node.MDPstate);
    node.lastUpdated = iteration;
  }
}

inline
int
AStarSpace::computeHeuristic(const CMDPSTATE &origin){
  auto target = backwardSearch ? startState : goalState;
  assert(target);

  return problem->GetFromToHeuristic(origin.id, target->id);
}

// Configuration
// -------------
inline
void
AStarSpace::setStart(StateID startID) {
  // Getting the A* node is an overhead (here), but will be generated later.
  auto newStartNode  = getNode0(startID);
  auto newStartState = newStartNode->MDPstate;

  assert(newStartState);
  if(startState != newStartState){
    SBPL_DEBUG(" start = A*Node[%p] {%s}",
               newStartNode,
               problem->cString(startID)
    );
    startState = newStartState;

    // Heuristic remains consistent
  }
}


inline
void
AStarSpace::setGoal(StateID goalID) {
  // Getting the A* node is an overhead (here), but will be generated later.
  auto newGoalNode  = getNode0(goalID);
  auto newGoalState = newGoalNode->MDPstate;
  newGoalNode->_g = Cost::infinity;

  assert(newGoalState);
  if(goalState != newGoalState){
    SBPL_DEBUG("  goal = A*Node[%p] {%s}",
               newGoalNode,
               problem->cString(goalID)
    );
    goalState = newGoalState;

    // TODO: Use lazy update
    for(CMDPSTATE *state : MDP.StateArray){
      assert(state);
      AStarNode *node = (AStarNode*) state->PlannerSpecificData;
      assert(node);
      // REVIEW: compute h needed for the whole space?
      // REVIEW: use invalidation and re-compute on demand
      node->_h = computeHeuristic(*state);
    }
  }
}




// Open list management
// --------------------
inline
void
AStarSpace::upsertOpen(AStarNode* node) {
  CKey k;
  k.key[0] = node->f();
  k.key[1] = node->h();

  assert(node);
  if(node->heapindex)
    updateOpen_(node, k);
  else
    insertOpen_(node, k);
}
inline
void
AStarSpace::insertOpen_(AStarNode *node, CKey key) {
  assert(node);
  assert(node->lastUpdated==iteration);

  open->insertheap(node, key);
}
inline
void
AStarSpace::updateOpen_(AStarNode* node, CKey key) {
  assert(node);
  assert(node->lastUpdated==iteration);

  open->updateheap(node, key);
}
inline
AStarNode*
AStarSpace::popOpen() {
  auto node = (AStarNode*) open->deleteminheap();
  assert(node);
  return node;
}
inline
bool
AStarSpace::openEmpty(){
  return open->emptyheap();
}
inline
void
AStarSpace::openClear() {
  return open->makeemptyheap();
}


// Nodes management
// ----------------
inline
AStarNode*
AStarSpace::getNode(StateID id) {
  assert(validSearchID(id));

  if(id >= problem->StateID2IndexMapping.size())
    QUIT("State %zu is invalid\n", id);
  if(problem->StateID2IndexMapping[id][AStarMDP_STATEID2IND] == -1)
    return new AStarNode(id, this);

  return getNode_(id);
}
inline
AStarNode*
AStarSpace::getNode0(StateID id) {
  assert(validSearchID(id));

  if(id >= (StateID) problem->StateID2IndexMapping.size())
    QUIT("State %zu is invalid\n", id);
  if(problem->StateID2IndexMapping[id][AStarMDP_STATEID2IND] == -1)
    return new AStarNode(id, this, 0);

  return getNode_(id);
}
inline
AStarNode*
AStarSpace::getNode_(StateID id) {
  assert(validSearchID(id));

  auto index = problem->StateID2IndexMapping[id][AStarMDP_STATEID2IND];
  CMDPSTATE *cmdpState = MDP.StateArray[index];
  assert(cmdpState);
  auto node = (AStarNode*) cmdpState->PlannerSpecificData;
  assert(node);

  updateNode(*node);

  return node;
}

inline
AStarNode*
AStarSpace::getNode(CMDPSTATE* mdpState) {
  assert(mdpState);
  assert(mdpState->id);
  auto node = (AStarNode*) mdpState->PlannerSpecificData;
  assert(node);

  #ifdef DEBUG
  auto nodeFromID = getNode(mdpState->id);
  if(node!=nodeFromID)
    QUIT("\nCDMPState[%p] data unconsistent data:\n"
    "  state[%zu]\n"
    "  cmdp planner data: A*Node[%p]\n"
    "  space id target:   A*Node[%p]",
    (void*)mdpState,
         mdpState->id,
         (void*)node,
         (void*)nodeFromID);
    #endif


    updateNode(*node);

    return node;
}

// Shortcuts
inline AStarNode* AStarSpace::getStart()  {
  auto startNode = getNode(startState);
  assert(startNode);

  updateNode(*startNode);
  return startNode;
}
inline AStarNode* AStarSpace::getGoal()   {
  auto goalNode = getNode(goalState);
  assert(goalNode);

  goalNode->_h = 0;
  return goalNode;
}





// Statistics
// ----------
inline
void
AStarSpace::resetStatistics(){
}

