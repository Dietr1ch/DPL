#pragma once

// Includes
// ========
// C++
#include <iostream>
// External libraries
#include <easylogging++.h>




using std::endl;
typedef el::base::type::ostream_t OutStream;


// Initialize Easy logging++
INITIALIZE_EASYLOGGINGPP

el::Logger* logger_alg = el::Loggers::getLogger("algorithm");
el::Logger* logger_inf = el::Loggers::getLogger("information");
el::Logger* logger_mem = el::Loggers::getLogger("memory");

#define log_alg DCLOG(INFO, "algorithm")
#define log_inf CLOG(INFO, "information")
#define log_mem DCLOG(INFO, "memory")




// Helpers
// =======
/**
 * \brief Prevents unused arguments warnings.
 *
 * Debugging may introduce variables that are only use on DEBUG blocks.
 * This helps reducing the overhead of different function arguments depending
 *   on the Debugging mode. Always use the extended Debugging mode signature
 *   and mark extra parameters as ignored.
 *
 * This overhead should be optimized away.
 */
template <typename T>
void _ignore(T &&) {}
