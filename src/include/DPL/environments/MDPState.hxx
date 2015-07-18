#pragma once

// Includes
// ========
// DPL
#include <DPL/environments/MDPAction.hxx>




namespace DPL {

/**
 * \brief A MDP State.
 *
 * \note Known as CMDPSTATE on the SBPL.
 */
class MDPState {

  const StateID id;
  vector<MDPAction*> actions;
  vector<StateID> predecessors;

  void* plannerData;  // Should point to a Node

  MDPState(StateID id) : id(id) {
  }
  ~MDPState() {
    if(plannerData) {
      err_env << "planner data was not deleted" << endl;
      throw new exception();
    }
  }

  //functions
  bool Delete();
  MDPAction* addAction(StateID id);
  bool containsPred(StateID state);
  bool addPred(StateID state);
  bool removePred(StateID state);
  bool removeAllActions();
  MDPAction* getAction(ActionID action);

  //operators
  void operator =(const MDPState& rhsState);
};

}
