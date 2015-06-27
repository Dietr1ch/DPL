#pragma once

// Includes
#include <dpl/environments/StateID.hxx>
#include <dpl/utils/Key.hxx>



/**
 * \brief base class for a search node.
 *
 * Nodes are containers for problem-specific States that also hold
 *   planner-specific search information.
 *
 * SBPL call this AbstractSearchState and apparently it offers no useful
 *   encapsulation there is no usage of AbstractSearchState
 *   outside their own specializations and also it forces all kind of
 *   nodes to hold 2 'listelem' (no template like this is used).
 */
template<typename keyType, int compareSize>
class Node {

public:

  /**
   * Index of a heap.
   * This may be moved to only some derived classes.
   */
  int heapIndex = 0;

  /**
   * Tie breaking
   */
  Key<keyType, compareSize> key;

  /**
   * StateID
   */
  const StateID id;

  Node(StateID stateID) : id(stateID) { }
  virtual ~Node() { }


  // Comparison
  // ==========
  bool operator ==(const Node& n) const {
    return this==&n;
  }
  bool operator  >(const Node& n) const {
    return key > n.key;
  }
  bool operator  <(const Node& n) const {
    return key < n.key;
  }
  bool operator !=(const Node& n) const {
    return !(operator ==(n));
  };
  bool operator >=(const Node& n) const {
    return !(operator <(n));
  }
  bool operator <=(const Node& n) const {
    return !(operator >(n));
  }


  // Indexing
  // ========
  keyType& operator[](std::size_t i) {
    // FIXME: this hides the modified state, heap will not reorder it.
    return key[i];
  };
  const keyType& operator[](std::size_t i) const {
    // FIXME: this hides the modified state, heap will not reorder it.
    return key[i];
  };


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
