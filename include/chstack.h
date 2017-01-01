#ifndef CHSTACK_H
#define CHSTACK_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>


#define BUILDMAPSPATH(x, len)   snprintf(x, len, "/proc/%d/maps", getpid());
          
#define P_ERR(msg, err, ret)    fprintf(stderr, "%s\t%s", msg, err); \
								return ret;

long int getStackBase(long int * startStack);
int swapRegisters(void * newStackAddr, const long int stackBaseAddr, const long int len);
void * setupNewStack(const long int len);
int chgStack(void);
#endif

