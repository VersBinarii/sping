#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


int asprintf(char **str, char *fmt, ...){
  va_list args;
  va_start(args, fmt);
  char dummy[1];

  int len = vsnprintf(dummy, 1, fmt, args);
  if(len < 1){
    *str = NULL;
    return len;
  }
  va_end(args);

  *str = malloc(len + 1);
  if(!str){
    return -1;
  }

  va_start(args, fmt);
  vsnprintf(*str, (len + 1), fmt, args);
  va_end(args);
  return len;
}
