# include <stdio.h>
# include <libmill.h>
# include <string.h>
# include <stdlib.h>

coroutine void f(int index)
{
  msleep(now() + rand() % 50 );
  printf("Worker %d\n", index);  
}

int main(int argc, char **argv)
{
  for(int i=1;i<=10; i++) {
    go(f(i));
  }
  msleep(now() + 60);
  return 0;
}
