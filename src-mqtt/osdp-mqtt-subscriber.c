#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


#define EQUALS ==

typedef struct osdp_subscriber
{
  int verbosity;
  int sleep_time;
} OSUB;

#define ST_OK (0)
int subscribe_to_mqtt(OSUB *ctx);


int check_disabled
  (OSUB *ctx)

{ /* check_disabled */

  return(ST_OK);

} /* check_disabled */


int main
  (int argc,
  char *argv [])

{ /* main for osdp-mqtt-subscriber */

  OSUB *ctx;
  OSUB my_osdp_subscriber_context;
  int done;
  int status;


  ctx = &my_osdp_subscriber_context;
  memset(ctx, 0, sizeof(*ctx));
  ctx->verbosity = 9;
  ctx->sleep_time = 1;

  // read settings

  done = 0;
  while (!done)
  {
    status = check_disabled(ctx);
    if (status != ST_OK)
      done = 1;
    if ((!done) && (status EQUALS ST_OK))
    {
      if (ctx->verbosity > 3)
        fprintf(stderr, "subscribing...\n");
      status = subscribe_to_mqtt(ctx);
      if (status != ST_OK)
        done = 1;
    };
  };
  return(status);

} /* main for osdp-mqtt-receiver */

int subscribe_to_mqtt
  (OSUB *ctx)

{ /* subscribe_to_mqtt */

  char command [1024];


  strcpy(command,
    "rm -f /tmp/queued-command.json");
  system(command);
  strcpy(command,
    "mosquitto_sub --port 8468 -t \"osdp\" -h localhost -C 1 >/tmp/queued-command.json");
  system(command);
  if (ctx->sleep_time > 0)
    sleep(ctx->sleep_time);
  strcpy(command, 
"/opt/osdp-conformance/bin/open-osdp-kick ACU </tmp/queued-command.json");
  system(command);
  return(ST_OK);
}

