# pragma once

// Includes
#include <iostream>




namespace DPL {

// Mode Detection
// ==============
enum Target {
  release=0,
  debug
};
std::ostream& operator <<(std::ostream& os, const Target c) {
  switch(c) {
    case release:  os << "Release"; break;
    case debug:    os << "Debug"; break;
    default:       os << "Unknown"; break;
  }
  return os;
}

const Target target =
#ifdef NDEBUG
                      Target::release;
#else
                      Target::debug;
#endif



// Compiler Detection
// ==================
// REVIEW: Is there a cleaner way?
enum Compiler {
  gcc=0,
  clang,
  intel,
  hp,
  ibm,
  mvs,
  pgi,
  sun,
  unknown
};
std::ostream& operator <<(std::ostream& os, const Compiler c) {
  switch(c) {
    case clang:  os << "Clang/LLVM"; break;
    case gcc:    os << "GCC"; break;
    case intel:  os << "Intel ICC/ICPC"; break;
    case hp:     os << "Hewlett-Packard C/aC++"; break;
    case ibm:    os << "IBM XL C/C++"; break;
    case mvs:    os << "Microsoft Visual Studio"; break;
    case pgi:    os << "Portland Group PGCC/PGCPP"; break;
    case sun:    os << "Oracle Solaris Studio"; break;
    default:     os << "Unknown"; break;
  }
  return os;
}

const Compiler compiler =
#if defined(__clang__)                            // Clang/LLVM
                          Compiler::clang;
#elif defined(__ICC) || defined(__INTEL_COMPILER) // Intel ICC/ICPC.
                          Compiler::intel;
#elif defined(__GNUC__) || defined(__GNUG__)      // GNU GCC/G++.
                          Compiler::gcc;
#elif defined(__HP_cc) || defined(__HP_aCC)       // Hewlett-Packard C/aC++.
                          Compiler::hp;
#elif defined(__IBMC__) || defined(__IBMCPP__)    // IBM XL C/C++.
                          Compiler::ibm;
#elif defined(_MSC_VER)                           // Microsoft Visual Studio.
                          Compiler::mvs;
#elif defined(__PGI)                              // Portland Group PGCC/PGCPP.
                          Compiler::pgi;
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC) // Oracle Solaris Studio.
                          Compiler::sun;
#else                                             // Unknown compiler.
                          Compiler::unknown;
#endif


}
