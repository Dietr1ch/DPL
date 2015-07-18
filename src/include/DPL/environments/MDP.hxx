#pragma once

// Includes
// ========
// DPL
#include <DPL/environments/MDPState.hxx>




namespace dpl {

/**
 * \brief A Markov Decision Process.
 *
 * \note Known as CMDP on the SBPL.
 */
class MDP {

public:
  // Data
  // ----
  std::vector<MDPState*> stateArray;

  // Constructors
  // ------------
  MDP() {}
  virtual ~MDP() {}

  //functions
  bool empty() const;
  bool full() const;

  //creates numofstates states. Their ids are their orderings for Original, Thresholded & Search MDPs
  bool create(int numofstates);
  bool Delete();
  MDPState* addState(StateID stateID);

  /**
   * Output stream support.
   */
  friend std::ostream& operator <<(std::ostream& os, const MDP& mdp);
  /**
   * EasyLogging++ stream support.
   */
  virtual void log(OutStream& os) const {
    err_env << "Logging not implomented";
    _ignore_reviewed(os);
  }
};


/**
 * \brief MDP Problem definition for some environment, a (start, goal) pair.
 *
 * REVIEW: The goal may not be unique.
 *
 * \note Known as MDPConfig on the SBPL.
 */
struct MDPProblem {
  StateID start;
  StateID goal;
};

}
