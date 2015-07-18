#define DPL "DPL Planning Library"

// Includes
// ========
// C
#include <cstdlib>
#include <cstdio>
#include <csignal>
// DPL
#include <DPL/planners/AStar/AStarPlanner.hxx>


using std::cout;
using std::endl;
using namespace dpl;




void signalHandler(int s) {
  cout << "caught signal " << s << ". Terminating" << endl;

  exit(1);
}


void init(int argc, char* argv[]) {
#ifdef NDEBUG
  cout << "Starting on Release Mode" << endl;
#else
  cout << "Starting on Debug Mode" << endl;
#endif
  signal(SIGABRT, &signalHandler);
  START_EASYLOGGINGPP(argc, argv);

  // REVIEW: Load config from files?
  el::Loggers::addFlag(el::LoggingFlag::AutoSpacing);
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
}


void test_showSolution(Maybe<Solution> ms) {
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


void print(Maybe<AStarSpace<>::_Open::Element> e) {
  if(e)
    log_dst << "Top node: " << *e->node << " (" << e->key << ")";
}


/**
 *
 */
class FakeEnv : public DiscreteEnvironment<> {

public:
  FakeEnv() {}
  ~FakeEnv() {}

  bool loadFile(const std::string file) {
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
  std::string toString(const StateID state) {
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

  auto a = heap._peek().node;

  // Update node a (increase value)
  k1[0]= k2[0].c()*2;
  k1[1]= 2*k2[1];
  heap.update(n1, k1);
  print(heap.peek());

  auto b = heap._peek().node;

  if(a!=b)
    log_dst << "Queue reordered elements";
  if(b<a)
    log_dst << "min heap";

  log_dst << "Removing all objects";
  while(true){
    Maybe<AStarSpace<>::_Open::Element> e = heap.pop();
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
  SearchID search(0);
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
