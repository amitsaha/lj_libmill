# include <stdio.h>
# include <libmill.h>

coroutine void f(int index)
{
  printf("Worker %d\n", index);
}

int main(int argc, char **argv)
{
  for(int i=1;i<=10; i++) {
    go(f(i));
  }
  return 0;
}
