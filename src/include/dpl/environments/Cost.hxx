#pragma once

// Includes
// ========
// C
#include <cstdint>
// C++
#include <limits>
// DPL
#include <dpl/utils/log.hxx>


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
 * A cost of a path (or an action).
 *
 * This class has only an INT with the number.
 * Provides an infinity value, and overflow detection support enables careless
 *   operation.
 */
template<typename INT>
class _Cost {

public:
  /**
   * Value used to represent infinity
   */
  static constexpr INT infinity = std::numeric_limits<INT>::max();

private:
  INT C;

public:
  /**
   * Default constructor starts on 0
   */
  _Cost() : C(0) {}
  /**
   * Primary costructor
   */
  _Cost(INT cost) : C(cost) {}


  INT operator +(const _Cost& v) const {
#if CAP_VALUES
    INT r;
    if (__add_overflow(C, v.C, &r) || infty_check(r))
      return error_infinity(v);
    return r;
#else
    return C+v.C;
#endif
  }

  INT operator +=(const _Cost& v) {
#if CAP_VALUES
    INT r;
    if (__add_overflow(C, v.C, &r) || infty_check(r))
      return C = error_infinity(v);
    return C = r;
#else
    C += v.C;
    return C;
#endif
  }

  INT operator *(const _Cost& v) const {
#if CAP_VALUES
    INT r;
    if (__mul_overflow(C, v.C, &r) || infty_check(r))
      return error_infinity(v);
    return r;
#else
    return C*v.C;
#endif
  }

  INT operator *=(const _Cost& v) {
#if CAP_VALUES
    INT r;
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
  bool operator ==(const _Cost &cost) const {
    return C == cost.C;
  }
  bool operator  >(const _Cost& cost) const {
    return C > cost.C;
  }
  bool operator  <(const _Cost& cost) const {
    return C < cost.C;
  }
  bool operator !=(const _Cost& cost) const {
    return C != cost.C;
  }
  bool operator >=(const _Cost& cost) const {
    return C >= cost.C;
  }
  bool operator <=(const _Cost& cost) const {
    return C <= cost.C;
  }


  INT c() const {
    return C;
  }
  /**
   * Explicit cast to INT
   */
  explicit operator INT() const {
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
  virtual void log(OutStream& os) const {
    if(C < _Cost::infinity)
      os << C;
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
  INT error_infinity(const _Cost& v) const {
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


template<typename INT>
_Cost<INT> operator+(const INT& a, const _Cost<INT>& b) {return (_Cost<INT>)a + b;}

template<typename INT>
_Cost<INT> operator*(const INT& a, const _Cost<INT>& b) {return (_Cost<INT>)a * b;}


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
