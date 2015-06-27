#pragma once

// Includes
// ========
// DPL
#include <dpl/environments/MDPState.hxx>




class MDP {

public:
  // Data
  // ----
  vector<MDPState*> StateArray;

  // Constructors
  // ------------
  MDP() { }
  ~MDP() { }

  //functions
  bool empty();
  bool full();

  //creates numofstates states. Their ids are their orderings for Original, Thresholded & Search MDPs
  bool Create(int numofstates);
  bool Delete();
  void Print(FILE* fOut);
  MDPState* AddState(StateID stateID);
};
