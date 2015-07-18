# pragma once

// Includes
// ========
// C
#include <cstddef>
// DPL
#include <DPL/utils/log.hxx>




namespace DPL {

/**
 * State identifier
 */
class StateID {

private:
  // Single member holding the identifier.
  const size_t id;

public:
  StateID (size_t stateID) : id(stateID) {}

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
