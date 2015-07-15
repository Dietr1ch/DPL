#pragma once

// Includes
// ========
// DPL
#include <dpl/planners/Node.hxx>
#include <dpl/utils/WeakHeap.hxx>




template<typename keyType=Cost, KeySize keySize=1>
class AStarNode : public Node<keyType, keySize> {

protected:
  Cost      _g;
  Heuristic _h;

  IndexType indexOpen = 0;


public:
  /**
   * WeakHeap that this templated class may use
   */
  typedef WeakHeap<AStarNode<keyType, keySize>, &AStarNode::indexOpen, keyType, keySize> Open;

  AStarNode(StateID stateID, Cost g=Cost::infinity, Heuristic h=0) : Node<keyType, keySize>(stateID) {
    _g = g;
    _h = h;
  }
  virtual ~AStarNode() {
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
