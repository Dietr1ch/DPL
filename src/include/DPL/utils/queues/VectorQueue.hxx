#pragma once

// Includes
// ========
// C
#include <cassert>
// DPL
#include <DPL/utils/queues/IndexedQueue.hxx>




namespace DPL {

/**
 * A Queue that operates on Nodes<KeyType, keySize> implemented over a vector<Element>
 *
 * \param NodeType: Node to be hold on the heap. Should extend DPL::Node
 * \param *index:   Node member that holds the index for this heap.
 * \param KeyType:  Type to use for comparisons.
 * \param keySize:  Number of comparisons ('1+tie breaks').
 * \param MIN_QUEUE: Whether lower key is prefered or not.
 */
template<
  typename   NodeType,
  IndexType (NodeType::*index),
  typename   KeyType,
  int        keySize=1,
  bool       MIN_QUEUE=true
>
class VectorQueue : public IndexedQueue<
                             NodeType,
                             index,
                             KeyType,
                             keySize,
                             MIN_QUEUE
                           > {

public:

  // Type aliases
  // ============
  typedef IndexedQueue<NodeType, index, KeyType, keySize> _IndexedQueue;
  typedef typename _IndexedQueue::Element _Element;
  typedef typename _IndexedQueue::_Key _Key;


private:
  /**
   * 'Array' holding the data for the heap.
   *
   * \note Standard vector implementation has the same behaviour as the
   *         manually managed Element[] array that was used on the SBPL
   *       It doubles the allocated memory when the capacity limit was reached.
   */
  vector<_Element> heap;

  /**
   * Tracks heap usage.
   *
   * \note The heap ignores the 0th element.
   */
  size_t size = 0;

  struct {
    /**
     * Percolate operations
     */
    int percolates = 0;
  } stats;

public:
  VectorQueue(size_t startingSize=8192) {
    heap.reserve(startingSize);

    _Element dummy;
    heap.push_back(dummy);

    assert(heap.size()>=1);
    assert(!heap[0].node);
  }
  ~VectorQueue() {
    assert(heap.size()>=1);
  }


  // Queue operations
  // ===============

  // Peek
  // ----
  /**
   * \brief Peek the head of the heap without removing it
   */
  optional<_Element> peek() const {
    if(size)
      return _peek();
    else
      return optional<_Element>();
  }

  /**
   * \brief Unsafe Peek
   */
  inline
  _Element _peek() const {
    assert(heap.size()>=1);

    return heap[1];
  }


  // Pop
  // ---
  /**
   * Removes and return the head of the Queue
   */
  optional<_Element> pop() {
    optional<_Element> ret;
    if (size == 0)
      return ret;

    return _pop();
  }

  /**
   * Removes and return the head of the Queue
   */
  _Element _pop() {
    assert(heap.size()>=1);

    _Element e(_peek());
    assert(e.node);

    percolateDown(1, heap[size--]);
    return e;
  }


  // Insert
  // ------
  /**
   * \brief Inserts a node into the heap
   *
   * \param n: Node to insert.
   * \param k: Key for the new Node.
   */
  void insert(NodeType &n, const _Key k) {
    if (n.*index != 0) {
      err_dst << "Node is already in heap\n";
      throw new exception();
    }
    _insert(n, k);
  }

  /**
   * \brief Unsafe insert
   *
   * \note Assumes that the Node is not currently on the Queue.
   *
   * \param n: Node to insert.
   * \param k: Key for the new Node.
   */
  inline
  void _insert(NodeType &n, const _Key k) {
    assert(heap.size()>=1);

    dbg_dst << "Inserting node " << n << " (" << k << ")";
    _Element e(n, k);
    percolateUp(++size, e);
  }


  // Remove
  // ------
  /**
   * \brief Removes a node from the heap.
   *
   * \param n: Node to remove.
   */
  void remove(NodeType &n) {
    if (n.*index == 0) {
      err_dst << "Node is not in heap";
      throw new exception();
    }
    _remove(n);
  }

  /**
   * \brief unsafe remove.
   *
   * \param n: Node to remove. Should be on the heap.
   */
  inline
  void _remove(NodeType &n) {
    assert(heap.size()>=1);

    percolate(n.*index, heap[size--]);
    n.*index = 0;
  }


  // Update
  // ------
  /**
   * Updates a node
   *
   * \param i:      Index of an existing Node.
   * \param newKey: New Key for the Node.
   */
  void updatei(size_t i, const _Key newKey) {
    if (i==0) {
      err_dst << "Node is not in heap";
      throw new exception();
    }
    _updatei(i, newKey);
  }

  /**
   * \brief Unsafe update.
   *
   * \note Assumes that Node at index i exists.
   *
   * \param i:      Index of an existing Node.
   * \param newKey: New Key for the Node.
   */
  inline
  void _updatei(size_t i, const _Key newKey) {
    assert(heap.size()>=1);

    dbg_dst << "Updating node " << heap[i].node << " (" << newKey << ")";
    if(heap[i].key != newKey) {
      heap[i].key = newKey;
      percolate(i, heap[i]);
    }
  }


  // Upsert
  // ------
  void upsert(NodeType &n, const _Key newKey) {
    if(n.*index==0)
      insert(n, newKey);
    else
      // update(n, newKey);
      updatei(n.*index, newKey);
  }

  // Clear
  // -----
  /**
   * Removes all elements from the heap
   */
  void clear() {
    assert(heap.size()>=1);

    for (size_t i=1; i<=size; ++i)
      heap[i].node->*index = 0;  // Invalidates Node*
    size = 0;
  }



  // Queue state
  // ==========
  /**
   * \brief Checks if the heap is empty
   */
  bool empty() {
    assert(heap.size()>=1);

    return size==0;
  }
  /**
   * \brief Checks if a node is in the heap
   */
  bool contains(NodeType &n) {
    assert(heap.size()>=1);

    return n.*index!=0;
  }


  // Print
  // =====
  /**
   * Output stream support.
   */
  friend ostream& operator <<(ostream& os, const VectorQueue& q) {
    // TODO: proper logging.
    _ignore(q);
    os << "VectorQueue {???}"; // << q.heap << "}";
    return os;
  }
  /**
   * EasyLogging++ stream support.
   */
  virtual void log(OutStream& os) const {
    // TODO: proper logging.
    os << "VectorQueue {???}";  // << heap << "}";
  }



private:

  // Internal operations
  // ===================
  void percolateUp(size_t hole, _Element e) {
    assert(heap.size()>=1);

    if (size==0)
      return;

    for (; hole>1 && e<heap[hole/2]; hole/=2) {
      stats.percolates++;
      heap[hole] = heap[hole/2];
      heap[hole].node->*index = hole;
    }
    heap[hole] = e;
    heap[hole].node->*index = hole;
  }

  void percolateDown(size_t hole, _Element e) {
    assert(heap.size()>=1);

    if (size==0)
      return;

    for(size_t child; 2*hole <= size; hole=child) {
      child = 2*hole;

      if (child!=size && heap[child+1]<heap[child])
        child++;

      if (heap[child]<e) {
        stats.percolates++;
        heap[hole] = heap[child];
        heap[hole].node->*index = hole;
      }
      else
        break;
    }
    heap[hole] = e;
    heap[hole].node->*index = hole;
  }

  void percolate(size_t hole, _Element e) {
    if (size==0)
      return;

    if (hole>1 && heap[hole/2]>e)
      percolateUp(hole, e);
    else
      percolateDown(hole, e);
  }
};

}
