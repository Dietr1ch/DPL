#pragma once

// Includes
// ========
// DPL
#include <DPL/planners/Planner.hxx>




/**
 * \brief An abstract class defining (min-)heap operations.
 *
 * REVIEW: Maybe this template should receive a type T and a comparator<T>.
 *
 * \note It may be important to some to note that these virtual functions
 *         are not expected to give an overhead as the Concrete derived
 *         classes should be used directly. The actual class may even be a
 *         template argument as the virtual functions should be sufficient
 *         for most algorithms making all heaps replaceable with no overhead.
 *
 * \param NodeType: Node to be hold on the heap. Should extend DPL::Node
 * \param KeyType:  Type to use for comparisons.
 * \param keySize:  Number of comparisons ('1+tie breaks').
 */
template<
  typename NodeType,
  typename KeyType,
  int      keySize=1
>
class Queue {

public:
  /**
   * Ensure that templating matches the expected types
   */
  static_assert(std::is_base_of<Node<KeyType, keySize>, NodeType>::value, "NodeType must derive from Node<K,n>");

  /**
   * \brief Type of Key used.
   *
   * The Key Type is the matching Key<KeyType, keySize>.
   */
  typedef Key<KeyType, keySize> _Key;

  /**
   * \brief An element of the heap, a pair (Node, Key).
   *
   * \note Known as HEAPELEMENT on the SBPL.
   */
  struct Element {
    NodeType* node;
    _Key      key;

    Element() : node(nullptr) {}
    Element(NodeType* n) : node(n) {}
    Element(NodeType* n, _Key k) : node(n),key(k) {}

    // Comparison Operators
    // --------------------
    inline bool operator ==(const Element& e) const { return key == e.key; }
    inline bool operator !=(const Element& e) const { return key != e.key; }
    inline bool operator  <(const Element& e) const { return key  < e.key; }
    inline bool operator  >(const Element& e) const { return key  > e.key; }
    inline bool operator <=(const Element& e) const { return key <= e.key; }
    inline bool operator >=(const Element& e) const { return key >= e.key; }
  };


  // Operations
  // ==========
  virtual Maybe<Element> peek() const = 0;
  virtual Maybe<Element> pop() = 0;
  virtual void insert(NodeType &n, const _Key k) = 0;

  virtual void clear() = 0;


  // Aliases
  // =======
  inline
  Maybe<Element> top() const {
    return peek();
  }
  inline
  Maybe<Element> remove() const {
    return pop();
  }
  inline
  void push(NodeType &n, const _Key k) {
    insert(n,k);
  }

};
