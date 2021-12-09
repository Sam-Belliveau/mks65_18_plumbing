#include "bipipe.h"

void bipipe_create(struct bipipe* parent, struct bipipe *child)
{
    int fd_parent_to_child[2];
    int fd_child_to_parent[2];

    if(pipe(fd_parent_to_child) < 0)
    {
        fprintf(stderr, "error: unable to open pipe: %s [%d]", strerror(errno), errno);
        exit(-1);
    }

    if(pipe(fd_child_to_parent) < 0)
    {
        fprintf(stderr, "error: unable to open pipe: %s [%d]", strerror(errno), errno);
        exit(-1);
    }

    parent->out = fd_parent_to_child[PIPE_WRITE];
    child->in = fd_parent_to_child[PIPE_READ];
    
    child->out = fd_child_to_parent[PIPE_WRITE];
    parent->in = fd_child_to_parent[PIPE_READ];
}

struct bipipe bipipe_fork(child_function child_fn)
{
    int f;
    struct bipipe parent, child;

    bipipe_create(&parent, &child);

    f = fork();
    
    // Child
    if(f != 0)
    {
        close(parent.out);
        close(parent.in);
     
        child_fn(child);

        close(child.out);
        close(child.in);
        exit(0);
    }

    // Parent
    else
    {
        close(child.out);
        close(child.in);

        return parent;
    }
}