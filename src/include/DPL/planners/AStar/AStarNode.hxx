#pragma once

// Includes
// ========
// DPL
#include <DPL/planners/Node.hxx>
#include <DPL/utils/queues/IndexedQueue.hxx>




/**
 * \brief A* Search Node.
 */
template<
  typename KeyType = Cost,
  KeySize  keySize = 1
>
class AStarNode : public Node<KeyType, keySize> {

protected:
  Cost      _g;
  Heuristic _h;

public:
  IndexType indexOpen = 0;


public:

  AStarNode(StateID stateID, Cost g=Cost::infinity, Heuristic h=0) : Node<KeyType, keySize>(stateID) {
    _g = g;
    _h = h;
  }
  virtual ~AStarNode() {
  }


  // Search
  // ======
  // g,h,f getters
  virtual Cost g() const {
    return _g;
  }
  virtual Heuristic h() const {
    return _h;
  }
  virtual Cost f() const{
    return _g + (Cost) _h;
  }

  // g,h,f setters
  virtual void g(const Cost G) {
    _g = G;
  }
  virtual void h(const Heuristic H) {
    _h = H;
  }
  virtual void f(const Cost G, const Heuristic H) {
    _g = G;
    _h = H;
  }

};
