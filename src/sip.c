#include <stdlib.h>

#include "inc/log.h"
#include "inc/config.h"
#include "inc/sip.h"
#include "inc/mysprintf.h"

static char *OPTION = 

"OPTIONS sip:%s:%d;transport=udp SIP/2.0\n\
Call-ID: 00-01991-0011623a-35292d8d1@%s\n\
Contact: <sip:%s:%d;transport=udp>\n\
CSeq: 63104 OPTIONS\n\
From: SPING<sip:sping@%s:%d>;tag=00-01991-00116239-25a473e47\n\
Max-Forwards: 70\n\
To: <sip:%s@%s:%d>\n\
Via: SIP/2.0/UDP %s:%d;rport;branch=z9hG4bKhjhs8ass877\n\
Accept: application/sdp\n\
Content-Length: 0\n";

/*
OPTIONS sip:172.25.6.10:5060;transport=udp SIP/2.0
Call-ID: 00-01991-0011623a-35292d8d1@178.250.112.75
Contact: <sip:178.250.112.75:5060;transport=udp>
CSeq: 99200 OPTIONS
From: <sip:keepalive@178.250.112.75:5060>;tag=00-01991-00116239-25a473e47
Max-Forwards: 70
To: <sip:172.25.6.10:5060>
Via: SIP/2.0/UDP 178.250.112.75:5060;rport;branch=z9hG4bK-ZZSM-00440142-23025a2b
Content-Length: 0
*/

char *sp_sip_option(sp_config_s cfg){
  char *opt;
  mysprintf(&opt, OPTION, 
	   cfg.dest, cfg.destport,
	   cfg.src, cfg.src, cfg.srcport,
	   cfg.src, cfg.srcport,
	   cfg.number, cfg.dest, cfg.destport,
	   cfg.src, cfg.srcport);
  
  return opt;  
}
