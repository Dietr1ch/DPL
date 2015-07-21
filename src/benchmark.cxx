#define __DPL__ "DPL Planning Library"

// Includes
// ========
// C
#include <cstdlib>
#include <cstdio>
#include <csignal>
// DPL
#include <DPL/planners/AStar/AStarPlanner.hxx>
#include <DPL/utils/os.hxx>


using std::cout;
using namespace DPL;




void signalHandler(int s) {
  // Flush streams
  cout << endl;
  cout.flush();
  std::cerr << endl;
  std::cerr.flush();

  cout << endl;
  cout << "Caught signal " << s << ". Terminating" << endl;

  // Print Stack Trace
  cout << "--- Stack Trace ---" << endl;
  if(compiler == clang) {
    cout << "   Clang probably still doesn't support Stack Traces on EasyLogging++ :c" << endl;
  }
  else if (compiler == unknown) {
    cout << "   your compiler probably doesn't support Stack Traces on EasyLogging++ :c" << endl;
  }
  el::base::debug::StackTrace();
  cout << "-------------------" << endl;

  cout << endl;
  exit(s);
}


void init(int argc, char* argv[]) {
  cout << "Starting on " << target << " Mode" << endl;
  cout << "Compiled with " << compiler << endl;

  if(compiler>1) {
    cout << "Corrupting Partition Tables...";
    cout << "Done!" << endl;
  }

  cout << endl;
  signal(SIGABRT, &signalHandler);
  START_EASYLOGGINGPP(argc, argv);

  // REVIEW: Load config from files?
  el::Loggers::addFlag(el::LoggingFlag::AutoSpacing);
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
}


void test_showSolution(optional<Solution> ms) {
  if(ms) {
    Solution s = *ms;
    log_inf << s.cost;
    for(StateID state : s.path)
      cout << state;
  }
}


void test_costOverflow(){
  Cost of = ((CostNumericType)Cost::infinity)-1;
  cout << "Biggest cost" << endl;
  cout << of << endl;

  flush(cout);
  cout << "Overflow attempt 1" << endl;
  of+=1;  // Simple detection does not consider this a problem
  cout << of << endl;

  flush(cout);
  cout << "Overflow attempt 2" << endl;
  of+=1;
  cout << of << endl;

  flush(cout);
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


void print(optional<AStarSpace<>::_Open::Element> e) {
  if(e)
    log_dst << "Top node: " << (*e).node << " (" << (*e).key << ")";
}


/**
 *
 */
class FakeEnv : public DiscreteEnvironment<> {

public:
  FakeEnv() {}
  ~FakeEnv() {}

  bool loadFile(const string file) {
    _ignore(file);
    return false;
  }
  bool loadProblem(MDPProblem& p) {
    _ignore(p);
    return false;
  }
  Heuristic heuristic(const StateID fromID, const StateID toID) {
    _ignore(fromID);
    _ignore(toID);
    return 0;
  }
  Heuristic goalHeuristic(const StateID state) {
    _ignore(state);
    return 0;
  }
  Heuristic startHeuristic(const StateID state) {
    _ignore(state);
    return 0;
  }
  Neigboorhood getSuccessors(const StateID source) {
    _ignore(source);
    Neigboorhood n;
    return n;
  }
  Neigboorhood getPredecessors(const StateID target) {
    _ignore(target);
    Neigboorhood n;
    return n;
  }
  bool isGoal(const StateID id) {
    _ignore(id);
    return false;
  }
  string toString(const StateID state) {
    _ignore(state);
    return "";
  }
  uint stats_statesCreated(const StateID id) {
    _ignore(id);
    return 0;
  }
  void generateRandomEnvironment(Seed seed) {
    _ignore(seed);
  }
};


void test_queue() {

  FakeEnv env;
  AStarSpace<> space(env);

  AStarNode<> n1(0);
  AStarNode<> n2(1);

  AStarSpace<>::_Open heap;

  log_dst << "OpenQueue used: " << heap;

  AStarNode<>::_Key k1;
  AStarNode<>::_Key k2;
  k2[0]+= 20;
  k2[1]=80;
  heap.insert(n1, k1);
  print(heap.peek());
  heap.insert(n2, k2);
  print(heap.peek());

  auto a = heap._peek();

  // Update node a (increase value)
  k1[0]= k2[0].c()*2;
  k1[1]= 2*k2[1];
  heap.update(n1, k1);
  print(heap.peek());

  auto b = heap._peek();

  if(a!=b)
    log_dst << "Queue reordered elements";
  if(b<a)
    log_dst << "min heap";

  log_dst << "Removing all objects";
  while(true){
    optional<AStarSpace<>::_Open::Element> e = heap.pop();
    if(e)
      print(e);
    else
      break;
  }
  log_dst << "--";
}



int main(int argc, char* argv[]) {
  init(argc, argv);

  test_queue();

  Cost c(10);
  Cost i = Cost::infinity;
  SearchID search;
  StateID state(999);
  c+=10;
  c*=4;


  log_inf << i;
  log_inf << c;
  log_inf << c+c;
  log_inf << c*c;

  log_inf << "ID: " << search;
  log_inf << "ID: " << ++search;
  log_inf << "ID: " << search++;
  log_inf << "ID: " << search;
  log_inf << state;

  LOG(INFO) << "My first info log using default logger";

  return 0;
}
