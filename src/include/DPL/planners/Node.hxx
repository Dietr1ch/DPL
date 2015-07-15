#pragma once

// Includes
// ========
// DPL
#include <DPL/environments/StateID.hxx>
#include <DPL/utils/Key.hxx>



/**
 * \brief base class for a search node.
 *
 * Nodes are containers for problem-specific States that also hold
 *   planner-specific search information.
 *
 * \param keyType: type to use for the key values.
 * \param keySize: type to use for the key values.
 *
 * \note Known as AbstractSearchState on the SBPL.
 */
template<typename KeyType=Cost, KeySize keySize=1>
class Node {

public:
  typedef Key<KeyType, keySize> Key;

  /**
   * StateID
   */
  const StateID id;

  // Statistics
  // ----------
#if STATISTICS
  struct {
    uint expansions = 0;
    uint nodesGenerated = 0;
  } stats;
#endif


  // Constructor
  // ===========
  Node(const StateID stateID) : id(stateID) {}

  virtual ~Node() {}


  // Print
  // =====
  /**
   * Output stream support for Costs.
   */
  friend std::ostream& operator <<(std::ostream& os, const Node& n) {
    return os << "Node[" << n.id << "]";
  }
  /**
   * EasyLogging++ stream support
   */
  virtual void log(OutStream& os) const {
    os << "Node[" << id << "]";
  }
};
