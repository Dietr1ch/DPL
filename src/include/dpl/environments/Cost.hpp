#pragma once

// Includes
// ========
// C
#include <cstdint>
// C++
#include <limits>
// DPL
#include <dpl/utils/log.hpp>


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
#define __mul_overflow(A,B,R) __builtin_sadd_overflow((A),(B),(R))
//# GNU GCC/G++
//  GCC5 offers nicer overflow detection builtins, this is only an alias.
#elif defined(__GNUC__) || defined(__GNUG__)
#define __add_overflow(A,B,R) __builtin_add_overflow((A),(B),(R))
#define __mul_overflow(A,B,R) __builtin_add_overflow((A),(B),(R))
#endif




/**
 * A cost of a path (or an action).
 *
 * This class has only an INT with the number.
 * Provides an infinity value, and overflow detection support enables careless
 *   operation.
 */
template<typename INT>
class _Cost : public el::Loggable {

public:
  /**
   * Value used to represent infinity
   */
  static constexpr INT infinity = std::numeric_limits<INT>::max();

private:
  INT c;

public:
  /**
   * Default constructor starts on 0
   */
  _Cost() : c(0) {}
  /**
   * Primary costructor
   */
  _Cost(INT cost) : c(cost) {}


  INT operator +(const _Cost& v) {
#if CAP_VALUES
    INT r;
    if (__add_overflow(c, v.c, &r) || infty_check(r))
      return error_infinity(v);
    return r;
#else
    return c+v.c;
#endif
  }

  INT operator +=(const _Cost& v) {
#if CAP_VALUES
    INT r;
    if (__add_overflow(c, v.c, &r) || infty_check(r))
      return c = error_infinity(v);
    return c = r;
#else
    c += v.c;
    return c;
#endif
  }

  INT operator *(const _Cost& v) {
#if CAP_VALUES
    INT r;
    if (__mul_overflow(c, v.c, &r) || infty_check(r))
      return error_infinity(v);
    return r;
#else
    return c*v.c;
#endif
  }

  INT operator *=(const _Cost& v) {
#if CAP_VALUES
    INT r;
    if ( __mul_overflow(c, v.c, &r) || infty_check(r))
      return c = error_infinity(v);
    return c = r;
#else
    c *= v.c;
    return c;
#endif
  }

  /**
   * Explicit cast to INT
   */
  explicit operator INT() const {
    return c;
  }

  /**
   * Output stream support for Costs.
   */
  friend std::ostream& operator <<(std::ostream& os, const _Cost& cost) {
    if(cost.c < _Cost::infinity)
      return os << cost.c;
    return os << "infty";
  }
  /**
   * EasyLogging++ stream support
   */
  virtual void log(OutStream& os) const {
    if(c < _Cost::infinity)
      os << c;
    os << "infty";
  }

private:
  /**
   * Checks if value is infinity.
   */
  static inline
  bool infty_check(INT n){
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
  INT error_infinity(const _Cost& v) {
#if DEBUG_COST_OVERFLOW
#if CAP_VALUES_SIMPLE
    log_alg << "Infinity passed! (Integer overflow: infinity<" << c << "+" << v.c << ")";
#else
    if(v.c!=_Cost::infinity)
      if(c!=_Cost::infinity)
        log_alg << "Infinity reached! (infty<=" << c << "+" << v.c << ")";
#endif
#endif
    return _Cost::infinity;
  }
};


// Aliases
// =======
/**
 * Cost of a path
 */
typedef _Cost<CostNumericType> Cost;

/**
 * Estimated cost of a path
 */
typedef Cost Heuristic;

/**
 * Weight to focus the search
 */
typedef Cost Weight;
