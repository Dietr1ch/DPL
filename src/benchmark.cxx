
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
#include <dpl/planners/Planner.hxx>
#include <dpl/planners/AStar/AStarNode.hxx>
#include <dpl/utils/types.hxx>
#include <dpl/utils/WeakHeap.hxx>

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

void print(Maybe<AStarHeap::Element> e) {
  if(e)
    log_dst << "Top node: " << *e->node << " (" << e->key << ")";
}

void test_heap() {

  AStarNode n1(0);
  AStarNode n2(1);

  AStarHeap heap;

  AStarKey k1;
  AStarKey k2;
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
    log_dst << "Heap reordered elements";
  if(b<a)
    log_dst << "min heap";

  log_dst << "Removing all objects";
  while(true){
    Maybe<AStarHeap::Element> e = heap.pop();
    if(e)
      print(e);
    else
      break;
  }
  log_dst << "--";
}


int main(int argc, char* argv[]) {
  init(argc, argv);

  test_heap();

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
