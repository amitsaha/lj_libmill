/* Demo of a worker pool using libmill (Go: https://gobyexample.com/worker-pools) */

# include <stdio.h>
# include <libmill.h>

coroutine void worker(int index, chan ch)
{
  while(1)
    printf("Worker %d, Message in channel: %s\n", index, chr(ch, char*));
}

int main(int argc, char **argv)
{
  char str[10];
  /* Create an unbuffered channel */
  chan ch = chmake(char*, 0);

  for(int i=0;i<=5;i++) {
    go(worker(i, ch));
  }
  while(1) {
    /* Send some data */
    chs(ch, char*, "Hello");
  }

  /* Close channel */
  chclose(ch);
  return 0;
}
