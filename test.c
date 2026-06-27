#include "test.h"
int main() {
  return
#ifdef XYZ
      0
#else
      1
#endif
      ;
}
