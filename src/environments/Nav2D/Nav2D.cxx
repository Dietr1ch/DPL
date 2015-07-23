
// Includes
// ========
#include <DPL/environments/Nav2D/Nav2D.hxx>




namespace DPL {

// Constructor
// ===========
Nav2D::Nav2D() {
}
Nav2D::~Nav2D() {
}

// StateManagement

StateID
Nav2D::getID(Arguments a) {
  // TODO: If args had an ID, return the (old) ID.


  StateEntry newEntry{stateEntries.size(), a};
  assert(newEntry.id.valid());
  stateEntries.push_back(newEntry);

  return newEntry.id;
}



// Config
// ======
bool
Nav2D::loadFile(const string environmentFilePath) {
  _ignore(environmentFilePath);

  return false;
}

bool
Nav2D::loadProblem(MDPProblem& problem) {
  _ignore(problem);

  return false;
}

bool
Nav2D::setParameter(const string param, int value) {
  _ignore(param);
  _ignore(value);

  return false;
}



// Heuristic Function
// ==================
Heuristic
Nav2D::heuristic(const StateID source, const StateID target) {
  _ignore(source);
  _ignore(target);

  return 0;
}

Heuristic
Nav2D::goalHeuristic(const StateID id) {
  _ignore(id);

  return 0;
}

Heuristic
Nav2D::startHeuristic(const StateID id) {
  _ignore(id);

  return 0;
}




// Successor function
// ==================
Neigboorhood
Nav2D::getSuccessors(const StateID source) {
  _ignore(source);
  Neigboorhood n;

  // SBPL used absorbing goal states, but it should not be needed.
//   if(isGoal(source))
//     return n;

//   StateEntry e = stateEntries[(size_t)source];
//   for(auto d : directions) {
//     StateEntry newEntry;
//     for(auto i=0; i<StateArgumentCount; i++)
//       newEntry.arguments[i] = e.arguments[i]+delta[i];
//
//   }

  return n;
}

Neigboorhood
Nav2D::getPredecessors(const StateID target) {
  _ignore(target);
  Neigboorhood n;

  return n;
}




// Goal check
// ==========
bool
Nav2D::isGoal(const StateID id) {
  _ignore(id);

  return false;
}




// Statistics
// ==========
uint
Nav2D::stats_statesCreated(const StateID id) {
  _ignore(id);

  return 0;
}



// Benchmarking
// ============
void
Nav2D::generateRandomEnvironment(Seed seed) {
  _ignore(seed);
}

void
Nav2D::modifyEnvironment(Seed seed, Percentage changes) {
  _ignore(seed);
  _ignore(changes);
}

optional<MDPProblem>
Nav2D::generateRandomProblem(Seed seed, int maxTries) {
  _ignore(seed);
  _ignore(maxTries);
  optional<MDPProblem> ret;

  return ret;
}

optional<StateID>
Nav2D::generateRandomStart(Seed seed, int maxTries) {
  _ignore(seed);
  _ignore(maxTries);
  optional<StateID> ret;

  return ret;
}

optional<StateID>
Nav2D::generateRandomGoal(Seed seed, int maxTries) {
  _ignore(seed);
  _ignore(maxTries);
  optional<StateID> ret;

  return ret;
}


ostream&
operator <<(ostream& os, const DiscreteEnvironment<2>& env) {
  _ignore(env);

  return os;
}
void Nav2D::log(OutStream& os) const {
  _ignore(os);
}

}
