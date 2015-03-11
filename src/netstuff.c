#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "inc/netstuff.h"
#include "inc/log.h"

#define MAX_BUF 1024

static int init_socket(char *, int);
static int udp_send_to(int, char *, int, char *);
static int udp_recv_from(int, int, char *);

static struct sockaddr_in me_addr, them_addr;
static socklen_t addr_len = sizeof(them_addr);

int sp_udp_ping(char *ip, int port){
  int sock;
  char buf[MAX_BUF];
  char *options = strdup("sip options...plenty options...");

  if((sock = init_socket(ip, port)) == -1){
    return -1;
  }

  if(udp_send_to(sock, ip, port, options) == -1){
    kg_log_err("Problem sending OPTIONS to [%s]\n", ip);
    return -1;
  }
  if(udp_recv_from(sock, port, buf) == -1){
    kg_log_err("Problem getting response\n");
    return -1;
  }
  close(sock);
  return 0;
}

static int init_socket(char *ip, int port){

  int this_sock; /* socket for this server */

  if((this_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
    kg_log_err("Cannot get socket.");
    return -1;
  }
  /* might want to reuse this often */
  int reuse = 1;
  if(setsockopt(this_sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int)) == -1){
    kg_log_err("Could not change socket options.");
    return -1;
  }
  
  /* destination details */
  memset(&them_addr, 0, sizeof(them_addr));
  them_addr.sin_family = AF_INET;
  them_addr.sin_port = htons(port);

  /* source details */
  memset(&me_addr, 0, sizeof(me_addr));
  me_addr.sin_family = AF_INET;
  me_addr.sin_port = htons(port);
  me_addr.sin_addr.s_addr= htonl(INADDR_ANY);

  bind(this_sock, (struct sockaddr *)&me_addr, sizeof(me_addr));
  
  return this_sock;
}

static int udp_send_to(int sock, char *ip, int port, char *sip_pack){

  if(inet_aton(ip, &them_addr.sin_addr) == 0){
    kg_log_err("inet_aton() failed\n");
    return -1;
  }
  
  int numbytes; /* number of bytes sent */
  if((numbytes = sendto(sock, sip_pack, strlen(sip_pack), 0, (struct sockaddr *)&them_addr, addr_len)) == -1) {
    kg_log_err("Problem with sending data");
    return -1;
  }
  kg_log_info("Sending [%d] bytes to [%s]\n",numbytes, inet_ntoa(them_addr.sin_addr));
  return numbytes;
}

static int udp_recv_from(int sock, int port, char *databuf){

  kg_log_info("recv_from() - \nport = [%d]\n", port);

  int numbytes;
  if((numbytes = recvfrom(sock, databuf, MAX_BUF - 1, 0, (struct sockaddr *) &me_addr, &addr_len)) == -1){
    kg_log_err("Din't get reply from remote server\n");
    return -1;
  }
  databuf[numbytes] = '\0';
  kg_log_info("Received [%s]\n", databuf);
  
  return numbytes;
}
