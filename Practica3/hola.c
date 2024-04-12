#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void hijo (const char *num_hermanos);
int main (int argc, char *argv[]) {
hijo(argv[1]);
return 0;
}
void hijo (const char *num_hermanos) {
printf("Soy %ld y tengo %d hermanos!\n",
(long)getpid(), atoi(num_hermanos) - 1);
}