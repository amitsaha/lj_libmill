### Example coroutine based HTTP server using libmill

This is an example couroutine based HTTP server implemented using libmill.
It implements a simple HTTP interface to generating hashids using 
[hashids.c](https://github.com/tzvetkoff/hashids.c). The hashids.h and
hashids.c files are from the hashids.c repository.

## Compiling and running it
$ gcc -o hashid_service hashids.c hashid_service.c -lmill
$ ./hashid_service

## Using it

$ curl http://127.0.0.1:5555/1
..
