#pragma once

// Includes
// ========
// DPL
#include <dpl/planners/Node.hxx>
#include <dpl/utils/WeakHeap.hxx>




template<typename keyType, int compareSize>
class _AStarNode : public Node<keyType, compareSize> {

  Cost _g;
  Heuristic _h;


public:
  /**
   * WeakHeap that this templated class may use
   */
  typedef WeakHeap<_AStarNode<keyType, compareSize>, keyType, compareSize> WeakHeap;
  typedef Key<keyType, compareSize> Key;

  _AStarNode(StateID stateID) : Node<keyType, compareSize>(stateID) {
  }
  virtual ~_AStarNode() {
  }

  // Search
  // ======
  virtual Cost g() const {
    return _g;
  }
  virtual void g(const Cost G) {
    _g = G;
  }
  virtual Heuristic h() const {
    return _h;
  }
  virtual void h(const Heuristic H) {
    _h = H;
  }
  virtual Cost f() const{
    return _g + (Cost) _h;
  }
  virtual void f(const Cost G, const Heuristic H) {
    _g = G;
    _h = H;
  }

};

template<typename keyType>
class _AStarNode<keyType, 1> : public Node<keyType, 1> {
};

// Default parameters
typedef _AStarNode<Cost, 2> AStarNode;
typedef AStarNode::WeakHeap AStarHeap;
typedef AStarNode::Key AStarKey;
