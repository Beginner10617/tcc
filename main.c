#include "preprocessor.h"
#include "stdio.h"
#include "stdlib.h"
int main(int argc, char *argv[]) {
  char *filepath = argv[1];
  char *src = read_file(filepath);
  Buffer final_src = preprocess(src, true);
  return EXIT_SUCCESS;
}
