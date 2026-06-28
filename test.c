#include "test.h"
int main() {
  int x = 42;
  char *test = "Test";
  test[2] = 'e';
  printf("%s", test);
  return
#ifdef XYZ
      0
#else
      1
#endif
      ;
}
