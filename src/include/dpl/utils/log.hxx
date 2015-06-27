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
el::Logger* logger_dst = el::Loggers::getLogger("data-structure");
el::Logger* logger_env = el::Loggers::getLogger("environment");
el::Logger* logger_inf = el::Loggers::getLogger("information");
el::Logger* logger_mem = el::Loggers::getLogger("memory");

#define dbg_alg DCLOG(DEBUG, "algorithm")
#define dbg_dst DCLOG(DEBUG, "data-structure")
#define dbg_env DCLOG(DEBUG, "environment")
#define dbg_inf DCLOG(DEBUG, "information")
#define dbg_mem DCLOG(DEBUG, "memory")

#define log_alg DCLOG(INFO, "algorithm")
#define log_dst DCLOG(INFO, "data-structure")
#define log_env DCLOG(INFO, "environment")
#define log_inf CLOG(INFO, "information")
#define log_mem DCLOG(INFO, "memory")

#define warn_alg DCLOG(WARNING, "algorithm")
#define warn_dst DCLOG(WARNING, "data-structure")
#define warn_env DCLOG(WARNING, "environment")
#define warn_inf CLOG(WARNING, "information")
#define warn_mem DCLOG(WARNING, "memory")

#define err_alg DCLOG(ERROR, "algorithm")
#define err_dst DCLOG(ERROR, "data-structure")
#define err_env DCLOG(ERROR, "environment")
#define err_inf CLOG(ERROR, "information")
#define err_mem DCLOG(ERROR, "memory")




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
