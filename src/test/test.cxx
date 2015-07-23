#define __DPL_TEST__ "DPL Planning Library Tests"
//#define :w "I meant to save the file."

// Includes
// ========
// DPL
#include <DPL/utils/types.hxx>
#include <DPL/utils/os.hxx>


using std::cout;
using namespace DPL;




int main() {
  cout << "Starting on " << target << " Mode" << endl;
  cout << "Compiled with " << compiler << endl;
  cout << endl;

  // TODO: Add some tests


  log_tst << "All tests passed";
  return 0;
}
