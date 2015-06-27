#pragma once

// Includes
// ========
// DPL
#include <dpl/planners/Node.hpp>
#include <dpl/utils/log.hpp>
#include <dpl/utils/types.hpp>




struct PossibleOutcome {
  StateID stateID;
  Cost cost;
  Probability chance;

  static bool likelihood(const PossibleOutcome& l, const PossibleOutcome& r){
    return l.chance < r.chance;
  }
};

class Outcomes {
  vector<PossibleOutcome> outcomes;
#ifdef DEBUG
  /**
   * Margin for the normalization
   */
  static Probability epsilon = 0.00001;
#endif

  Outcomes(vector<PossibleOutcome> outcomes) {
    this->outcomes = outcomes;
#ifdef DEBUG
    Probability s = 0;
    for(PossibleOutcome o : outcomes)
      s+=o.chance;

    if(abs(s-1) > epsilon) {
      err_env << "Probabilies sum is not close enough to 1" << endl;
    }
#endif
  };

  Maybe<PossibleOutcome> mostLikelyIndex() {
    // return std::max_element(outcomes.begin(), outcomes.end(), PossibleOutcome::likelihood);
    Maybe<PossibleOutcome> mostLikely = nullptr;
    Probability max(0);
    Probability s = 0;
    for(PossibleOutcome o : outcomes)
      if(o.chance > max) {
        mostLikely = o;
        max = o.chance;
      }
    return mostLikely;
  }
};

typedef vector<PossibleOutcome> Outcomes;
typedef std::size_t PossibleOutcomeIndex;

typedef int ActionID;

class MDPAction {
public:
  // Data
  // ====
  // Source Node
  StateID  sourceID;
  std::unique_ptr<Node> node;
  // Action
  ActionID actionID;
  Outcomes outcomes;

  // Constructor
  MDPAction(ActionID a, StateID s) {
    actionID = a;
    sourceID = s;
    node = nullptr;
  }
  ~MDPAction() {
#ifdef DEBUG
    if (node != nullptr) {
      err_env << "ERROR: state deletion: planner specific data is not deleted\n";
      throw new SBPL_Exception();
    }
#endif
  }


  //functions
  bool Delete();
  bool IsValid();
  void addOutcome(PossibleOutcome o);
  int GetIndofMostLikelyOutcome();
  int GetIndofOutcome(StateID OutcomeID);
  bool DeleteAllOutcomes();

  //operators
  void operator =(const CMDPACTION&);
};
