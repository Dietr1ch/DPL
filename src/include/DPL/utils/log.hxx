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

// #define str_alg "alg"
// #define str_dst "dst"
// #define str_env "env"
// #define str_inf "inf"
// #define str_mem "mem"
#define str_alg "algorithm"
#define str_dst "data-structure"
#define str_env "environment"
#define str_inf "information"
#define str_mem "memory"

el::Logger* logger_alg = el::Loggers::getLogger(str_alg);
el::Logger* logger_dst = el::Loggers::getLogger(str_dst);
el::Logger* logger_env = el::Loggers::getLogger(str_env);
el::Logger* logger_inf = el::Loggers::getLogger(str_inf);
el::Logger* logger_mem = el::Loggers::getLogger(str_mem);

#define dbg_alg DCLOG(DEBUG, str_alg)
#define dbg_dst DCLOG(DEBUG, str_dst)
#define dbg_env DCLOG(DEBUG, str_env)
#define dbg_inf DCLOG(DEBUG, str_inf)
#define dbg_mem DCLOG(DEBUG, str_mem)

#define log_alg DCLOG(INFO, str_alg)
#define log_dst DCLOG(INFO, str_dst)
#define log_env DCLOG(INFO, str_env)
#define log_inf CLOG(INFO,  str_inf)
#define log_mem DCLOG(INFO, str_mem)

#define warn_alg DCLOG(WARNING, str_alg)
#define warn_dst DCLOG(WARNING, str_dst)
#define warn_env DCLOG(WARNING, str_env)
#define warn_inf CLOG(WARNING,  str_inf)
#define warn_mem DCLOG(WARNING, str_mem)

#define err_alg DCLOG(ERROR, str_alg)
#define err_dst DCLOG(ERROR, str_dst)
#define err_env DCLOG(ERROR, str_env)
#define err_inf CLOG(ERROR,  str_inf)
#define err_mem DCLOG(ERROR, str_mem)




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
template <
  typename T
  >
void _ignore(T &&) {}
