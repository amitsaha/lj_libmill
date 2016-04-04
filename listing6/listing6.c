# include <libmill.h>
# include <stdio.h>
# include <stdlib.h>
# include <time.h>

struct data {
  char *url;
  char *data;
};

coroutine void f(chan work, chan result, chan error) {
  struct data d;
  char *url = chr(work, char*);
  d.url = url;
  /* Simulate 70% success and 30% failure scenario*/
  double r = (double) rand()/RAND_MAX;
  if (r < 0.71) {
    // Successful scenario
    d.data = "Data at the URL";
    chs(result, struct data, d);
  } else {
    // Unsuccessful scenario
    d.data = "Error retreiving data";
    chs(error, struct data, d);
  }
}

int main(int argc, char **argv) {
  chan work = chmake(char*, 0);
  chan result = chmake(struct data, 0);
  chan error = chmake(struct data, 0);

  for(int i=1; i<argc; i++) {
    go(f(work, result, error));
  }

  for(int i=1; i<argc; i++) {
    choose {
    in(result, struct data, value):
      printf("Processed URL: %s, Result: %s\n", value.url, value.data);
    in(error, struct data, value):
      printf("Processed URL: %s, Error: %s\n", value.url, value.data);
    end
    }
  }
  chclose(work);
  chclose(result);
  chclose(error);
  return 0;
}
