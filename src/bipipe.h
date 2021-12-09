#ifndef SAM_BELLIVEAU_PIPE_HEADER
#define SAM_BELLIVEAU_PIPE_HEADER 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <errno.h>

#define PIPE_READ 0
#define PIPE_WRITE 1

struct bipipe
{
    int out;
    int in;
};

typedef void (*child_function)(struct bipipe);

void bipipe_create(struct bipipe* parent, struct bipipe *child);

struct bipipe bipipe_fork(child_function child_fn);

#endif