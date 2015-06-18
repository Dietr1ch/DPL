# pragma once

// Includes
// ========
// C
#include <cstdint>
// Standard Library
#include <vector>
#include <experimental/optional>
// DPL
#include <dpl/environments/StateID.hpp>
#include <dpl/environments/Cost.hpp>
#include <dpl/planners/SearchID.hpp>
#include <dpl/utils/log.hpp>




// Solution
// ========

/**
 * \brief Sequence of States from the start to the goal (inclusively).
 *
 * NOTE: State ID's are Planning-Space-dependant, as they are made-up numbers
 *   that represent a state on the problem domain.
 *
 * REVIEW: Maybe Paths shold carry a reference to their Search Space.
 */
typedef std::vector<StateID> Path;

struct Solution {
  Path& path;
  Cost& cost;
};



// Numbers
// =======
// Semantic types
typedef unsigned int Seed;  // Machine dependant, not 'uint'
typedef double Probability;
typedef double Percentage;
typedef double Seconds;



// Optional
// ========
// optional alias
template<typename T>
using Maybe = std::experimental::optional<T>;
