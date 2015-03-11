#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "inc/config.h"
#include "inc/sip.h"
#include "inc/log.h"
#include "inc/netstuff.h"

#define MAX_BUF 2048

static int init_socket(sp_config_s *);
static int udp_send_to(int, char *, int, char *);
static int udp_recv_from(int, int, char *);
static char *get_my_address(char *);

static struct sockaddr_in me_addr, them_addr;
static socklen_t addr_len = sizeof(them_addr);

int sp_udp_ping(sp_config_s config){
  int sock;
  char buf[MAX_BUF];
 
  if((sock = init_socket(&config)) == -1){
    return -1;
  }

  /* the structure is populated at this sstage */
  if(config.verbose){
    kg_log_info("Creating SIP OPTIONS\n");
  }
  char *options = sp_sip_option(config);

  if(udp_send_to(sock, config.dest, config.destport, options) == -1){
    kg_log_err("Problem sending OPTIONS to [%s]\n", config.dest);
    return -1;
  }
  if(udp_recv_from(sock, config.srcport, buf) == -1){
    kg_log_err("Response timeout\n");
    return -1;
  }
  kg_log_info("[%s] responds:\n\n[%s]\n",config.dest, buf);
  close(sock);
  free(options);
  return 0;
}

static int init_socket(sp_config_s *config){
  
  int this_sock; /* socket for this server */
  
  if((this_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
    kg_log_err("Cannot get socket.");
    return -1;
  }
  /* might want to reuse this often */
  int reuse = 1;
  if(setsockopt(this_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1){
    kg_log_err("Could not change socket options.");
    return -1;
  }
  struct timeval tv = {
    .tv_sec = 10,
    .tv_usec = 0
  };

  if(setsockopt(this_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval)) == -1){
    kg_log_err("Could not set socket timeout.");
    return -1;
  }
  
  /* destination details */
  memset(&them_addr, 0, sizeof(them_addr));
  them_addr.sin_family = AF_INET;
  them_addr.sin_port = htons(config->destport);
  
  /* source details */
  memset(&me_addr, 0, sizeof(me_addr));
  me_addr.sin_family = AF_INET;
  me_addr.sin_port = htons(config->srcport);

  /* if no iface specyfied than use any */
  if(config->iface){ 
    config->src = get_my_address(config->iface);
    inet_aton(config->src, &them_addr.sin_addr);
    if(config->verbose){
      kg_log_info("Sending from [%s] interface.\nThe IP: [%s]", config->iface, config->src);
    }
  }else{
    kg_log_err("Specify outgoing interface\n");
    return -1;
  }
  
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
  return numbytes;
}

static int udp_recv_from(int sock, int port, char *databuf){

  int numbytes;
  if((numbytes = recvfrom(sock, databuf, MAX_BUF - 1, 0, (struct sockaddr *) &me_addr, &addr_len)) == -1){
    kg_log_err("Din't get reply from remote server\n");
    return -1;
  }
  databuf[numbytes] = '\0';

  return numbytes;
}

static char *get_my_address(char * ifc){
  struct ifreq iface;

  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  
  iface.ifr_addr.sa_family = AF_INET;
  strncpy(iface.ifr_name, ifc, IFNAMSIZ-1);

  if(ioctl(sock, SIOCGIFADDR, &iface) == -1){
    kg_log_err("Cannot find IP for [%s]\n", ifc);
    return NULL;
  }
  close(sock);

  return inet_ntoa(((struct sockaddr_in *)&iface.ifr_addr)->sin_addr);
}
