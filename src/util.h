#ifndef UTIL_H
#define	UTIL_H

#define TIMEVAL_SET(tv, t)                     \
        do {                                             \
            tv.tv_sec  = (long) t;                       \
            tv.tv_usec = (long) ((t - tv.tv_sec) * 1e6); \
        } while (0)

#define TIMEVAL_TO_DOUBLE(tv) (tv.tv_sec + tv.tv_usec * 1e-6)

#endif	/* UTIL_H */

