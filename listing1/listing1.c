# include <stdio.h>
# include <libmill.h>
# include <string.h>

coroutine void f(int index, const char *text)
{
  printf("Worker %d, Message %s\n", index, text);
}

int main(int argc, char **argv)
{
  char str[10];
  for(int i=1;i<=10; i++) {
    sprintf(str, "Text %d", i);
    go(f(i, str));
  }
  return 0;
}
