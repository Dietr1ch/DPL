#pragma once

// Includes
// ========
// STL
#include <vector>
// DPL
#include <dpl/utils/types.hxx>



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

};
