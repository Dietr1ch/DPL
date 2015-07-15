#pragma once

// Includes
// ========
// DPL
#include <dpl/planners/Node.hxx>
#include <dpl/utils/types.hxx>
#include <dpl/utils/log.hxx>




struct PossibleOutcome {
  StateID stateID;
  Cost cost;
  Probability chance;

  //PossibleOutcome(StateID state, Cost cost, Probability chance) : stateID(state),cost(cost),chance(chance) {}
  //~PossibleOutcome() {}

  static bool likelihood(const PossibleOutcome& l, const PossibleOutcome& r) {
    return l.chance < r.chance;
  }
};

class Outcomes {
  std::vector<PossibleOutcome> outcomes;
#ifdef DEBUG
  /**
   * Margin for the normalization
   */
  static Probability epsilon = 0.00001;
#endif

public:
  Outcomes(std::vector<PossibleOutcome> outcomes) : outcomes(outcomes) {
#ifdef DEBUG
    Probability s = 0;
    for(PossibleOutcome o : outcomes)
      s+=o.chance;

    if(abs(s-1) > epsilon) {
      err_env << "Probabilies sum is not close enough to 1" << endl;
    }
#endif
  };
  ~Outcomes() {}

  Maybe<PossibleOutcome> mostLikelyIndex() {
    Maybe<PossibleOutcome> ret;
    if(outcomes.empty())
      return ret;
    return *std::max_element(outcomes.begin(), outcomes.end(), PossibleOutcome::likelihood);
  }
};

typedef std::size_t PossibleOutcomeIndex;
typedef int ActionID;

class MDPAction {
public:
  // Data
  // ====
  // Source Node
  const StateID  source;
  // std::unique_ptr<Node> node = nullptr;  // Review: is this necessary?
  // Action
  const ActionID actionID;
  Outcomes outcomes;

  // Constructor
  MDPAction(StateID state, ActionID action, Outcomes o) : source(state),actionID(action),outcomes(o) {}

  ~MDPAction() {}


  //functions
  bool isValid() const;
  void addOutcome(PossibleOutcome o);
  PossibleOutcomeIndex getIndexOfMostLikelyOutcome() const;
  PossibleOutcomeIndex getIndexOfOutcome(const StateID outcome) const;
  bool deleteAllOutcomes();

  //operators
  bool operator ==(const MDPAction& a) const {
    return actionID==a.actionID;
  }
};
