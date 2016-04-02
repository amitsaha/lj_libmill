# include <stdio.h>
# include <libmill.h>
# include <string.h>
# include <stdlib.h>

coroutine void worker(chan input, chan output) {
  /* Receive work*/
  char *work = chr(input, char*);
  printf("Processing: %s\n", work);
  /* Send the result back by writing to the channel*/
  chs(output, int, strlen(work));
}

int main(int argc, char **argv)
{
  if (argc != 2) {
    printf("Please specify one command line argument\n");
    exit(1);
  }
   /* Create an unbuffered channel to send work on*/
  chan input = chmake(char*, 0);
  if (input == NULL) {
    printf("Failed to create channels for input\n");
    exit(1);
  }

  /* Create an unbuffered channel to recieve result on*/
  chan output = chmake(int, 0);
  if (output == NULL) {
    printf("Failed to create channels for output\n");
    exit(1);
  }

  /* Create a worker coroutine */
  go(worker(input, output));
  /* Send data to worker by writing to input channel*/
  chs(input, char*, argv[1]);
  /* Recieve result from the coroutine and output*/
  printf("Result: %d\n", chr(output, int));

  /* Close channels */
  chclose(input);
  chclose(output);
  return 0;
}






