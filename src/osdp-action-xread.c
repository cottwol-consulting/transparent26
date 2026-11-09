/*
  osdp-action-xread - action routine for ACU receiving an osdp_XRD response

  Arguments:
    argv [1] is the PD address
    argv [2] is mode
    argv [3] is preply
    argv [4] is all the hex bytes of pdata

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
int xread_detail_setup(TEST_CONTEXT *ctx, char *arg,
  unsigned char *details, int *details_length);
int xread_setup(TEST_CONTEXT *ctx, int argc, char *argv[],
  unsigned char *xrd_response, unsigned char *mode, unsigned char *preply);


int main
  (int argc,
  char *argv [])

{ /* main for osdp-action-xread */

  TEST_CONTEXT *ctx;
  unsigned char details [1600]; // max osdp apdu
  int details_length;
  unsigned char mode;
  TEST_CONTEXT my_test_context; 
  char next_message [8192];
  unsigned char preply;
  int response; // flag set if response is recognized
  T26_STATISTICS statistics;
  unsigned char xrd_response;
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
    if (ctx->verbosity > 9)
    {
      fprintf(ctx->log, "osdp_XRD response processor %s\n", T26_VERSION);
    };
    status = t26_restore_context(ctx);
    if (status EQUALS STT26_NO_CONTEXT)
    {
      if (ctx->card_format EQUALS T26_CARDFORMAT_DESFIRE3)
      {
        fprintf(ctx->log, "Initializing context.  Card format is DESFIRE3.\n");
        ctx->state = TSEQ_STATE_IDLE;
      };
      if (ctx->card_format EQUALS T26_CARDFORMAT_GLOBALPLATFORM)
      {
        fprintf(ctx->log, "Initializing context.  Card format is GLOBALPLATFORM.\n");
        ctx->state = TSEQ_STATE_IDLE;
      };
      if (ctx->card_format EQUALS T26_CARDFORMAT_PKOC)
      {
        fprintf(ctx->log, "Initializing context.  Card format is PKOC.\n");
        ctx->state = TSEQ_STATE_IDLE;
      };
      status = ST_OK;
    };
  };
  if (status EQUALS ST_OK)
    status = t26_restore_statistics(ctx, &statistics);
  if (status EQUALS ST_OK)
  {
    if (ctx->verbosity > 9)
      fprintf(ctx->log, "card-format %d opmode %d\n",
        ctx->card_format, ctx->operating_mode);
    status = xread_setup(ctx, argc, argv, &xrd_response, &mode, &preply);
  };

  if (status EQUALS ST_OK)
  {
    details_length = 0;
    status = xread_detail_setup(ctx, argv[4], details, &details_length);
  };
  if (status EQUALS ST_OK)
  {
    response = -1;
    statistics.xrd_all_received ++;
    if (ctx->verbosity > 9)
      fprintf(ctx->log, "received now %lu\n", statistics.xrd_all_received);

    // card present?

    if ((mode EQUALS 0x01) && (preply EQUALS 0x01))
    {
      response = 1;
      ctx->response = T26_RESPOND_PR01PRES; // this is a "card present"
      statistics.xrd_card_present ++;
      if (ctx->verbosity > 9)
        fprintf(ctx->log, "present now %lu\n", statistics.xrd_card_present);
      if (ctx->verbosity > 9)
        fprintf(ctx->log,
          "Mode is respond-to-PR01PRES (XRD card present), responding\n");
      status = t26_respond(ctx, &statistics);
    };

    // incoming APDU?

    if ((mode EQUALS 0x01) && (preply EQUALS 0x02))
    {
      response = 1;
      ctx->response = T26_RESPOND_PR01SCREP;

      fprintf(ctx->log, "osdp_PR01SCREP: Rdr %02x Status %02x APDU (%d. octets)\n%s\n",
        details [0], details [1], details_length-2,
        t26_dump_hex(ctx, "  RECV APDU:", details, details_length, ""));

      status = t26_respond(ctx, &statistics);
fprintf(ctx->log, "DEBUG: responded with card response to response\n");
    };
    if (response EQUALS -1)
    {
      fprintf(ctx->log,
        "XRD Mode %02X PREPLY %02X (dlth %d.) not processed.\n",
        mode, preply, details_length);
    };
  };
  if (status EQUALS ST_OK)
  {
    status = t26_save_context(ctx);
    if (status EQUALS ST_OK)
      status = t26_save_statistics(ctx, &statistics);
  };
  if (status != ST_OK)
    fprintf(ctx->log, "osdp-action-read exit status %d.\n", status);
  return(status);

} /* main for osdp-action-xread */


int xread_detail_setup
  (TEST_CONTEXT *ctx,
  char *arg,
  unsigned char *details,
  int *details_length)

{ /* xread_detail_setup */

  int i;
  char octet [3];
  char *p;


  p = arg;
  *details_length = 0;
  while (strlen(p) > 0)
  {
    octet[0] = *p;
    octet[1] = *(p+1);
    octet[2] = 0;
    sscanf(octet, "%X", &i);
    details[*details_length] = i;
    (*details_length) ++;
    p = p + 2;
  };
  return(ST_OK);

} /* xread_detail_setup */


int xread_setup
  (TEST_CONTEXT *ctx,
  int argc,
  char *argv[],
  unsigned char *xrd_response,
  unsigned char *mode,
  unsigned char *preply)

{ /* xread_setup */

  int i;
  int status;


  status = ST_OK;
  sscanf(argv [2], "%d", &i);
  *mode = i;
  sscanf(argv [3], "%d", &i);
  *preply = i;
  if (ctx->verbosity > 9)
    fprintf(ctx->log, "DEBUG: PDATA %s\n", argv [4]);

  return(status);

} /* xread_setup */

