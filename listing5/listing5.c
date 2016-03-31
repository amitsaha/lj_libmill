# include <stdio.h>
# include <libmill.h>
# include <string.h>

coroutine void f(int index, const char *text)
{
  char a[256];
  printf("%p\n", a);
  strncpy(a, text, sizeof(text));
  printf("Worker %d, Message %s\n", index, a);
}


int main(int argc, char **argv)
{
  char str[10];
  gotrace(1);
  for(int i=1;i<=10; i++) {
    sprintf(str, "Text %d", i);
    go(f(i, str));
  }
  return 0;
}
