# pragma once

// Includes
// ========
// C
#include <cstddef>
// C++
#include <iostream>




/**
 * State identifier
 */
class StateID {

private:
  // Single member holding the identifier.
  const std::size_t id;

public:
  StateID (std::size_t stateID) : id(stateID) {}

  /**
   * Cast to size_t
   */
  explicit operator std::size_t() const {
    return id;
  }

  /**
   * Output stream support for State IDs.
   */
  friend std::ostream& operator<< (std::ostream& os, const StateID& s) {
    return os << "StateID[" << s.id << "]";
  }

  inline
  bool operator ==(const StateID &state) const {
    return id == state.id;
  }
};

