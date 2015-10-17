/**
 * @file:           cutils.h
 * @description:    Separated utils (for C targets)
 */

#ifndef CUTILS_H
#define CUTILS_H

#ifdef RCPP_INTERFACE
// FIXME?
#define C_EXIT(x)
#define C_FFLUSH(stream)
#define C_FPRINTF(stream, ...)
#define C_PRINTF(...)
#else
#define C_EXIT(x) exit(x);
#define C_FFLUSH(stream) fflush(stream);
#define C_FPRINTF(stream, ...) fprintf(stream, __VA_ARGS__);
#define C_PRINTF(...) printf(__VA_ARGS__);
#endif

#endif
