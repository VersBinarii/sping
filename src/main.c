#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "inc/log.h"

#define SIP_PORT "5060"
          
int main(int argc, char **argv){
  int this_sock;
  int status;
  struct addrinfo hints, *hinfo;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if((status = getaddrinfo(argv[1], SIP_PORT, &hints, &hinfo)) != 0){
    kg_log_info("getaddrinfo failed.");
    exit(EXIT_FAILURE);
  }

  if((this_sock = socket(hinfo->ai_family, hinfo->ai_socktype, hinfo->ai_protocol)) == -1){
    kg_log_info("Cannot get socket.");
  }
  /* might want to reuse this often */
  int reuse = 1;
  if(setsockopt(this_sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int)) == -1){
    kg_log_info("Could not change socket options.");
    exit(EXIT_FAILURE);
  }

  /* need to bind cause will want to wait for response */
  bind(this_sock, hinfo->ai_addr, hinfo->ai_addrlen);
  


  return 0;
}
