#pragma once

// Includes
// ========
// C
#include <cstddef>




typedef int KeySize;


/**
 * Multi-level comparator
 *
 * \param KeyType: Type to use for comparisons
 * \param keySize: Number of comparisons ('1+tie breaks')
 */
template<
  typename KeyType,
  KeySize  keySize
  >
class Key {
  bool changed = true;
  KeyType key[keySize] = {0};

public:

  virtual ~Key() {}

  // Comparison
  // ==========
  bool operator ==(const Key &k) const {
  for(int i=0; i<keySize; i++)
    if(key[i] != k.key[i])
      return false;
    return true;
  }
  bool operator  >(const Key& k) const {
    for(int i=0; i<keySize; i++) {
      if(key[i] > k.key[i])
        return true;
      else if(key[i] < k.key[i])
        return false;
    }
    return false;
  }
  bool operator  <(const Key& k) const {
    for(int i=0; i<keySize; i++) {
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


  KeyType& operator[](std::size_t i) {
    return key[i];
  };
  const KeyType& operator[](std::size_t i) const {
    return key[i];
  };

  // Print
  // =====
  /**
   * Output stream support for Key.
   */
  friend std::ostream& operator <<(std::ostream& os, const Key& k) {
    os << k.key[0];
    for(int i=1; i<keySize; i++)
      os << ", " << k.key[i];
    return os;
  }
  /**
   * EasyLogging++ stream support
   */
  virtual void log(OutStream& os) const {
    os << key[0];
    for(int i=1; i<keySize; i++)
      os << ", " << key[i];
  }
};
