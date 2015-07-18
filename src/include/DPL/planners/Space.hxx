#pragma once

// Includes
// ========
// DPL
#include <DPL/utils/types.hxx>




/**
 * \brief A Search Space.
 *
 * This handles the Search Space for the Planner.
 *
 * \note Not defined on the SBPL, but used multiple times.
 *
 * \param NodeType Type of the node used.
 */
template<
  typename NodeType
  >
class Space {

  //StateID start;
  //StateID goal;

  SearchID currentSearch;

  // Statistics
  // ----------
#if STATISTICS
  /** Statistics */
  struct {
    uint expansions = 0;
    uint nodesGenerated = 0;

    /** Per-search-episode stuff */
    struct{
      /** Expansions made on this space */
      std::vector<uint> expansions;
      std::vector<uint> pathLength;
    } perSearch;
  } stats;
#endif

public:
  Space() {}
  ~Space() {}

  NodeType& getNode(StateID state);

  NodeType& getStart(StateID state);
};
