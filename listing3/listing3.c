# include <libmill.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <unistd.h>

/* Handler coroutine */
coroutine void handler(tcpsock as) {
  printf("New connection!\n");
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
  } else {
    printf("Server listening on %d\n", port);
  }

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
