#ifndef _CONFIG_H_
#define _CONFIG_H_

typedef enum proto_type{
  UDP = 0,
  TCP
}proto_type;

typedef struct sp_config_s{
  char *iface;
  char *src;
  char *dest;
  int destport;
  int srcport;
  char *number; /* optional phone number */
  proto_type type; /* tcp or udp */
  int verbose;

}sp_config_s;          


#endif
