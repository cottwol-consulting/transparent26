/*
  t26-response-switch - direct APDU to current processor

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


#include <transparent26.h>
#include <t26-apdu-contents.h>
#define XWR_APDU_DESFIRE3_GET_VERSION \
  "{\"command\":\"xwrite\",\"mode\":\"01\",\"pcmnd\":\"01\",\"pdata\":\"9060000000\"}"
#define XWR_APDU_GET_CARD_RECOGNITION_DATA \
  "{\"command\":\"xwrite\",\"mode\":\"01\",\"pcmnd\":\"01\",\"pdata\":\"80CA006600\"}"
#define XWR_APDU_PIV_SELECT \
  "{\"command\":\"xwrite\",\"mode\":\"01\",\"pcmnd\":\"01\",\"pdata\":\"0000A4040009A0000003080000100000\"}"
#define XWR_DONE \
  "{\"command\":\"xwrite\",\"mode\":\"01\",\"pcmnd\":\"02\",\"pdata\":\"00\"}"
char *desfire_get_version = XWR_APDU_DESFIRE3_GET_VERSION;
char *gp_get_card_recognition = XWR_APDU_GET_CARD_RECOGNITION_DATA;
char *piv_select = XWR_APDU_PIV_SELECT;
char *pkoc_select = XWR_APDU_PKOC_SELECT;
char *pkoc_authenticate = XWR_APDU_PKOC_AUTHENTICATE;
char *xread_done = XWR_DONE;


int t26_respond
  (TEST_CONTEXT *ctx,
  T26_STATISTICS *stats)

{ /* t26_respond */

  char command [1024];
  int status;


  status = ST_OK;
  switch(ctx->card_format)
  {
  default:
    fprintf(ctx->log, "responding: assuming response script should be used. card format was %d.\n",
      ctx->card_format);
    fflush(ctx->log);
    sprintf(command, "%s &", T26_RESPOND_TO_CARD);
    system(command);
    break;
  case T26_CARDFORMAT_DESFIRE3:
    switch(ctx->state)
    {
    default:
      fprintf(ctx->log, "State %d unknown for card format %d, skipping.\n",
        ctx->state, ctx->card_format);
      break;
    case TSEQ_STATE_IDLE:
      fprintf(ctx->log, "Card present while idle.\n");
      status = t26_command_osdp(ctx, desfire_get_version);
      ctx->state = TSEQ_STATE_IDLE;
      break;
    };
    break;
  case T26_CARDFORMAT_GLOBALPLATFORM:
    switch(ctx->state)
    {
    default:
      fprintf(ctx->log, "State %d unknown for card format %d, skipping.\n",
        ctx->state, ctx->card_format);
      break;
    case TSEQ_STATE_IDLE:
      fprintf(ctx->log, "Card present while idle.\n");
      status = t26_command_osdp(ctx, gp_get_card_recognition);
      ctx->state = TSEQ_STATE_IDLE;
      break;
    };
    break;
  case T26_CARDFORMAT_PKOC:
    if (ctx->verbosity > 9)
      fprintf(ctx->log, "card-format:PKOC state %d\n", ctx->state);
    switch(ctx->state)
    {
    default:
      fprintf(ctx->log, "State %d unknown for card format %d, skipping.\n",
        ctx->state, ctx->card_format);
      ctx->state = TSEQ_STATE_IDLE;
      break;
    case TSEQ_STATE_IDLE:
      fprintf(ctx->log, "Card present while idle.\n");
      status = t26_command_osdp(ctx, pkoc_select);
      ctx->state = TSEQ_PKOC_STATE_SELECTED_WAITING;
      break;
    case TSEQ_STATE_STANDBY:
      // stop the connection, we're done processing the card.
      status = t26_command_osdp(ctx, xread_done);
      if (status EQUALS ST_OK)
      {
        // furthermore drop out of transparent mode?
      };
      break;
    case TSEQ_PKOC_STATE_SELECTED_WAITING:
      if ((ctx->response EQUALS T26_RESPOND_PR01SCREP) || (ctx->response EQUALS T26_RESPOND_BUSY))
      {
        fprintf(ctx->log, "Card responded to select\n");
        status = t26_command_osdp(ctx, pkoc_authenticate);
        ctx->state = TSEQ_PKOC_STATE_AUTHENTICATION_WAITING;
      }
      else
      {
        if (ctx->verbosity > 9)
          fprintf(ctx->log, "not PR01SCREP in SELECTED_WAITING state.\n");
      };
      break;
    case TSEQ_PKOC_STATE_AUTHENTICATION_WAITING:
      if (ctx->response EQUALS T26_RESPOND_PR01SCREP)
      {
        fprintf(ctx->log, "Completed PKOC sequence, switching to standby\n");
        status = t26_command_osdp(ctx, xread_done);
        ctx->state = TSEQ_STATE_STANDBY;
      }
      else
      {
        fprintf(ctx->log, "response received but not APDU\n");
      };
      break;
    };
    break;
  case T26_CARDFORMAT_PIV:
    if (ctx->verbosity > 9)
      fprintf(ctx->log, "card-format:PIV state %d\n", ctx->state);
    switch(ctx->state)
    {
    default:
      fprintf(ctx->log, "State %d unknown for card format %d, skipping.\n",
        ctx->state, ctx->card_format);
      ctx->state = TSEQ_STATE_IDLE;
      break;
    case TSEQ_STATE_STANDBY:
      // stop the connection, we're done processing the card.
      status = t26_command_osdp(ctx, xread_done);
      if (status EQUALS ST_OK)
      {
        status = t26_leave_transparent_mode(ctx);
      };
      break;
    case TSEQ_STATE_IDLE:
      fprintf(ctx->log, "PIV card present while idle.\n");
      status = t26_command_osdp(ctx, piv_select);
      ctx->state = TSEQ_PIV_STATE_SELECTED_WAITING;
      break;
    case TSEQ_PIV_STATE_SELECTED_WAITING:
      if ((ctx->response EQUALS T26_RESPOND_PR01SCREP) || (ctx->response EQUALS T26_RESPOND_BUSY))
      {
        fprintf(ctx->log, "PIV card responded to select\n");
        status = t26_command_osdp(ctx, xread_done);
      };
      break;
    };
    break;
  };
  return(status);

} /* t26_respond */

