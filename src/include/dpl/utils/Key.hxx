#pragma once

// Includes
// ========
// C
#include <cstddef>




/**
 * Multi-level comparator
 */
template<typename K, int compareSize>
class Key {
  K key[compareSize];

public:
  Key() {
    for(int i=0; i<compareSize; i++)
      key[i] = K(0);
  }

  // Comparison
  // ==========
  bool operator ==(const Key &k) const {
  for(int i=0; i<compareSize; i++)
    if(key[i] != k.key[i])
      return false;
    return true;
  }
  bool operator  >(const Key& k) const {
    for(int i=0; i<compareSize; i++) {
      if(key[i] > k.key[i])
        return true;
      else if(key[i] < k.key[i])
        return false;
    }
    return false;
  }
  bool operator  <(const Key& k) const {
    for(int i=0; i<compareSize; i++) {
      if(key[i] < k.key[i])
        return true;
      else if(key[i] > k.key[i])
        return false;
    }
    return false;
  }
  bool operator !=(const Key& k) const {
    return !(operator ==(k));
  }
  bool operator >=(const Key& k) const {
    return !(operator <(k));
  }
  bool operator <=(const Key& k) const {
    return !(operator >(k));
  }


  K& operator[](std::size_t i) {
    return key[i];
  };
  const K& operator[](std::size_t i) const {
    return key[i];
  };

  // Print
  // =====
  /**
   * Output stream support for Key.
   */
  friend std::ostream& operator <<(std::ostream& os, const Key& k) {
    os << k.key[0];
    for(int i=1; i<compareSize; i++)
      os << ", " << k.key[i];
    return os;
  }
  /**
   * EasyLogging++ stream support
   */
  virtual void log(OutStream& os) const {
    os << key[0];
    for(int i=1; i<compareSize; i++)
      os << ", " << key[i];
  }
};
