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
#define C_FPRINTF(stream, string, ...)
#define C_PRINTF(string, ...)
#else
#define C_EXIT(x) exit(x);
#define C_FFLUSH(stream) fflush(stream);
#define C_FPRINTF(stream, string, ...) fprintf(stream, string, __VA_ARGS__);
#define C_PRINTF(string, ...) printf(string, __VA_ARGS__);
#endif

#endif
