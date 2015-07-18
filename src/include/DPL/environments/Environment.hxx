#pragma once

// Includes
// ========
#include <fstream>
// DPL
#include <DPL/environments/MDP.hxx>




/**
 * \brief An action for Deterministic Environments.
 *
 * REVIEW: Should planners use a vector<Outcomes>?
 *         Changing this may break compability with the SBPL!.
 */
class NodeStub {
public:
  StateID id;
  Cost    cost;

  NodeStub(StateID state, Cost c) : id(state), cost(c) {}
};
typedef std::vector<NodeStub> Neigboorhood;


/**
 * \brief Changes that occurred on the Environment.
 *
 * A set of lists describing the changes ocurred on each State
 *
 * REVIEW: Occlusion may be wanted, as probably not all changes
 *           can be observed by the agent. Probably this will
 *           work fine for a single update, but multiple updates
 *           will require tracking what the agent knew (or
 *           merging the distinct types of changes).
 */
struct StateChanges {
  map<StateID, Neigboorhood> appeared;
  map<StateID, Neigboorhood> increased;
  map<StateID, Neigboorhood> decreased;
  map<StateID, Neigboorhood> removed;
};


/**
 * \brief base class for environments defining planning graphs
 *
 * It is independent of the graph search used
 * The main means of communication between environment and graph search is
 * through stateID.  Each state is uniquely defined by stateID and graph
 * search is ONLY aware of stateIDs. It doesn't know anything about
 * the actual state variables.  Environment, on the * other hand, maintains a
 * mapping from stateID to actual state variables (coordinates) using
 * StateID2IndexMapping array
 *
 * \note Known as DiscreteSpaceInformation on the SBPL.
 *
 * \param StateArgumentCount: State arity for the problem.
 */
template <
  std::size_t StateArgumentCount=1
>
class DiscreteEnvironment {

  typedef std::array<std::size_t, StateArgumentCount> StateArguments;
  /**
   * \brief mapping from hashentry stateID (used in environment to contain
   *        the coordinates of a state, say x,y or x,y,theta)
   *        to an array of state indices used in searches.
   *
   * If a single search is done, then it is a single entry.
   *
   * StateID2IndexMapping[100][0] = 5 means that hashentry with stateID 100
   * is mapped onto search index = 5 in search 0
   * The value of -1 means that no search state has been created yet for this
   * hashentry
   */
  std::vector<StateArguments> StateID2IndexMapping;

  // Initialization
  // ==============
public:

  DiscreteEnvironment() {}

  /**
   * Virtual destructor allows calling the derived class destructor.
   */
  virtual ~DiscreteEnvironment() {
    log_mem << "DiscreteEnvironment at " << (void*)this << " will be destroyed...";
    log_mem << "DiscreteEnvironment at " << (void*)this << " was destroyed...";
  }


  // Config
  // ------
  /**
   * \brief initialization environment from file (see .cfg files for examples)
   */
  virtual bool loadFile(const std::string environmentFilePath) = 0;

  /**
   * \brief initialization of MDP data structure
   */
  virtual bool loadProblem(MDPProblem& problem) = 0;
  /**
   * \brief sets a parameter to a value. The set of supported parameters depends on the particular environment
   */
  virtual bool setParameter(const std::string parameter, int value) {
    _ignore(parameter);
    _ignore(value);
    err_env << ("Environment has no parameters that can be set via SetEnvParameter function\n");
    return false;
  }


  // Heuristic function
  // ==================
  /**
   * \brief heuristic estimate from state FromStateID to state ToStateID
   */
  virtual Heuristic heuristic(const StateID fromID, const StateID toID) = 0;

  /**
   * \brief heuristic estimate from state with stateID to goal state
   */
  virtual Heuristic goalHeuristic(const StateID id) = 0;

  /**
   * \brief heuristic estimate from start state to state with stateID
   */
  virtual Heuristic startHeuristic(const StateID id) = 0;


  // Successor function
  // ==================
  virtual Neigboorhood getSuccessors(const StateID source) = 0;
  virtual Neigboorhood getPredecessors(const StateID target) = 0;


  // Goal check
  // ==========
  virtual bool isGoal(const StateID id) = 0;


  // Statistics
  // ==========
  virtual uint stats_statesCreated(const StateID id) = 0;


  // Print
  // =====
  virtual std::string toString(const StateID state) = 0;


  // Benchmarking
  // ============

  /**
   * \brief Generates a random environment.
   * Used to benchmark algorithms
   */
  virtual void generateRandomEnvironment(Seed seed) = 0;

  /**
   * \brief Generates a random environment.
   * Used to benchmark algorithms
   */
  virtual void modifyEnvironment(Seed seed, Percentage changes) {
    // TODO: this should take more parameters and return something?
    _ignore(seed);
    _ignore(changes);
    err_env << ("ERROR: modifyEnvironment is not implemented for this environment!\n");
    throw new exception();
  };

  /**
   * \brief Generates a random (start, goal) pair cheking for feasibility.
   * Used to benchmark algorithms
   *
   * \return whether the goal state is reachable from the start
   */
  virtual Maybe<MDPProblem> generateRandomProblem(Seed seed, int maxTries) {
    _ignore(seed);
    _ignore(maxTries);
    err_env << ("ERROR: generateRandomProblem is not implemented for this environment!\n");
    throw new exception();
  };

  /**
   * \brief Generates a random (start, goal) pair cheking for feasibility.
   * Used to benchmark algorithms
   *
   * \return whether the goal state is reachable from the start
   */
  virtual Maybe<StateID> generateRandomStart(Seed seed, int maxTries) {
    _ignore(seed);
    _ignore(maxTries);
    err_env << ("ERROR: generateRandomProblem is not implemented for this environment!\n");
    throw new exception();
  };
  /**
   * \brief Generates a random goal cheking for feasibility.
   * Used to benchmark algorithms
   *
   * \return whether the goal state is reachable from the start
   */
  virtual Maybe<StateID> generateRandomGoal(Seed seed, int maxTries) {
    _ignore(seed);
    _ignore(maxTries);
    err_env << ("ERROR: generateRandomProblem is not implemented for this environment!\n");
    throw new exception();
  };


  // Validation
  // ==========
  Maybe<Cost> checkArc(StateID source, StateID target) {
    Maybe<Cost> c;
    auto neighs = getSuccessors(source);

    for(NodeStub n : neighs) {
      StateID s = n.id;
      if(s==target) {
        // This assumes that no 2 arc exists for any pair of nodes
        c = n.cost;
        return c;
      }
    }

    return c;
  }

  Maybe<Cost> check(Path& path) {
    Maybe<Cost> ret;
    Cost cost(0);

    if(!path.size()){
      err_env << ("Path is empty\n");
      return ret;
    }
    if(!isGoal(path.back())){
      err_env << ("Last StateID is not a goal\n");
      return ret;
    }

    for(std::size_t i=0; i<path.size()-1; i++) {
      StateID s = path[i];
      StateID t = path[i+1];

      if(isGoal(s)) {
        warn_env << "Subpath reachs a goal\n";
        return cost;
      }

      Maybe<Cost> c = checkArc(s, t);
      if(!c) {
        err_env << "Path is not continuous. "
                << "(" << s << ": " << toString(s) << ")"
                << " -/-> "
                << "(" << t << ": " << toString(t) << ")"
                << endl;
        return ret;
      }
      else {
        cost += *c;
      }
    }

    return cost;
  }


  // TODO: Expose Sensor updates

  // Logging
  // =======
  /**
   * Output stream support for Search IDs.
   */
  friend std::ostream& operator<< (std::ostream& os, const DiscreteEnvironment& env) {
    return os << "Environment[" << env << "]";
  }
  /**
   * EasyLogging++ stream support
   */
  virtual void log(OutStream& os) const {
    os << "Environment["<< (void*)this << "]";
  }

};
