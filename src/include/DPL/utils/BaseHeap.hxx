#pragma once

// Includes
// ========
// STL
#include <type_traits>
#include <functional>
#include <queue>
// DPL
#include <DPL/planners/Planner.hxx>




typedef std::size_t IndexType;

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
class BaseHeap {

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
  typedef Key<KeyType, keySize> Key;

  /**
   * \brief An element of the heap, a pair (Node, Key).
   *
   * \note Known as HEAPELEMENT on the SBPL.
   */
  struct Element {
    NodeType *node;
    Key key;

    Element() : node(nullptr) {}
    Element(NodeType *n) : node(n) {}

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
  virtual void insert(NodeType &n, const Key k) = 0;

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
  void push(NodeType &n, const Key k) {
    insert(n,k);
  }
};


/**
 * \brief A Heap that allows quick update operations.
 *
 * Usually lookup takes O(n) on a heap, because no information about the current key or
 *   the element position is known.
 * A common trick is to extend the Type used (here we use Nodes) with an index that
 *   are internal to the heap implementation, allowing the heap to reach the corresponding
 *   element on a O(1) lookup.
 * Because only some Types require to carry this per-heap extra information, we can't
 *   assume that all Nodes will have sufficient members to hold the indexes. C++ allows
 *   us to give the member name to the template, so that the heap is able to use
 *   'Node::index' to hold the index.
 *
 * REVIEW: This may have a drawback, as multiple classes may be generated when using
 *           multiple heaps on the same type (Node) instead of only one that gets an
 *           index to an array of indices (holding indices to multiple heaps).
 *         I belive that the possible extra code generated will not have a significant,
 *           or even a measurable overhead, as even having complete duplication of the
 *           heaps should not be a concern when the L1 instruction cache should be at
 *           least of 32kB (per core), so if the cache is over-occupied with the extra
 *           code, then even without it will probably remain over-occupied.
 *         Don't remove this until proper benchmarking is done.
 *
 * \param NodeType: Node to be hold on the heap. Should extend DPL::Node
 * \param *index:   Node member that holds the index for this heap.
 * \param KeyType:  Type to use for comparisons.
 * \param keySize:  Number of comparisons ('1+tie breaks').
 */
template<
  typename   NodeType,
  IndexType (NodeType::*index),
  typename   KeyType,
  int        keySize=1
  >
class IndexedHeap : public BaseHeap<NodeType, KeyType, keySize> {

public:

  // Type aliases
  // ============
  typedef BaseHeap<NodeType, KeyType, keySize> BaseHeap;
  typedef typename BaseHeap::Element Element;
  typedef typename BaseHeap::Key Key;


  // Indexed Heap Operations
  // =======================
  virtual void updatei(std::size_t elementIndex, const Key newKey) = 0;

  inline
  void update(NodeType &n, const Key newKey) {
    updatei(n.*index, newKey);
  }

  virtual void upsert(NodeType &n, const Key newKey) = 0;

  virtual void remove(NodeType &n) = 0;
};
