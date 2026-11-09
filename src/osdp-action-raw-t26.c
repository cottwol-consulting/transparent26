/*
  osdp-action-raw-t26 - action routine - transparent mode after osdp_RAW

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
#include <string.h>
#include <getopt.h>


#include <transparent26.h>
#include <t26-version.h>


int main
  (int argc,
  char *argv [])

{ /* main for osdp-action-raw-t26 */

  TEST_CONTEXT *ctx;
  TEST_CONTEXT my_test_context; 
  T26_STATISTICS statistics;
  int status;


  status = ST_OK;
  ctx = &my_test_context;
  memset(ctx, 0, sizeof(*ctx));
  ctx->log = stderr;
  memset(&statistics, 0, sizeof(statistics));
  fprintf(ctx->log, "osdp-action-raw-t26 %s is in startup.\n", T26_VERSION);
  strcpy(ctx->settings_filename, T26_SETTINGS_FILE);
  strcpy(ctx->context_filename, T26_CONTEXT_FILE);
  status = t26_init_settings(ctx);
  if (status EQUALS ST_OK)
    status = t26_restore_context(ctx);
  if (status EQUALS ST_OK)
  {
    if ((ctx->response EQUALS T26_RESPOND_RAW) && (ctx->operating_mode EQUALS T26_OPMODE_T_IDLE))
    {
      fprintf(ctx->log,
"Mode is respond-to-raw, responding for card format %d\n",
  ctx->card_format);
      switch (ctx->card_format)
      {
      case T26_CARDFORMAT_PIV:
        status = t26_respond(ctx, &statistics);
        break;
      default:
        fprintf(ctx->log, "osdp_RAW response ignored.\n");
        break;
      };
    };
  };

  return(status);

} /* main for osdp-action-raw-t26 */

