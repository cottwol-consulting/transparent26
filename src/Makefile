# Make file for source for transparent26

# (C)2026 Cottwol Consulting

PROGS=osdp-action-busy osdp-action-raw-t26 osdp-action-xread \
  t26-post-install t26-respond-to-card
ACTIONS=osdp_BUSY osdp_RAW osdp_XRD
T26_ROOT=opt/transparent26

INCLUDES=../include/transparent26.h

CC=clang
CFLAGS=-c -g -I ../include -Wall -Werror
LINK=clang
LDFLAGS=-g -ljansson

all:	${PROGS}
	mkdir -p ../${T26_ROOT}/bin
	cp ${PROGS} ../${T26_ROOT}/bin
	mkdir -p ../${T26_ROOT}/actions
	cp ${ACTIONS} ../${T26_ROOT}/actions

clean:
	rm -f a.out *.o ${PROGS}

osdp-action-busy:	osdp-action-busy.o t26-response-switch.o t26-utils.o Makefile
	${LINK} -o osdp-action-busy osdp-action-busy.o t26-response-switch.o t26-utils.o ${LDFLAGS}

osdp-action-raw-t26:	osdp-action-raw-t26.o t26-response-switch.o t26-utils.o Makefile
	${LINK} -o osdp-action-raw-t26 osdp-action-raw-t26.o t26-response-switch.o  t26-utils.o ${LDFLAGS}

osdp-action-xread:	osdp-action-xread.o t26-response-switch.o t26-utils.o Makefile
	${LINK} -o osdp-action-xread osdp-action-xread.o t26-response-switch.o t26-utils.o ${LDFLAGS}

t26-post-install:	t26-post-install.sh
	cp t26-post-install.sh t26-post-install
	chmod +x t26-post-install

t26-respond-to-card:	t26-respond-to-card.sh
	cp t26-respond-to-card.sh t26-respond-to-card
	chmod +x t26-respond-to-card

osdp-action-busy.o:	osdp-action-busy.c
	${CC} ${CFLAGS} osdp-action-busy.c

osdp-action-raw-t26.o:	osdp-action-raw-t26.c
	${CC} ${CFLAGS} osdp-action-raw-t26.c

osdp-action-xread.o:	osdp-action-xread.c ../include/transparent26.h ../include/t26-version.h
	${CC} ${CFLAGS} osdp-action-xread.c

t26-response-switch.o:	t26-response-switch.c ${INCLUDES}
	${CC} ${CFLAGS} t26-response-switch.c

t26-utils.o:	t26-utils.c
	${CC} ${CFLAGS} t26-utils.c

