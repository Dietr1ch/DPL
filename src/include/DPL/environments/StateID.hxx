# pragma once

// Includes
// ========
// C
#include <cassert>
#include <cstddef>
// DPL
#include <DPL/utils/log.hxx>




namespace DPL {

/**
 * \brief State identifier.
 *
 * Meaningful type for those numbers that identify an State on the MDP.
 *
 * REVIEW: Should this use a template argument defaulting to size_t?.
 *           Apparently it adds no benefit.
 */
class StateID {
public:
  static constexpr size_t invalidID = std::numeric_limits<size_t>::max();

private:
  // Single member holding the identifier.
  const size_t id;

public:
  StateID() : id(invalidID) {}
  StateID(size_t stateID) : id(stateID) {
    assert(stateID!=invalidID);
  }

  /**
   * Cast to size_t
   */
  explicit operator size_t() const {
    return id;
  }


  // Operators
  // =========
  inline
  bool operator ==(const StateID &state) const {
    return id == state.id;
  }
  inline
  bool valid() const {
    return id!=invalidID;
  }


  // Logging
  // =======
  /**
   * Output stream support.
   */
  friend ostream& operator <<(ostream& os, const StateID& s) {
    return os << "StateID[" << s.id << "]";
  }
  /**
   * EasyLogging++ stream support.
   */
  void log(OutStream& os) const {
    os << "StateID[" << id << "]";
  }
};

}
