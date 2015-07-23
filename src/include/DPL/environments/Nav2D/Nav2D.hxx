#pragma once

// Includes
// ========
// DPL
#include <DPL/environments/Environment.hxx>




namespace DPL {

class Nav2D : public DiscreteEnvironment<2> {

public:
  Nav2D();
  ~Nav2D();

  StateID getID(Arguments args);

  // Config
  bool loadFile(const string environmentFilePath);
  bool loadProblem(MDPProblem& problem);
  bool setParameter(const string param, int value);

  // Heuristic Function
  virtual Heuristic heuristic(const StateID fromID, const StateID toID);
  virtual Heuristic goalHeuristic(const StateID id);
  virtual Heuristic startHeuristic(const StateID id);

  // Successor function
  virtual Neigboorhood getSuccessors(const StateID source);
  virtual Neigboorhood getPredecessors(const StateID target);

  // Goal Check
  virtual bool isGoal(const StateID id);

  // Statistics
  virtual uint stats_statesCreated(const StateID id);

  // Benchmarking
  virtual void generateRandomEnvironment(Seed seed);
  virtual void modifyEnvironment(Seed seed, Percentage changes);
  virtual optional<MDPProblem> generateRandomProblem(Seed seed, int maxTries);
  virtual optional<StateID> generateRandomStart(Seed seed, int maxTries);
  virtual optional<StateID> generateRandomGoal(Seed seed, int maxTries);

  // Validation
  optional<Cost> checkArc(StateID source, StateID target);
  optional<Cost> check(Path& path);

  // Logging
  friend ostream& operator <<(ostream& os, const DiscreteEnvironment<2>& env);
  virtual void log(OutStream& os) const;
};

}
