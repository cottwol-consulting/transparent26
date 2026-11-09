#!/bin/bash
T26CMD=/opt/osdp-conformance/bin/t26-command
T26_SEND_APDU=x
${T26CMD} ${T26_SEND_APDU} PIV_select
${T26CMD} ${T26_SEND_APDU} PIV_get-CHUID

