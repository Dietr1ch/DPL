#pragma once

// Includes
// ========
// DPL
#include <dpl/environments/Environment.hxx>
#include <dpl/environments/Cost.hxx>
#include <dpl/environments/StateID.hxx>




class LazyNodeStub : public NodeStub {
public:
  bool isTrueCost;

  LazyNodeStub(NodeStub ns, bool isTrue=true) : id(ns.id), cost(ns.cost),isTrueCost(isTrue) {}
  LazyNodeStub(StateID state, Cost c, bool isTrue=true) : id(state),cost(c),isTrueCost(isTrue) {}
};
typedef std::vector<LazyNodeStub> LazyNeigboorhood;


/**
 * An environment that supports lazy operations
 */
template <int StateArgumentCount>
class LazyEnvironment : DiscreteEnvironment<StateArgumentCount> {
  virtual LazyNeigboorhood getLazySuccessors(StateID sourceState) = 0;
  virtual LazyNeigboorhood getLazyPredecessors(StateID targetState) = 0;
};
