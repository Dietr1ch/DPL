#pragma once

// Includes
// ========
// DPL
#include <DPL/utils/queues/Queue.hxx>




namespace DPL {

typedef size_t IndexType;

/**
 * \brief A Queue that allows quick update operations.
 *
 * Usually lookup takes O(n) on a queue, because no information about the current key or
 *   the element position is known.
 * A common trick is to extend the Type used (here we use Nodes) with an index that
 *   are internal to the queue implementation, allowing the queue to reach any element
 *   on a O(1) lookup.
 * Because only some Types require to carry this per-queue extra information, we can't
 *   assume that all Nodes will have sufficient members to hold the indexes. C++ allows
 *   us to give the member name to the template, so that the queue is able to use
 *   'Node::index' to hold the index.
 *
 *
 *
 *  * https://dl.acm.org/citation.cfm?id=2523707
 *
 * REVIEW: This may have a drawback, as multiple classes may be generated when using
 *           multiple queues on the same type (Node) instead of only one that gets an
 *           index to an array of indices (holding indices to multiple queues).
 *         I belive that the possible extra code generated will not have a significant,
 *           or even a measurable overhead, as even having complete duplication of the
 *           queues should not be a concern when the L1 instruction cache should be at
 *           least of 32kB (per core), so if the cache is over-occupied with the extra
 *           code, then even without it will probably remain over-occupied.
 *         Don't remove this until proper benchmarking is done.
 *
 *
 * [EdelkampEK12] Compares multiple queues, suggesting this order for new Indexed Queues:
 *
 *   * binary heap  (Implemented on VectorQueue)
 *   * pairing heap
 *   * weak queue
 *   * Fibonacci heap
 *   * weak heap
 *   * rank−relaxed weak queue
 *   * rank−relaxed weak heap
 *
 * [EdelkampEK12]:https://dl.acm.org/citation.cfm?id=2523707
 *
 * \param NodeType: Node to be hold on the Queue. Should extend DPL::Node
 * \param *index:   Node member that holds the index for this Queue.
 * \param KeyType:  Type to use for comparisons.
 * \param keySize:  Number of comparisons ('1+tie breaks').
 * \param MIN_QUEUE: Whether lower key is prefered or not.
 */
template<
  typename   NodeType,
  IndexType (NodeType::*index),
  typename   K,
  bool       MIN_QUEUE=true
>
class IndexedQueue : public Queue<
                              NodeType,  // Type of Node used.
                              K,   // Type of the Key to use.
                              MIN_QUEUE  // Queue ordering.
                            > {

public:

  // Type aliases
  // ============
  typedef Queue<NodeType, K> _Queue;
  typedef typename _Queue::Element _Element;


  // Indexed Queue Operations
  // =======================
  virtual void updatei(size_t elementIndex, const K newKey) = 0;

  inline
  void update(NodeType& n, const K newKey) {
    updatei(n.*index, newKey);
  }

  virtual void upsert(NodeType& n, const K newKey) = 0;

  virtual void remove(NodeType& n) = 0;
};

}
