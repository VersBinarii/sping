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
#include "inc/netstuff.h"


#define SIP_PORT 5060 /* standard but must be able to change */
          
int main(int argc, char **argv){
  
  sp_udp_ping(argv[1], SIP_PORT);
  return 0;
}
