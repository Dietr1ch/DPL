#pragma once

// Includes
// ========
// C
#include <cstdint>
// DPL
#include <DPL/utils/log.hxx>




/**
 * \brief Search iteration identifier
 *
 * Used to tag search iterations, allowing to recognize old data
 *   this helps to build Lazy algorithms.
 *
 * \note Defaults to 0, which is an invalid SearchID. Can only be incremented.
 */
template<
  typename INT
>
class _SearchID : public el::Loggable {

private:
  // Single member holding the identifier.
  INT id;


public:
  _SearchID () {
    id = 0;
  }
  _SearchID (INT ID) {
    id = ID;
  }

  bool valid() const {
    return id!=(INT)0;
  }
  bool valid(_SearchID currentTime) const {
    if(valid())
      return id <= (INT) currentTime;
    return false;
  }

  /**
   * Increment SearchID
   */
  _SearchID& operator++() {
    id++;
    return *this;
  }

  /**
   * Get a copy and then increment SearchID
   */
  _SearchID operator++(int) {
    _SearchID old(*this); // copy
    operator++();
    return old;
  }

  /**
   * Cast to bool checks if the ID seems valid.
   */
  operator bool() const {
    return valid();
  }

  /**
   * Cast to integer. MUST be explicit
   */
  explicit operator INT() const {
    return id;
  }

  /**
   * Output stream support for Search IDs.
   */
  friend std::ostream& operator<< (std::ostream& os, const _SearchID& searchID) {
    if(searchID)
      return os << "Search{" << searchID.id << "}";
    return os << "Search{--}";
  }
  /**
   * EasyLogging++ stream support
   */
  virtual void log(OutStream& os) const {
    if(valid())
      os << "Search{" << id << "}";
    os << "Search{--}";
  }
};

typedef _SearchID<uint32_t> SearchID;
