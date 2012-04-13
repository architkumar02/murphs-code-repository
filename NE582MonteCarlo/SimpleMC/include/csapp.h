/* $begin csapp.h */
#ifndef _csapp_h
#define _csapp_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <errno.h>
#include <math.h>

// Dumbed down 15-213 general header

/* Our own error-handling functions */
void unix_error(char *msg);
void app_error(char *msg);

/* Standard I/O wrappers */
void Fclose(FILE *fp);
FILE *Fdopen(int fd, const char *type);
char *Fgets(char *ptr, int n, FILE *stream);
FILE *Fopen(const char *filename, const char *mode);
void Fputs(const char *ptr, FILE *stream);
size_t Fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
void Fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

/* Dynamic storage allocation wrappers */
void *Malloc(size_t size);
void *Realloc(void *ptr, size_t size);
void *Calloc(size_t nmemb, size_t size);
void Free(void *ptr);

#endif /* __CSAPP_H__ */