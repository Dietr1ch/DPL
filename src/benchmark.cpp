
// Includes
// ========
// C
#include <cstdlib>
#include <cstdio>
#include <csignal>
// C++
#include <iostream>
#include <experimental/optional>
// DPL
#include <dpl/planners/Planner.hpp>
#include <dpl/utils/types.hpp>

using std::cout;
using std::endl;




void signalHandler(int s) {
  cout << "caught signal " << s <<" exiting" << endl;

  exit(1);
}
void init(int argc, char* argv[]) {
  signal(SIGABRT, &signalHandler);
  START_EASYLOGGINGPP(argc, argv);
}


void test_showSolution(Maybe<Solution> ms) {
  if(ms) {
    Solution s = *ms;
    log_inf << "aoeu";
    log_inf << s.cost;
    for(StateID state : s.path)
      cout << state;
  }
}

void test_costOverflow(){
  Cost of = ((CostNumericType)Cost::infinity)-1;
  cout << "Biggest cost" << endl;
  cout << of << endl;

  std::flush(cout);
  cout << "Overflow attempt 1" << endl;
  of+=1;  // Simple detection does not consider this a problem
  cout << of << endl;

  std::flush(cout);
  cout << "Overflow attempt 2" << endl;
  of+=1;
  cout << of << endl;

  std::flush(cout);
  cout << "Overflow attempt 3" << endl;
  of+=1;
  cout << of << endl;
}

void test_loopLogs(){
  for(int i=0; i<10; i++) {
    LOG_EVERY_N(2, INFO) << "Logged every second iter";
    LOG_AFTER_N(2, INFO) << "Log after 2 hits; " << i;
    LOG_N_TIMES(3, INFO) << "Log only 3 times; " << i;
  }
}


int main(int argc, char* argv[]) {
  init(argc, argv);

  Cost c(10);
  Cost i = Cost::infinity;
  SearchID search(0);
  StateID state(999);
  c+=10;
  c*=4;


  cout << i << endl;
  cout << c << endl;
  cout << c+c << endl;
  cout << c*c << endl;

  cout << "ID: " << search << endl;
  cout << "ID: " << ++search << endl;
  cout << "ID: " << search++ << endl;
  cout << "ID: " << search << endl;
  cout << state << endl;

  LOG(INFO) << "My first info log using default logger";

  return 0;
}
