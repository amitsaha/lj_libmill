#define MAX_MSG_SIZE 100

# include <stdio.h>
# include <libmill.h>

/* Generate work */
int work_idx = 0;
coroutine void generator(chan ch) {
  while(1) {
    char s[MAX_MSG_SIZE];
    snprintf(s, MAX_MSG_SIZE, "Work %d", work_idx++);
    chs(ch, char*, s);
  }
}

/* Process work */
coroutine void worker(int index, chan ch) {
  while(1)
    printf("Worker %d, Processing: %s\n", index, chr(ch, char*));
}

int main(int argc, char **argv)
{
  /* Create an unbuffered channel to recieve work*/
  chan ch1 = chmake(char*, 0);
  /* Create an unbuffered channel to send work on*/
  chan ch2 = chmake(char*, 0);

  /* Start the work generating coroutine */
  go(generator(ch1));

  /* Create a pool of worker coroutines */
  for(int i=1;i<=5;i++) {
    go(worker(i, ch2));
  }

  /* Receive work from generator and send to worker pool*/
  while(1) {
    chs(ch2, char*, chr(ch1, char*));
  }

  /* Close channels */
  chclose(ch1);
  chclose(ch2);
  return 0;
}
