/* Coroutine based HTTP server implements hashid as service (http://hashids.org/c/) */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libmill.h>
#include <string.h>
#include "hashids.h"

size_t build_http_response(int status_code,
                           char *message,
                           char *response_buffer,
                           size_t response_size) {
  char *http_response_template =
    "HTTP/1.1 %d OK\r\n\r\n"
    "%s\n";
  memset(response_buffer, 0, response_size);
  return snprintf(response_buffer, response_size, http_response_template, status_code, message);
}

coroutine void serve(tcpsock as) {

  int64_t deadline = now() + 10000;

  /* Request and response */
  char http_method[5];
  char inbuf[256];
  char outbuf[512];
  char response[512];
  int resp_nbytes;
  int resource_to_fetch_pos;

  /* hashids.c */
  struct hashids_t *hashids;
  unsigned int bytes_encoded;
  char hash[512];
  unsigned long long integer_to_encode=0;
  hashids = hashids_init3("this is my salt", 6, HASHIDS_DEFAULT_ALPHABET);

  /* Recieve data until either the buffer is full or \r is encountered */
  size_t sz = tcprecvuntil(as, inbuf, sizeof(inbuf), "\r", 1, deadline);
  if(errno != 0)
    goto cleanup;
  inbuf[sz - 1] = 0;
  printf("Serving request %s\n", inbuf);

  /* Find if we support the HTTP method requested*/
  int pos;
  for(pos=0; inbuf[pos] != ' '; pos++) {
    http_method[pos] = inbuf[pos];
  }
  http_method[pos] = 0;

  if (strcmp(http_method, "GET") != 0) {

    char *message = "Only GET supported\r\n";
    build_http_response(400, message, response, sizeof(response));
    resp_nbytes = snprintf(outbuf, sizeof(outbuf), "%s", response);

  } else {

    /* Get the ID to generate a hashID for*/
    char resource[10]; // TODO: set this to the maximum allowed integer size
    int index=0;
    resource_to_fetch_pos = pos+2;
    for(pos=resource_to_fetch_pos; pos < 10 && inbuf[pos] != ' '; pos++) { // TODO: update loop as above
      resource[index++] = inbuf[pos];
    }
    if (index == 0) {
      char *message = "I can only generate a hashid if an integer is supplied\r\n";
      build_http_response(400, message, response, sizeof(response));
      resp_nbytes = snprintf(outbuf, sizeof(outbuf), "%s", response);
    } else {
      char *endptr;
      integer_to_encode = strtoull(resource, &endptr, 0);
      if (*endptr != '\0') {
        char *message = "I can only generate hashids for integers\r\n";
        build_http_response(400, message, response, sizeof(response));
        resp_nbytes = snprintf(outbuf, sizeof(outbuf), "%s", response);
        /* reset endptr*/
        *endptr = '\0';
      } else {
        bytes_encoded = hashids_encode_one(hashids, hash, integer_to_encode);
        build_http_response(200, hash, response, sizeof(response));
        resp_nbytes = snprintf(outbuf, sizeof(outbuf), "%s", response);
      }
    }
  }

  sz = tcpsend(as, outbuf, resp_nbytes, deadline);
  if(errno != 0)
    goto cleanup;
  tcpflush(as, deadline);

  if(errno != 0)
    goto cleanup;

 cleanup:
  strerror(errno);

  tcpclose(as);

  return;
}

int main(int argc, char *argv[]) {

  int port = 5555;
  ipaddr addr = iplocal(NULL, port, 0);
  tcpsock ls = tcplisten(addr, 10);
  if(!ls) {
    perror("Can't open listening socket");
    return 1;
  }
  printf("Listening for HTTP connections on port %d\n", port);
  while(1) {
    tcpsock as = tcpaccept(ls, -1);
    if(!as)
      continue;
    go(serve(as));
  }

  return 0;
}
