#ifndef _NCLOUD_TIMER_H_
#define _NCLOUD_TIMER_H_

#ifdef __linux__
#include <sys/time.h>

/* The argument now should be a double (not a pointer to a double) */
#define GET_TIME(now) {                  \
   struct timeval t;                     \
   gettimeofday(&t, NULL);               \
   now = t.tv_sec + t.tv_usec/1000000.0; \
}

#elif _WIN32
#include <time.h>

#define GET_TIME(now) {     \
   clock_t t = clock();     \
   now = (double)t/CLK_TCK; \
}
#endif

#endif
