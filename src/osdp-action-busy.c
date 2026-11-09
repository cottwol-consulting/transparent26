/*
  osdp-action-busy - action routine for ACU receiving a busy in transparent mode

  (C)Copyright 2026 Cottwol Consulting

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
#include <string.h>
#include <getopt.h>


#include <transparent26.h>
#include <t26-version.h>


int main
  (int argc,
  char *argv [])

{ /* main for osdp-action-busy */

  TEST_CONTEXT *ctx;
  TEST_CONTEXT my_test_context; 
  char next_message [8192];
  T26_STATISTICS statistics;
  int status;


  status = ST_OK;
  ctx = &my_test_context;
  memset(ctx, 0, sizeof(*ctx));
  memset(next_message, 0, sizeof(next_message));
  ctx->log = stderr;
  ctx->verbosity = 3;
  strcpy(ctx->settings_filename, T26_SETTINGS_FILE);
  strcpy(ctx->context_filename, T26_CONTEXT_FILE);
  strcpy(ctx->statistics_filename, T26_STATISTICS_FILE);
  status = t26_init_settings(ctx);
  if (status EQUALS ST_OK)
  {
    if (ctx->verbosity > 3)
    {
      fprintf(ctx->log, "osdp_BUSY response processor %s\n", T26_VERSION);
    };
    status = t26_restore_context(ctx);
  };
  if (status EQUALS ST_OK)
    status = t26_restore_statistics(ctx, &statistics);

  if (status EQUALS ST_OK)
  {
    ctx->response = T26_RESPOND_BUSY;
    status = t26_respond(ctx, &statistics);
  };
  if (status EQUALS ST_OK)
  {
    status = t26_save_context(ctx);
    if (status EQUALS ST_OK)
      status = t26_save_statistics(ctx, &statistics);
  };
  if (status != ST_OK)
    fprintf(ctx->log, "osdp-action-busy exit status %d.\n", status);
  return(status);

} /* main for osdp-action-busy */

