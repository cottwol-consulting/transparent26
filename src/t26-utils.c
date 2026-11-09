/*
  t26-utils - utility rountines for transparent mode action routines

  Copyright 2026 Cottwol Consulting

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>


#include <jansson.h>


#include <transparent26.h>


int t26_command_osdp
  (TEST_CONTEXT *ctx,
  char *command)

{ /* t26_command */

  int status;


  status = ST_OK;
  strcpy(ctx->last_command_sent, command);
  {
    FILE *cf;
if (ctx->verbosity > 3)
  fprintf(ctx->log, "Sending command %s\n", command);
    cf = fopen("/tmp/cmd.json", "w");
    fprintf(cf, "%s", command);
    fflush(cf);
    fclose(cf);
    system(
"mosquitto_pub --port 8468 -t \"osdp\" -h localhost --file /tmp/cmd.json");
  };
#ifdef DIRECT_SOCKET
  struct sockaddr_un addr;
  int fd;
  char socket_path [1024];
  int status_io;

  if (ctx->verbosity > 3)
    fprintf(ctx->log, "OSDP Command issued: %s\n", command);
      
  sprintf(socket_path, "/opt/osdp-conformance/run/ACU/open-osdp-control");
  fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd EQUALS -1)
    status = STT26_OSDP_SOCKET;
  if (status EQUALS ST_OK)
  {
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    status_io = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if (status_io EQUALS -1)
    {
      status = STT26_OSDP_CONNECT;
    };
  };
  if (status EQUALS ST_OK)
  {
    status_io = write(fd, command, 1+strlen(command)); // write the null
    if (status_io EQUALS -1)
      status = STT26_OSDP_SOCK_WRITE;
  };
#endif

  return(status);

} /* t26_command */


char *t26_dump_hex
  (TEST_CONTEXT *ctx,
  char *prefix,
  unsigned char *raw_buffer,
  int raw_lth,
  char *suffix)

{
  int i;
  char octet_s [1024];
  static char results [1024];


  results [0] = 0;
  strcpy(results, prefix);
  for (i=0; i<raw_lth; i++)
  {
    sprintf(octet_s, " %02X", raw_buffer [i]);
    strcat(results, octet_s);
  };
  strcat(results, suffix);
  return(results);
}


// reads from the settings file

int t26_init_settings
  (TEST_CONTEXT *ctx)

{ /* t26_init_settings */

  int i;
  long ilong;
  json_t *root;
  int status;
  json_error_t status_json;
  json_t *value;


  status = ST_OK;
  root = json_load_file(ctx->settings_filename, 0, &status_json);
  if (root)
  {
    value = json_object_get (root, "verbosity");
    if (json_is_string (value))
    {
      sscanf(json_string_value(value), "%d", &i);
      ctx->verbosity = i;
    };
    value = json_object_get (root, "card-format");
    if (json_is_string (value))
    {
      sscanf(json_string_value(value), "%d", &i);
      ctx->card_format = i;
    };
    value = json_object_get (root, "delay-nsec");
    if (json_is_string (value))
    {
      sscanf(json_string_value(value), "%ld", &ilong);
      ctx->delay_nsec = ilong;
    };
    value = json_object_get (root, "delay-sec");
    if (json_is_string (value))
    {
      sscanf(json_string_value(value), "%d", &i);
      ctx->delay_sec = i;
    };
    value = json_object_get (root, "response");
    if (json_is_string (value))
    {
      sscanf(json_string_value(value), "%d", &i);
      ctx->response = i;
    };
  };
  return(status);

} /* t26_init_settings */


// initializes the context file on disk

int t26_initialize_context
  (TEST_CONTEXT *ctx)

{ /* t26_initialize_context */

  FILE *contextf;
  int status;


  status = ST_OK;
  contextf = fopen(T26_CONTEXT_FILE, "w");
  fprintf(contextf,
"{\"operating-mode\":\"%d\",\"card-format\":\"unknown\"}",
    T26_OPMODE_NORMAL);
  fclose(contextf);
  return(status);
} /* t26_initialize_context */


int t26_leave_transparent_mode
  (TEST_CONTEXT *ctx)
{
  fprintf(ctx->log, "***LEAVE TRANSPARENT MODE NOW***\n");
  return(ST_OK);
}


// this loads the in-memory context from the record file

int t26_restore_context
  (TEST_CONTEXT *ctx)

{ /* t26_restore_context */

  int i;
  json_t *root;
  int status;
  json_error_t status_json;
  json_t *value;


  status = ST_OK;
  root = json_load_file(ctx->context_filename, 0, &status_json);
  if (!root)
    status = STT26_NO_CONTEXT;
  if (root)
  {
    value = json_object_get (root, "DISABLED");
    if (json_is_string (value))
      status = -1;
    value = json_object_get (root, "last-command-sent");
    if (json_is_string (value))
    {
      strcpy(ctx->last_command_sent, json_string_value(value));
    };
    value = json_object_get (root, "operating-mode");
    if (json_is_string (value))
    {
      sscanf(json_string_value(value), "%d", &i);
      ctx->operating_mode = i;
    };
    value = json_object_get (root, "response");
    if (json_is_string (value))
    {
      sscanf(json_string_value(value), "%d", &i);
      ctx->response = i;
    };
    value = json_object_get (root, "state");
    if (json_is_string (value))
    {
      sscanf(json_string_value(value), "%d", &i);
      ctx->state = i;
    };
  };

  return(status);

} /* t26_restore_context */


int t26_restore_statistics
  (TEST_CONTEXT *ctx,
  T26_STATISTICS *stats)

{ /* t26_restore_statistics */

  unsigned long i;
  json_t *root;
  int status;
  json_error_t status_json;
  json_t *value;


  status = ST_OK;
  root = json_load_file(ctx->statistics_filename, 0, &status_json);
  if (!root)
  {
    // just zeroize it if not found.
    memset(stats, 0, sizeof(*stats));
  };
  if (root)
  {
    value = json_object_get (root, "xrd_all_received");
    if (json_is_string (value))
    {
      sscanf(json_string_value(value), "%lu", &i);
      stats->xrd_all_received = i;
    };
    value = json_object_get (root, "xrd_card_present");
    if (json_is_string (value))
    {
      sscanf(json_string_value(value), "%lu", &i);
      stats->xrd_card_present = i;
    };
    value = json_object_get (root, "state_select_waiting");
    if (json_is_string (value))
    {
      sscanf(json_string_value(value), "%lu", &i);
      stats->state_select_waiting = i;
    };
  };

  return(status);

} /* t26_restore_statistics */


int t26_save_context
  (TEST_CONTEXT *ctx)

{ /* t26_save_context */

  FILE *contextf;
  int status;


  if (ctx->verbosity > 9)
  {
    if (ctx->state != TSEQ_STATE_IDLE)
      fprintf(ctx->log, "t26_save_context: saving non-idle state %d\n", ctx->state);
  };
  status = ST_OK;
  contextf = fopen(T26_CONTEXT_FILE, "w");
  fprintf(contextf,
"{\"operating-mode\":\"%d\",\"card-format\":\"%d\",\"state\":\"%d\"\n",
    ctx->operating_mode,
    ctx->card_format, 
    ctx->state);
  fprintf(contextf,
",\"last-command-sent\":\"%s\"",
    ctx->last_command_sent);
  fprintf(contextf, "}\n");
  fclose(contextf);
  return(status);

} /* t26_save_context */


int t26_save_statistics
  (TEST_CONTEXT *ctx,
  T26_STATISTICS *stats)

{ /* t26_save_statistics */

  FILE *statsf;
  int status;


  if (ctx->verbosity > 9)
    fprintf(ctx->log, "saving statistics.\n");
  status = ST_OK;
  statsf = fopen(T26_STATISTICS_FILE, "w");
  if (statsf != NULL)
  {
  fprintf(statsf,
"{\n");
  fprintf(statsf,
"  \"xrd_all_received\":\"%lu\",\n",
    stats->xrd_all_received);
  fprintf(statsf,
"  \"xrd_card_present\":\"%lu\",\n",
    stats->xrd_card_present);
  fprintf(statsf,
"  \"state_select_waiting\":\"%lu\",\n",
    stats->state_select_waiting);
  fprintf(statsf,
"  \"_end\":\"1\"\n}\n");
  fclose(statsf);
  };
  return(status);

} /* t26_save_statistics */

