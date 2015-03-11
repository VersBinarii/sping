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
#define VERSION "0.0.5"

int timeout = 9999999;

static void show_usage(int status);

sp_config_s sp_config = {
  .iface = NULL,
  .src = NULL,
  .destport = 5060,
  .srcport = 5060,
  .number = "anonymous",
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
  while((argval = getopt(argc, argv, "i:I:p:tP:N:vh")) != -1){
    switch(argval){
    
    case 'i':
      //outgoing iface
      sp_config.iface = optarg;
      break;
     
    case 'p':
      //dest port umber
      sp_config.destport =atoi(optarg);
      break;

    case 'P':
      //source port
      sp_config.srcport = atoi(optarg);
      break;
      
    case 'N':
      // specify phone number
      sp_config.number = optarg;
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
  
  if(sp_config.verbose){
    kg_log_info("Sending UPD ping to [%s]\n", sp_config.dest);
  }
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
  fprintf(output, "\t\"-p <port>\"           - destination port\n");
  fprintf(output, "\t\"-P <port>\"           - source port\n");
  fprintf(output, "\t\"-i <if>\"             - specify outgoing iface\n");
  fprintf(output, "\t\"-N <phone_number>\"   - specify DDI number\n");
  fprintf(output, "\t\"-t\"                  - use TCP (not implemented yet)\n");
  fprintf(output, "\t\"-v\"                  - verbose output.\n");
  fprintf(output, "\t\"-h\"                  - display this help menu.\n");
  fprintf(output, "\n");
  exit(status);
}
