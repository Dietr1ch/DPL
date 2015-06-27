#pragma once

// Includes
// ========
#include <iostream>
#include <fstream>
// DPL
#include <dpl/environments/StateID.hxx>
#include <dpl/environments/Cost.hxx>
#include <dpl/utils/log.hpp>




class NodeStub {
public:
  StateID id;
  Cost    cost;
  NodeStub(StateID state, Cost c) {
    id = state;
    cost = c;
  }
};
typedef vector<NodeStub> Neigboorhood;


struct StateChanges {
  map<StateID, Neigboorhood> increased;
  map<StateID, Neigboorhood> decreased;
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
 */
class Environment {

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
  vector<int*> StateID2IndexMapping;

public:

  // Initialization
  // ==============
  /**
   * \brief constructor
   */
  DiscreteEnvironment() = 0;
  /**
   * \brief initialization environment from file (see .cfg files for examples)
   */
  DiscreteEnvironment(const string envFilePath) = 0;
  /**
   * \brief initialization of MDP data structure
   */
  DiscreteEnvironment(MDPConfig& config) = 0;

  /**
   * Virtual destructor allows calling the derived class destructor.
   */
  virtual ~DiscreteSpaceInformation() {
    log_mem << "DiscreteSpaceInformation at " << (void*)this << " will be destroyed...";

    for(auto mapping : StateID2IndexMapping)
      if(mapping)
        delete[] mapping;

    log_mem << "DiscreteSpaceInformation at " << (void*)this << " was destroyed...";
  }

  // Config
  // ------
  /**
   * \brief sets a parameter to a value. The set of supported parameters depends on the particular environment
   */
  virtual bool setParameter(const string parameter, int value) {
    err_env << ("Environment has no parameters that can be set via SetEnvParameter function\n");
    return false;
  }


  // Heuristic function
  // ==================
  /**
   * \brief heuristic estimate from state FromStateID to state ToStateID
   */
  virtual int heuristic(const StateID fromID, const StateID toID) = 0;

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
  virtual bool stats_statesCreated(const StateID id) = 0;


  // Print
  // =====
  string toString(const StateID state) = 0;


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
    err_env << ("ERROR: modifyEnvironment is not implemented for this environment!\n");
    throw new SBPL_Exception();
  };

  /**
   * \brief Generates a random (start, goal) pair cheking for feasibility.
   * Used to benchmark algorithms
   *
   * \return whether the goal state is reachable from the start
   */
  virtual bool generateRandomProblem(MDPConfig *cfg, Seed seed, int maxTries) {
    err_env << ("ERROR: generateRandomProblem is not implemented for this environment!\n");
    throw new SBPL_Exception();
  };

  /**
   * \brief Generates a random (start, goal) pair cheking for feasibility.
   * Used to benchmark algorithms
   *
   * \return whether the goal state is reachable from the start
   */
  virtual bool generateRandomStart(MDPConfig *cfg, Seed seed, int maxTries) {
    err_env << ("ERROR: generateRandomProblem is not implemented for this environment!\n");
    throw new SBPL_Exception();
  };
  /**
   * \brief Generates a random goal cheking for feasibility.
   * Used to benchmark algorithms
   *
   * \return whether the goal state is reachable from the start
   */
  virtual bool generateRandomGoal(MDPConfig *cfg, Seed seed, int maxTries) {
    err_env << ("ERROR: generateRandomProblem is not implemented for this environment!\n");
    throw new SBPL_Exception();
  };


  // Validation
  // ==========
  Maybe<Cost> checkArc(StateID source, StateID target) {
    auto neighs = getSuccessors(source);

    for(NodeStub n : neighs) {
      StateID s = n.id;
      int c  = n.cost;

      if(s==target) {
        // This assumes that no 2 arc exists for any pair of nodes
        return c;
      }
    }

    return nullptr;
  }

  Maybe<Cost> check(Path& path) {
    Cost cost(0);

    if(!path.size()){
      err_env << ("Path is empty\n");
      return false;
    }
    if(!isGoal(path.back())){
      err_env << ("Last StateID is not a goal\n");
      return nullptr;
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
                << "("
                << s << ": " << toString(s)
                << ")"
                << " -/-> "
                << "("
                << t << ": " << toString(t)
                << ")" <<endl;
        return nullptr;
      }
    }

    return cost;
  }


  // TODO: Expose Sensor updates
};
