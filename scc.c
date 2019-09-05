// this file is used for tutorial to build the compiler step by step


#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "svm.h"
#define int long long // work with 64bit target

int token;            // current token
char *src, *old_src;  // pointer to source code string;
int poolsize;         // default size of text/data/stack
int line;             // line number
int *text,            // text segment
    *old_text,        // for dump text segment
    *stack;           // stack
char *data;           // data segment

// instructions

void next() {
    token = *src++;
    return;
}

void expression(int level) {
    // do nothing
}

void program() {
    next(); // get next token
    while (token > 0) {
        printf("token is: %c", (char)token);
        next();
    }
}
#undef int // Mac/clang needs this

int main(int argc, char **argv)
{
    #define int long long // work with 64bit target

    int i, fd, *exe, *offset;

    argc--;
    argv++;

    poolsize = 32 * 1024; // arbitrary size
    line = 1;

    if ((fd = open(*argv, 0)) < 0) {
        printf("could not open(%s)\n", *argv);
        return -1;
    }

    if (!(src = old_src = malloc(poolsize))) {
        printf("could not malloc(%lld) for source area\n", poolsize);
        return -1;
    }
    // read the source file
    if ((i = read(fd, src, poolsize-1)) <= 0) {
        printf("read() returned %lld\n", i);
        return -1;
    }
    src[i] = 0; // add EOF character
    close(fd);

    // allocate memory for virtual machine
	offset = exe = init_vm(poolsize);
	if(exe == NULL) {
        printf("could not malloc \n");
        return -1;
	}
	text = (int*)((char*)exe + offset[0]);
	data = (char*)exe + offset[1];
	stack = (int*)((char*)exe + offset[2]);

    program();

    return eval(exe);
    return 0;
}
