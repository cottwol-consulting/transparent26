# Make file for source for mqtt adapter

# (C)2026 Cottwol Consulting

PROGS= osdp-mqtt-subscriber
T26_ROOT=opt/transparent26

CC=clang
CFLAGS=-c -g -I ../include -Wall -Werror
LINK=clang
LDFLAGS=-g -ljansson

all:	${PROGS}
	mkdir -p ../${T26_ROOT}/bin
	mkdir -p ../${T26_ROOT}/etc
	cp ${PROGS} ../${T26_ROOT}/bin
	cp ../integration-scripts/osdp-mqtt.service ../${T26_ROOT}/etc

clean:
	rm -f a.out *.o ${PROGS}

osdp-mqtt-subscriber:	osdp-mqtt-subscriber.o Makefile
	${LINK} -o osdp-mqtt-subscriber osdp-mqtt-subscriber.o

osdp-mqtt-subscriber.o:	osdp-mqtt-subscriber.c
	${CC} ${CFLAGS} osdp-mqtt-subscriber.c

