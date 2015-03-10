#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "inc/log.h"

#define MAX_BUF 1024
#define SIP_PORT 5060 /* standard but must be able to change */
          
int main(int argc, char **argv){

  int this_sock; /* socket for this server */
  struct sockaddr_in them_addr, me_addr;
  char *options = strdup("sip options...plenty options...");
  char buf[MAX_BUF];
  
  if((this_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
    kg_log_err("Cannot get socket.");
  }
  /* might want to reuse this often */
  int reuse = 1;
  if(setsockopt(this_sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int)) == -1){
    kg_log_err("Could not change socket options.");
    exit(EXIT_FAILURE);
  }
  
  /* destination details */
  memset(&them_addr, 0, sizeof(them_addr));
  them_addr.sin_family = AF_INET;
  them_addr.sin_port = htons(SIP_PORT);
  socklen_t addr_len = sizeof(them_addr);

  /* source details */
  memset(&me_addr, 0, sizeof(me_addr));
  me_addr.sin_family = AF_INET;
  me_addr.sin_port = htons(SIP_PORT);
  me_addr.sin_addr.s_addr= htonl(INADDR_ANY);
  bind(this_sock, (struct sockaddr *)&me_addr, sizeof(me_addr));


  if(inet_aton(argv[1], &them_addr.sin_addr) == 0){
    kg_log_err("inet_aton() failed\n");
    exit(EXIT_FAILURE);
  }
  
  int numbytes; /* number of bytes sent */
  if((numbytes = sendto(this_sock, options, strlen(options), 0, (struct sockaddr *)&them_addr, addr_len)) == -1) {
    kg_log_err("Problem with sending data");
    exit(EXIT_FAILURE);
  }
  kg_log_info("Sending [%d] bytes to [%s]\n",numbytes, inet_ntoa(them_addr.sin_addr));
  
  if((numbytes = recvfrom(this_sock, buf, MAX_BUF - 1, 0, (struct sockaddr *) &me_addr, &addr_len)) == -1){
    kg_log_err("Din't get reply from remote server\n");
    exit(EXIT_FAILURE);
  }
  buf[numbytes] = '\0';
  kg_log_info("Received [%s]\n", buf);
  
  return 0;
}
