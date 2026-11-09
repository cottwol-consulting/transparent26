/*
  transparent26.h - definition for osdp transparent mode exerciser

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

#define EQUALS ==

#define T26_RESPOND_TO_CARD "/opt/osdp-conformance/bin/t26-respond-to-card"
#define T26_CONTEXT_FILE "/opt/osdp-conformance/t26/t26-context.json"
#define T26_SETTINGS_FILE "/opt/osdp-conformance/t26/t26-settings.json"
#define T26_STATISTICS_FILE "/opt/osdp-conformance/t26/t26-statistics.json"
#define T26_APDU_DIR "/opt/osdp-conformance/t26/apdu"

#define OT_ACTION (1001)
#define OT_HELP   (1002)
#define OT_MODE   (1003)
#define OT_PCMND  (1004)
#define OT_PDATA  (1005)
#define OT_VERBOSITY (1006)
#define OT_NOOP      (1007)

#define TSEQ_STATE_IDLE                        ( 0)
#define TSEQ_STATE_STANDBY                     ( 1)
#define TSEQ_PKOC_STATE_IDLE                   (95)
#define TSEQ_PKOC_STATE_SELECTED_WAITING       (96)
#define TSEQ_PKOC_STATE_AUTHENTICATION_WAITING (97)
#define TSEQ_PIV_STATE_SELECTED_WAITING        (86)

#define T26_OPMODE_NORMAL (0)
#define T26_OPMODE_T_IDLE (1)
#define T26_OPMODE_T_ACTIVE (2)

#define T26_CARDFORMAT_PIV      (0)
#define T26_CARDFORMAT_PKOC     (1)
#define T26_CARDFORMAT_DESFIRE3 (2)
#define T26_CARDFORMAT_GLOBALPLATFORM (3)

#define T26_RESPOND_BUSY      (-1)
#define T26_RESPOND_PR01PRES  (0)
#define T26_RESPOND_RAW       (1)
#define T26_RESPOND_PR01SCREP (2)


typedef struct test_context
{
  int verbosity;
  int option;
  FILE *log;
  char settings_filename [1024];
  char context_filename [1024];
  char statistics_filename [1024];
  unsigned char mode;
  unsigned char pcmnd;
  unsigned char payload [256];
  int payload_length;

  int state;
  int operating_mode;
  int card_format;
  int response;
  long delay_nsec;
  int delay_sec;
  unsigned char returned_mode;
  unsigned char returned_preply;
  unsigned char returned_detail_1;
  int returned_detail_lth;
  unsigned char returned_detail [2048]; // really should be max osdp size
  int next_state;
  char last_command_sent [1024];
} TEST_CONTEXT;

typedef struct t26_statistics
{
  unsigned long xrd_all_received;
  unsigned long xrd_card_present;
  unsigned long state_select_waiting;
} T26_STATISTICS;

#define ST_OK                 (0)
#define ST_TSEQ_IGNORE_MODE_0 (1)
#define ST_TSEQ_CARD_DETECTED (2)
//#define ST_TSEQ_blah          (3)
#define ST_TSEQ_RESPONSE      (4)
#define STT26_RESPOND_TO_RAW  (5)
#define STT26_OSDP_SOCKET     (6)
#define STT26_OSDP_CONNECT    (7)
#define STT26_OSDP_SOCK_WRITE (8)
#define STT26_NO_CONTEXT      ( 9)

int t26_command_osdp(TEST_CONTEXT *ctx, char *command);
char *t26_dump_hex(TEST_CONTEXT *ctx, char *prefix, unsigned char *raw_buffer, int raw_lth, char *suffix);
int t26_init_settings(TEST_CONTEXT *ctx);
int t26_initialize_context(TEST_CONTEXT *ctx);
int t26_leave_transparent_mode(TEST_CONTEXT *ctx);
int t26_respond(TEST_CONTEXT *ctx, T26_STATISTICS *stats);
int t26_restore_context(TEST_CONTEXT *ctx);
int t26_restore_statistics(TEST_CONTEXT *ctx, T26_STATISTICS *statistics);
int t26_save_context(TEST_CONTEXT *ctx);
int t26_save_statistics(TEST_CONTEXT *ctx, T26_STATISTICS *statistics);

