#include <stdio.h>
#include <stdarg.h>
#include "inc/log.h"

static void kg_log(FILE *f, char const *fmt, va_list args){
  vfprintf(f, fmt, args);
}

void kg_log_err(char const *fmt, ...){
  va_list args;
  va_start(args, fmt);
  kg_log(stderr, fmt, args);
  va_end(args);
}

void kg_log_info(char const *fmt, ...){
  va_list args;
  va_start(args, fmt);
  kg_log(stdout, fmt, args);
  va_end(args);
}
