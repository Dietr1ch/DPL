#pragma once

// Includes
// ========
// STL
#include <type_traits>
// DPL
#include <dpl/planners/Planner.hxx>
#include <dpl/planners/Node.hxx>

//the maximum size of the heap
#define HEAP_SIZE_MAX 20000000
#define HEAP_SIZE_INIT 5000



typedef std::size_t IndexType;

/**
 * A WeakHeap that operates on Nodes<KeyType, keySize>
 */
template<typename NodeType, IndexType (NodeType::*index), typename KeyType, int keySize=1>
class WeakHeap {

  /**
   * Ensure that templating matches the expected types
   */
  static_assert(std::is_base_of<Node<KeyType, keySize>, NodeType>::value, "NodeType must derive from Node<K,n>");
  typedef Key<KeyType, keySize> Key;

public:
  /**
   *
   * SBPL: HEAPELEMENT
   */
  struct Element {
    NodeType *node;
    Key key;

    Element() : node(nullptr) {}
    Element(NodeType *n) : node(n) {}
  };

private:
  /**
   * 'Array' holding the data for the heap.
   *
   * \note Standard vector implementation has the same behaviour as the
   *         manually managed Element[] array that was used on the SBPL
   *       It doubles the allocated memory when the capacity limit was reached.
   */
  vector<Element> heap;

  /**
   * Tracks heap usage
   */
  int size = 0;
  /**
   * Tracks heap array size
   */
  int allocated;

  struct {
    /**
     * Percolate operations
     */
    int percolates = 0;
  } stats;

public:
  WeakHeap(std::size_t startingSize = HEAP_SIZE_INIT) {
    heap.reserve(startingSize);
  }
  ~WeakHeap() {
  }


  // Node operations
  // ===============
  /**
   * Peek the head of the heap without removing it
   */
  Maybe<Element> peek() const {
    if(size)
      return heap[1];
    else
      return Maybe<Element>();
  }
  /**
   * Unsafe Peek
   */
  Element _peek() const {
    return heap[1];
  }

  /**
   * Peek the head of the heap without removing it
   */
  Maybe<Element> top() const {
    return peek();
  }

  /**
   * Removes and return the head of the Heap
   */
  Maybe<Element> pop() {
    if (size == 0)
      return Maybe<Element>();

    Element e(heap[1].node);
    percolateDown(1, heap[size--]);
    return e;
  }

  /**
   * Inserts a node into the heap
   */
  void insert(NodeType &n, const Key k) {
    Element e;

    if (n.*index != 0) {
      err_dst << "Node is already in heap\n";
      throw new std::exception();
    }
    dbg_dst << "Inserting node " << n << " (" << k << ")";
    e.node = &n;
    e.key = k;
    percolateUp(++size, e);
  }

  /**
   * Unsafe insert
   */
  void _insert(const NodeType &n, const Key k);

  /**
   * Removes and return a node
   */
  Maybe<Element> remove(NodeType &n) {
    if (n.index == 0) {
      err_dst << "Node is not in heap";
      throw new std::exception();
    }

    percolate(n.index, heap[size--]);
    n.*index = 0;
  }

  /**
   * Unsafe remove
   */
  Maybe<Element> _remove(NodeType &n);

  /**
   * Updates a node
   */
  void update(NodeType &n, const Key newKey) {
    size_t i = n.*index;
    if (i==0) {
      err_dst << "Node is not in heap";
      throw new std::exception();
    }

    dbg_dst << "Updating node " << n << " (" << newKey << ")";
    if(heap[i].key != newKey) {
      heap[i].key = newKey;
      percolate(i, heap[i]);
    }
  }

  /**
   * Unsafe update
   */
  void _update(NodeType &n, const Key newKey);
  /**
   * Inserts or updates a node
   */
  void upsert(NodeType &n, const Key newKey);

  /**
   * Removes all elements from the heap
   */
  void clear() {
    for (int i = 1; i<=size; ++i)
      heap[i].node->*index = 0;  // Invalidates Node*
    size = 0;
  }


  // Heap state
  // ==========
  /**
   * Checks if the heap is empty
   */
  bool empty() {
    return size==0;
  }
  /**
   * Checks if the heap is full
   */
  bool full()  {
    return size==allocated-1;
  }
  /**
   * Checks if a node is in the heap
   */
  bool contains(NodeType &n) {
    return n.index!=0;
  }


private:
  void percolateUp(int hole, Element e) {
    if (size==0)
      return;

    for (; hole > 1 && e.key < heap[hole / 2].key; hole /= 2) {
      stats.percolates++;
      heap[hole] = heap[hole / 2];
      heap[hole].node->*index = hole;
    }
    heap[hole] = e;
    heap[hole].node->*index = hole;
  }

  void percolateDown(int hole, Element e) {
    if (size==0)
      return;

    for(int child; 2 * hole <= size; hole = child) {
      child = 2*hole;

      if (child!=size && heap[child+1].key < heap[child].key)
        child++;

      if (heap[child].key < e.key) {
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

  void percolate(int hole, Element e) {
    if (size==0)
      return;

    if (hole > 1 && heap[hole / 2].key > e.key)
      percolateUp(hole, e);
    else
      percolateDown(hole, e);
  }
};
