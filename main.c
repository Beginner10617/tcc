#include "preprocessor.h"
#include "stdio.h"
#include "stdlib.h"
int main(int argc, char *argv[]) {
  char *filepath = argv[1];
  char *src = read_file(filepath);
  Buffer final_src = preprocess(src, argv[1], false, NULL);
  printf("%s", final_src.data);
  return EXIT_SUCCESS;
}
