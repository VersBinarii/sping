#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "inc/config.h"
#include "inc/log.h"
#include "inc/netstuff.h"

#define PROGNAME "sping"
#define VERSION "0.0.2"

static void show_usage(int status);

sp_config_s sp_config = {
  .src = NULL,
  .destport = 5060,
  .srcport = 5060,
  .type = UDP,
  .verbose = 0
};

int main(int argc, char **argv){
  
  if(argc < 2){
    //show usage
    kg_log_err("Not enough arguments. Specify at least destination\n");
    show_usage(EXIT_FAILURE);
  }
  
  sp_config.dest = argv[1];

  int argval;
  while((argval = getopt(argc, argv, "I:p:tP:vh")) != -1){
    switch(argval){
    case 'I':
      //source address
      sp_config.src = optarg;
      break;

    case 'p':
      //dest port umber
      sp_config.destport =atoi(optarg);
      break;

    case 'P':
      //source port
      sp_config.srcport = atoi(optarg);
      break;
      
    case 't':
      //use TCP
      sp_config.type = TCP;
      break;
      
    case 'v':
      //verbose
      sp_config.verbose = 1;
      break;

    case 'h':
      //display help
      show_usage(EXIT_SUCCESS);
      break;

    default:
      show_usage(EXIT_FAILURE);
    }
  }
  
  kg_log_info("Sending UPD ping to [%s]\n", sp_config.dest);
  sp_udp_ping(sp_config);
  return 0;
}

static void show_usage(int status){

  FILE *output = status?stderr:stdout;
  fprintf(output, "\n");
  fprintf(output, "%s - SIPping - SIP troubleshooting tool\n", PROGNAME);
  fprintf(output, "Version: %s\n", VERSION);
  fprintf(output, "\nUsage: %s <host> [options]\n", PROGNAME);
  fprintf(output, "Options:\n");
  fprintf(output, "\t\"-p <port>\" - destination port\n");
  fprintf(output, "\t\"-P <port>\" - source port\n");
  fprintf(output, "\t\"-I <ip>\"   - specify source IP (not implemented yet)\n");
  fprintf(output, "\t\"-t\"        - use TCP (not implemented yet)\n");
  fprintf(output, "\t\"-v\"        - verbose output.\n");
  fprintf(output, "\t\"-h\"        - display this help menu.\n");
  fprintf(output, "\n");
  exit(status);
}
