#include <stdio.h>
#include <stdlib.h>

#define F2(x) F1(x)
#define F stdout
#define F1(x) #x

int main() {
  int fd = fileno(F);
  printf(F2(F)"'s file descriptor is %d \n", fd);
  return 0;
}
