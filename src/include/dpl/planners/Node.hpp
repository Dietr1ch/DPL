#pragma once




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
template<int compareSize>
class Node {

  /**
   * \brief Each node can be a member of compareSize lists
   */
  int* _listelem[compareSize];

  Node() {
    for(int i=0; i<compareSize; i++)
      _listelem[i] = nullptr;
  }
  ~Node() { }

  virtual bool operator ==(Node& n) {
    return this==&n;
  }
  virtual bool operator !=(Node& n) {
    return !(operator ==(n));
  };
  virtual bool operator  >(Node& n);
  virtual bool operator  <(Node& n);
  virtual bool operator >=(Node& n) {
    return !(operator <(n));
  }
  virtual bool operator <=(Node& n) {
    return !(operator >(n));
  }
};
