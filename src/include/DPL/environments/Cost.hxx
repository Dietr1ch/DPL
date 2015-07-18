#pragma once

// Includes
// ========
// C
#include <cstdint>
// C++
#include <limits>
// DPL
#include <DPL/utils/log.hxx>



namespace dpl {

// Config
// ======

// Cost Numeric type
// -----------------
// Cost should NOT be unsigned.
//  * n<=-1 on unsigned Numbers. Underflow WILL ruin execution. Explicitly
//      saying that a number is expected to be positive is good, but it
//      doesn't turn the tide against hidden bugs.
//  * If intX seems not wide enough, uintX probably won't be.
//
//  NOTE: remember to update clang builtins
typedef int32_t CostNumericType;

// Overflow detection
// ------------------
/**
 * GCC5 and Clang 3.4 offer overflow detection builtins, so the above
 *   discussion becomes irrelevant.
 */
#define CAP_VALUES 1
/**
 * Simple: Reaching max value is not considered an overflow.
 * Not Simple: Reaching max value is considered an overflow.
 */
#define CAP_VALUES_SIMPLE 1
/**
 * Output to log_alg on overflows (or infinity reach).
 */
#define DEBUG_COST_OVERFLOW 1


// Setup
// =====

// Compiler specific overflow detection
// ------------------------------------
//# Clang/LLVM
#if defined(__clang__)
#define __add_overflow(A,B,R) __builtin_sadd_overflow((A),(B),(R))
#define __mul_overflow(A,B,R) __builtin_smul_overflow((A),(B),(R))
//# GNU GCC/G++
//  GCC5 offers nicer overflow detection builtins, this is only an alias.
#elif defined(__GNUC__) || defined(__GNUG__)
#define __add_overflow(A,B,R) __builtin_add_overflow((A),(B),(R))
#define __mul_overflow(A,B,R) __builtin_mul_overflow((A),(B),(R))
#endif




/**
 * \brief Cost of a path (or an action).
 *
 * This class has only an CostType with the number.
 * Provides an infinity value, and overflow detection support enables careless
 *   operation.
 */
template<
  typename CostType=CostNumericType
>
class _Cost {

public:
  /**
   * Value used to represent infinity
   */
  static constexpr CostType infinity = std::numeric_limits<CostType>::max();

private:
  CostType C;

public:
  /**
   * Default constructor starts on 0
   */
  _Cost() : C(0) {}
  /**
   * Primary costructor
   */
  _Cost(CostType cost) : C(cost) {}


  CostType operator +(const _Cost& v) const {
#if CAP_VALUES
    CostType r;
    if (__add_overflow(C, v.C, &r) || infty_check(r))
      return error_infinity(v);
    return r;
#else
    return C+v.C;
#endif
  }

  CostType operator +=(const _Cost& v) {
#if CAP_VALUES
    CostType r;
    if (__add_overflow(C, v.C, &r) || infty_check(r))
      return C = error_infinity(v);
    return C = r;
#else
    C += v.C;
    return C;
#endif
  }

  CostType operator *(const _Cost& v) const {
#if CAP_VALUES
    CostType r;
    if (__mul_overflow(C, v.C, &r) || infty_check(r))
      return error_infinity(v);
    return r;
#else
    return C*v.C;
#endif
  }

  CostType operator *=(const _Cost& v) {
#if CAP_VALUES
    CostType r;
    if ( __mul_overflow(C, v.C, &r) || infty_check(r))
      return C = error_infinity(v);
    return C = r;
#else
    C *= v.C;
    return C;
#endif
  }


  // Comparison
  // ==========
  inline
  bool operator ==(const _Cost &cost) const {
    return C == cost.C;
  }
  inline
  bool operator  >(const _Cost& cost) const {
    return C > cost.C;
  }
  inline
  bool operator  <(const _Cost& cost) const {
    return C < cost.C;
  }
  inline
  bool operator !=(const _Cost& cost) const {
    return C != cost.C;
  }
  inline
  bool operator >=(const _Cost& cost) const {
    return C >= cost.C;
  }
  inline
  bool operator <=(const _Cost& cost) const {
    return C <= cost.C;
  }


  inline
  CostType c() const {
    return C;
  }
  /**
   * Explicit cast to CostType
   */
  inline
  explicit operator CostType() const {
    return C;
  }

  /**
   * Output stream support for Costs.
   */
  friend std::ostream& operator <<(std::ostream& os, const _Cost& cost) {
    if(cost.C < _Cost::infinity)
      return os << cost.C;
    return os << "infty";
  }
  /**
   * EasyLogging++ stream support
   */
  void log(OutStream& os) const {
    if(C < _Cost::infinity)
      os << C;
    os << "infty";
  }

private:
  /**
   * Checks if value is infinity.
   */
  static inline
  bool infty_check(CostType n){
#if CAP_VALUES_SIMPLE
    _ignore(n);
    return false;
#else
    return n==infinity;
#endif
  }

  /**
   * Returns infinity. Logs on debugging.
   */
  inline
  CostType error_infinity(const _Cost& v) const {
#if DEBUG_COST_OVERFLOW
#if CAP_VALUES_SIMPLE
    log_alg << "Infinity passed! (Integer overflow: infinity<" << C << "+" << v.C << ")";
#else
    if(v.c!=_Cost::infinity)
      if(c!=_Cost::infinity)
        log_alg << "Infinity reached! (infty<=" << c << "+" << v.c << ")";
#endif
#endif
    return _Cost::infinity;
  }
};


template<typename CostType>
_Cost<CostType> operator+(const CostType& a, const _Cost<CostType>& b) {
  return (_Cost<CostType>)a + b;
}

template<typename CostType>
_Cost<CostType> operator*(const CostType& a, const _Cost<CostType>& b) {
  return (_Cost<CostType>)a * b;
}


// Aliases
// =======
/**
 * Cost of a path or an action.
 */
typedef _Cost<> Cost;

/**
 * Estimated cost of a path.
 */
typedef Cost Heuristic;

/**
 * Weight to focus the search.
 */
typedef Cost Weight;

}
