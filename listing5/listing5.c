# include <libmill.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <unistd.h>

# define NUM_PROCESSES 3

/* Handler coroutine */
coroutine void handler(tcpsock as) {
  printf("PID: %d, New connection!\n", getpid());
  tcpclose(as);
}

int main(int argc, char **argv)
{
  int port = 9090;
  ipaddr addr = iplocal(NULL, port, 0);
  tcpsock server = tcplisten(addr, 10);

  if (!server) {
    perror("Can't setup a listening server\n");
    return 1;
  }
  /* Set up the server processes - the main process is listening as well, so
     we fork NUM_PROCESSES-1 child processes
  */
  for (int i = 1; i < NUM_PROCESSES; i++ ) {
    pid_t pid = mfork();
    /* Child process?*/
    if (pid == 0)
      break;
  }

  printf("Listening on %d (PID: %d)\n", port, getpid());
  /* Server loop*/
  while(1) {
    tcpsock as = tcpaccept(server, -1);
    if (!as)
      continue;
    /* Dispatch this request */
    go(handler(as));
  }

return 0;
}
